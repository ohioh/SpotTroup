package com.arduino.ex2;

import java.io.IOException;
import java.nio.ByteBuffer;

import java.util.Timer;
import java.util.TimerTask;

import com.arduino.ex2.R;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.AssetFileDescriptor;
import android.graphics.PorterDuff;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Switch;

public class PeripheralControlActivity extends Activity {

	public static final String EXTRA_NAME = "name";
	public static final String EXTRA_ID = "id";

	private String mDeviceName;
	private String mDeviceAddress;
	private BleAdapterService mBluetoothLeService;
	private Timer mTimer;
	private boolean mSoundAlarmOnDisconnect = false;
	private int alert_level;

	private final ServiceConnection mServiceConnection = new ServiceConnection() {

		@Override
		public void onServiceConnected(ComponentName componentName,
				IBinder service) {
			mBluetoothLeService = ((BleAdapterService.LocalBinder) service)
					.getService();
			mBluetoothLeService.setActivityHandler(mMessageHandler);
		}

		@Override
		public void onServiceDisconnected(ComponentName componentName) {
			mBluetoothLeService = null;
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_peripheral_control);

		// read intent data
		final Intent intent = getIntent();
		mDeviceName = intent.getStringExtra(EXTRA_NAME);
		mDeviceAddress = intent.getStringExtra(EXTRA_ID);

		// show the device name
		((TextView) this.findViewById(R.id.nameTextView)).setText(mDeviceName);
		// hide the coloured rectangle used to show green/amber/red rssi
		// distance
		((LinearLayout) this.findViewById(R.id.rectangle))
				.setVisibility(View.INVISIBLE);
		// set low, mid, high rssi tolerance button colors
		((Button) this.findViewById(R.id.lowButton)).getBackground()
				.setColorFilter(0xFFFF0000, PorterDuff.Mode.MULTIPLY);
		((Button) this.findViewById(R.id.midButton)).getBackground()
				.setColorFilter(0xFF00FF00, PorterDuff.Mode.MULTIPLY);
		((Button) this.findViewById(R.id.highButton)).getBackground()
				.setColorFilter(0xFFFF9000, PorterDuff.Mode.MULTIPLY);

		// disable the noise button
		((Button) PeripheralControlActivity.this.findViewById(R.id.noiseButton))
				.setEnabled(false);

		Switch share_switch = (Switch) this.findViewById(R.id.switch1);
		share_switch.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			public void onCheckedChanged(CompoundButton buttonView,
					boolean isChecked) {
				if (mBluetoothLeService != null) {
					mBluetoothLeService.setShare_with_server(isChecked);
					if (!isChecked) {
						showMsg("Switched off sharing proximity data with the Proximity Monitoring Service");
						// write 0,0 to cause Arduino to switch off all LEDs
						if (mBluetoothLeService.writeCharacteristic(
								BleAdapterService.PROXIMITY_MONITORING_SERVICE_UUID,
								BleAdapterService.CLIENT_PROXIMITY_CHARACTERISTIC,
								new byte[] { 0 , 0 })) {
						} else {
							showMsg("Failed to inform Arduino sharing has been disabled");
						}
					}
				}
			}
		});

		// connect to the Bluetooth smart service
		Intent gattServiceIntent = new Intent(this, BleAdapterService.class);
		bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		stopTimer();
		unbindService(mServiceConnection);
		mBluetoothLeService = null;
	}

	// set low rssi tolerance
	public void onLow(View view) {
		if (mBluetoothLeService.writeCharacteristic(
				BleAdapterService.LOSS_LINK_SERVICE_UUID,
				BleAdapterService.ALERT_LEVEL_CHARACTERISTIC, new byte[] { 0 })) {
			setAlertLevel(0);
			showMsg("alert_level set to 0");
		} else {
			showMsg("Failed to set alert_level set to 0");
		}
	}

	// set mid rssi tolerance
	public void onMid(View view) {
		if (mBluetoothLeService.writeCharacteristic(
				BleAdapterService.LOSS_LINK_SERVICE_UUID,
				BleAdapterService.ALERT_LEVEL_CHARACTERISTIC, new byte[] { 1 })) {
			setAlertLevel(1);
			showMsg("alert_level set to 1");
		} else {
			showMsg("Failed to set alert_level set to 1");
		}
	}

	// set high rssi tolerance
	public void onHigh(View view) {
		if (mBluetoothLeService.writeCharacteristic(
				BleAdapterService.LOSS_LINK_SERVICE_UUID,
				BleAdapterService.ALERT_LEVEL_CHARACTERISTIC, new byte[] { 2 })) {
			setAlertLevel(2);
			showMsg("alert_level set to 2");
		} else {
			showMsg("Failed to set alert_level set to 2");
		}
	}

	public void onNoise(View view) {
		if (mBluetoothLeService.writeCharacteristic(
				BleAdapterService.IMMEDIATE_ALERT_SERVICE_UUID,
				BleAdapterService.ALERT_LEVEL_CHARACTERISTIC,
				new byte[] { (byte) alert_level })) {
			showMsg("Immediate Alert, level " + alert_level);
		}
	}

	public void onConnect(View view) {
		showMsg("onConnect");
		if (mBluetoothLeService != null) {
			if (mBluetoothLeService.connect(mDeviceAddress)) {
				((Button) PeripheralControlActivity.this
						.findViewById(R.id.connectButton)).setEnabled(false);
			} else {
				showMsg("onConnect: failed to connect");
			}
		} else {
			showMsg("onConnect: mBluetoothLeService=null");
		}
	}

	// Service message handler //////////////////
	private Handler mMessageHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {

			Bundle bundle;

			switch (msg.what) {
			case BleAdapterService.GATT_CONNECTED:
				((Button) PeripheralControlActivity.this
						.findViewById(R.id.connectButton)).setEnabled(false);
				// we're connected

				mBluetoothLeService.logBondState();

				System.out
						.println("XXXX reading ALERT LEVEL of LINK LOSS SERVICE");
				PeripheralControlActivity.this.mBluetoothLeService
						.readCharacteristic(
								BleAdapterService.LOSS_LINK_SERVICE_UUID,
								BleAdapterService.ALERT_LEVEL_CHARACTERISTIC);

				break;
			case BleAdapterService.GATT_DISCONNECT:
				((Button) PeripheralControlActivity.this
						.findViewById(R.id.connectButton)).setEnabled(true);
				PeripheralControlActivity.this.stopTimer();
				if (PeripheralControlActivity.this.mSoundAlarmOnDisconnect) {
					PeripheralControlActivity.this.playSound();
					// set to false so when its fluctuating between
					// connected and disconnected it doesnt keep sounding the
					// alarm
					PeripheralControlActivity.this.mSoundAlarmOnDisconnect = false;
				}
				break;
			case BleAdapterService.GATT_SERVICES_DISCOVERED:

				// for clarity we will assume we have connected to the arduino
				// board
				// and its setup with the link service && push button service
				// correctly
				// see
				// http://developer.android.com/guide/topics/connectivity/bluetooth-le.html
				// for an example of enumerating the available services

				System.out.println("XXXX Services discovered");
				mBluetoothLeService.logBondState();
				// enable the noise button
				((Button) PeripheralControlActivity.this
						.findViewById(R.id.noiseButton)).setEnabled(true);

				// show the rssi distance colored rectangle
				((LinearLayout) PeripheralControlActivity.this
						.findViewById(R.id.rectangle))
						.setVisibility(View.VISIBLE);
				// start off the rssi reading timer
				PeripheralControlActivity.this.startReadRssiTimer();

				break;
			case BleAdapterService.GATT_CHARACTERISTIC_READ:
				bundle = msg.getData();
				if (bundle.get(BleAdapterService.PARCEL_UUID).toString()
						.equals(BleAdapterService.ALERT_LEVEL_CHARACTERISTIC)) {
					byte[] b = bundle
							.getByteArray(BleAdapterService.PARCEL_VALUE);
					PeripheralControlActivity.this.setAlertLevel(b[0]);
				}
				break;
			case BleAdapterService.GATT_REMOTE_RSSI:
				bundle = msg.getData();
				int rssi = bundle.getInt(BleAdapterService.PARCEL_RSSI);
				PeripheralControlActivity.this.updateRssi(rssi);
				break;
			case BleAdapterService.MESSAGE:
				bundle = msg.getData();
				String text = bundle.getString(BleAdapterService.PARCEL_TEXT);
				showMsg(text);
			}
		}

	};

	// helper functions ////////////////

	private void setAlertLevel(int level) {
		alert_level = level;
		// update UI
		((TextView) findViewById(R.id.alertTextView)).setText("Alert Level = "
				+ Integer.toString(level));
		// to sound alarm when set to high alert and device disconnects
		mSoundAlarmOnDisconnect = (level == 2) ? true : false;
	}

	private void playSound() {
		MediaPlayer mp = new MediaPlayer();
		AssetFileDescriptor afd = getResources().openRawResourceFd(R.raw.beep);
		try {
			mp.setDataSource(afd.getFileDescriptor(), afd.getStartOffset(),
					afd.getLength());
			mp.prepare();
			mp.start();
			afd.close();

		} catch (IllegalArgumentException e) {
		} catch (IllegalStateException e) {
		} catch (IOException e) {
		}
	}

	// rssi read timer functions //////////

	private void startReadRssiTimer() {
		mTimer = new Timer();
		mTimer.schedule(new TimerTask() {
			@Override
			public void run() {
				mBluetoothLeService.readRemoteRssi();
			}

		}, 0, 2000);
	}

	private void stopTimer() {
		if (mTimer != null) {
			mTimer.cancel();
			mTimer = null;
		}
	}

	private void updateRssi(int rssi) {
		((TextView) findViewById(R.id.rssiTextView)).setText("RSSI = "
				+ Integer.toString(rssi));
		LinearLayout layout = ((LinearLayout) PeripheralControlActivity.this
				.findViewById(R.id.rectangle));

		byte proximity_band = 3;
		if (rssi < -80) {
			layout.setBackgroundColor(0xFFFF0000);
		} else if (rssi < -50) {
			layout.setBackgroundColor(0xFFFF8A01);
			proximity_band = 2;
		} else {
			layout.setBackgroundColor(0xFF00FF00);
			proximity_band = 1;
		}
		layout.invalidate();
		if (mBluetoothLeService.isShare_with_server()) {
			showMsg("sharing proximity data with the Proximity Monitoring Service");
			if (mBluetoothLeService.writeCharacteristic(
					BleAdapterService.PROXIMITY_MONITORING_SERVICE_UUID,
					BleAdapterService.CLIENT_PROXIMITY_CHARACTERISTIC,
					new byte[] { proximity_band, (byte) rssi })) {
				showMsg("proximity data shared: proximity_band="
						+ proximity_band + ",rssi=" + rssi);
			} else {
				showMsg("Failed to share proximity data");
			}
		}
	}

	private void showMsg(String msg) {
		System.out.println("XXXX " + msg);
		((TextView) this.findViewById(R.id.msgTextView)).setText(msg);
	}

}