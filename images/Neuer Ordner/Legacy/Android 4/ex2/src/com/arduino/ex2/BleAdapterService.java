package com.arduino.ex2;

import java.util.List;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;

public class BleAdapterService extends Service {

	private BluetoothAdapter mBluetoothAdapter;
	private BluetoothGatt mBluetoothGatt;
	private BluetoothManager mBluetoothManager;
	private Handler mActivityHandler = null;
	private BluetoothDevice device;
	
	public BluetoothDevice getDevice() {
		return device;
	}


	private boolean share_with_server=false;

	public boolean isShare_with_server() {
		return share_with_server;
	}

	public void setShare_with_server(boolean share_with_server) {
		this.share_with_server = share_with_server;
		System.out.println("XXXX share_with_server="+share_with_server);
	}


	// messages sent back to activity
	public static final int GATT_CONNECTED = 1;
	public static final int GATT_DISCONNECT = 2;
	public static final int GATT_SERVICES_DISCOVERED = 3;
	public static final int GATT_CHARACTERISTIC_READ = 4;
	public static final int GATT_REMOTE_RSSI = 5;
	public static final int MESSAGE = 6;


	// message parms
	public static final String PARCEL_UUID = "UUID";
	public static final String PARCEL_VALUE = "VALUE";
	public static final String PARCEL_RSSI = "RSSI";
	public static final String PARCEL_TEXT = "TEXT";


	// service uuids
	public static String IMMEDIATE_ALERT_SERVICE_UUID = "00001802-0000-1000-8000-00805f9b34fb";
	public static String LOSS_LINK_SERVICE_UUID       = "00001803-0000-1000-8000-00805f9b34fb";
	public static String PROXIMITY_MONITORING_SERVICE_UUID = "3E099910-293F-11E4-93BD-AFD0FE6D1DFD";

	// service characteristics
	public static String ALERT_LEVEL_CHARACTERISTIC       = "00002a06-0000-1000-8000-00805f9b34fb";
	public static String CLIENT_PROXIMITY_CHARACTERISTIC = "3E099911-293F-11E4-93BD-AFD0FE6D1DFD";

	// Ble Gatt Callback ///////////////////////
	private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {

		@Override
		public void onConnectionStateChange(BluetoothGatt gatt, int status,
				int newState) {
			if (newState == BluetoothProfile.STATE_CONNECTED) {
				sendConsoleMessage("Connected");
				Message msg = Message.obtain(mActivityHandler, GATT_CONNECTED);
				msg.sendToTarget();
				mBluetoothGatt.discoverServices();
			} else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
				sendConsoleMessage("Disconnected");
				Message msg = Message.obtain(mActivityHandler, GATT_DISCONNECT);
				msg.sendToTarget();
			}
		}

		@Override
		public void onServicesDiscovered(BluetoothGatt gatt, int status) {
			sendConsoleMessage("Services Discovered");
			Message msg = Message.obtain(mActivityHandler,
					GATT_SERVICES_DISCOVERED);
			msg.sendToTarget();
		}

		@Override
		public void onCharacteristicRead(BluetoothGatt gatt,
				BluetoothGattCharacteristic characteristic, int status) {
			if (status == BluetoothGatt.GATT_SUCCESS) {
				sendConsoleMessage("characteristic read OK");
				Bundle bundle = new Bundle();
				bundle.putString(PARCEL_UUID, characteristic.getUuid()
						.toString());
				bundle.putByteArray(PARCEL_VALUE, characteristic.getValue());
				Message msg = Message.obtain(mActivityHandler,
						GATT_CHARACTERISTIC_READ);
				msg.setData(bundle);
				msg.sendToTarget();
			} else {
				System.out.println("XXXX failed to read characteristic:"+characteristic.getUuid().toString()+" of service "+characteristic.getService().getUuid().toString()+" : status="+status);
				sendConsoleMessage("characteristic read err:"+status);
				logBondState();
			}
		}

		@Override
		public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
			if (status == BluetoothGatt.GATT_SUCCESS) {
				sendConsoleMessage("RSSI read OK");
				Bundle bundle = new Bundle();
				bundle.putInt(PARCEL_RSSI, rssi);
				Message msg = Message
						.obtain(mActivityHandler, GATT_REMOTE_RSSI);
				msg.setData(bundle);
				msg.sendToTarget();
			} else {
				sendConsoleMessage("RSSI read err:"+status);
			}
		}

	};

	// service binder ////////////////
	private final IBinder mBinder = new LocalBinder();

	public class LocalBinder extends Binder {
		BleAdapterService getService() {
			return BleAdapterService.this;
		}
	}

	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}

	@Override
	public boolean onUnbind(Intent intent) {

		if (mBluetoothGatt != null) {
			mBluetoothGatt.close();
			mBluetoothGatt = null;
		}

		return super.onUnbind(intent);
	}

	// /////////////////////////////

	@Override
	public void onCreate() {

		if (mBluetoothManager == null) {
			mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
			if (mBluetoothManager == null) {
				return;
			}
		}

		mBluetoothAdapter = mBluetoothManager.getAdapter();
		if (mBluetoothAdapter == null) {
			return;
		}

	}

	// connect to the device
	public boolean connect(final String address) {

		if (mBluetoothAdapter == null || address == null) {
			sendConsoleMessage("connect: mBluetoothAdapter=null");
			return false;
		}

		device = mBluetoothAdapter.getRemoteDevice(address);
		if (device == null) {
			sendConsoleMessage("connect: device=null");
			return false;
		}

		// set auto connect to true 
		mBluetoothGatt = device.connectGatt(this, true, mGattCallback);
		sendConsoleMessage("connect: auto connect set to true");
		return true;
	}

	// disconnect from device
	public void disconnect() {
		sendConsoleMessage("disconnect");
		if (mBluetoothAdapter == null || mBluetoothGatt == null) {
			sendConsoleMessage("disconnect: mBluetoothAdapter|mBluetoothGatt null");
			return;
		}
		mBluetoothGatt.disconnect();
	}

	// set activity the will receive the messages
	public void setActivityHandler(Handler handler) {
		mActivityHandler = handler;
	}

	// return list of supported services
	public List<BluetoothGattService> getSupportedGattServices() {
		if (mBluetoothGatt == null)
			return null;
		return mBluetoothGatt.getServices();
	}

	// writes a value to a service
	public boolean writeCharacteristic(String serviceUuid,
			String charcteristicUuid, byte[] value) {

		sendConsoleMessage("writeCharacteristic");
		if (mBluetoothAdapter == null || mBluetoothGatt == null) {
			sendConsoleMessage("writeCharacteristic: mBluetoothAdapter|mBluetoothGatt null");
			return false;
		}

		BluetoothGattService gattService = mBluetoothGatt
				.getService(java.util.UUID.fromString(serviceUuid));
		if (gattService == null) {
			sendConsoleMessage("writeCharacteristic: gattService null");
			return false;
		}
		BluetoothGattCharacteristic gattChar = gattService
				.getCharacteristic(java.util.UUID.fromString(charcteristicUuid));
		if (gattChar == null) {
			sendConsoleMessage("writeCharacteristic: gattChar null");
			return false;
		}
		gattChar.setValue(value);

		return mBluetoothGatt.writeCharacteristic(gattChar);

	}

	// read value from service
	public boolean readCharacteristic(String serviceUuid,
			String characteristicUuid) {
		sendConsoleMessage("readCharacteristic:"+characteristicUuid+" of " +serviceUuid);
		if (mBluetoothAdapter == null || mBluetoothGatt == null) {
			sendConsoleMessage("readCharacteristic: mBluetoothAdapter|mBluetoothGatt null");
			return false;
		}

		BluetoothGattService gattService = mBluetoothGatt
				.getService(java.util.UUID.fromString(serviceUuid));
		if (gattService == null) {
			sendConsoleMessage("readCharacteristic: gattService null");
			return false;
		}
		BluetoothGattCharacteristic gattChar = gattService
				.getCharacteristic(java.util.UUID.fromString(characteristicUuid));
		if (gattChar == null) {
			sendConsoleMessage("readCharacteristic: gattChar null");
			return false;
		}
		return mBluetoothGatt.readCharacteristic(gattChar);
	}

	public void readRemoteRssi() {
		if (mBluetoothAdapter == null || mBluetoothGatt == null) {
			return;
		}
		mBluetoothGatt.readRemoteRssi();
	}

	private void sendConsoleMessage(String text) {
		System.out.println("XXXX "+text);
		Message msg = Message.obtain(mActivityHandler, MESSAGE);
		Bundle data = new Bundle();
		data.putString(PARCEL_TEXT, text);
		msg.setData(data);
		msg.sendToTarget();
	}
	
	public void logBondState() {
		// what is our bond state?
		String bond_state = "";
		int bstate = device.getBondState();
		switch (bstate) {
		case BluetoothDevice.BOND_NONE:
			bond_state = "NONE";
			break;
		case BluetoothDevice.BOND_BONDING:
			bond_state = "BONDING";
			break;
		case BluetoothDevice.BOND_BONDED:
			bond_state = "BONDED";
			break;
		}
		System.out.println("XXXX bond state=" + bond_state);
	}

}
