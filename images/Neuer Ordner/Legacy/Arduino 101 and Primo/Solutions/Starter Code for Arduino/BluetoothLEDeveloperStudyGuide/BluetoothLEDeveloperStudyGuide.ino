/*
 * Author: Martin Woolley
 *
 * Version History
 *  
 * V2.0
 * Made Bluetooth library selection conditional on board Arduino type.
 * Arduino 101 and Arduino Primo supported. Others may require code
 * to be manually tweaked.
 * 
 * V1.0: 
 * First version
 *
 */
 
  #if defined(ARDUINO_ARC32_TOOLS)
    // Arduino 101 Bluetooth library
    #include <CurieBLE.h>
  #elif defined(ARDUINO_NRF52_PRIMO)
    // Arduino Primo Bluetooth library
    #include <BLEPeripheral.h>
  #else
    // Other - defaulting to BLEPeripheral.h which may or may not work. If you're using a board other than Primo or 101 you
    // may need to do some tinkering.
    #include <BLEPeripheral.h>
  #endif


// BLE objects
BLEPeripheral blePeripheral;


// GAP properties
char device_name[] = "BDSK";

// Characteristic Properties
unsigned char LinkLoss_AlertLevel_props = BLERead | BLEWrite | 0;
unsigned char ImmediateAlert_AlertLevel_props = BLEWriteWithoutResponse | 0;
unsigned char TxPower_TxPowerLevel_props = BLERead | 0;
unsigned char ProximityMonitoring_ClientProximity_props = BLEWriteWithoutResponse | 0;

// Services and Characteristics
BLEService LinkLoss("1803");

BLECharacteristic LinkLoss_AlertLevel("2A06", LinkLoss_AlertLevel_props, 1);
BLEService ImmediateAlert("1802");

BLECharacteristic ImmediateAlert_AlertLevel("2A06", ImmediateAlert_AlertLevel_props, 1);
BLEService TxPower("1804");

BLECharacteristic TxPower_TxPowerLevel("2A07", TxPower_TxPowerLevel_props, 1);
BLEService ProximityMonitoring("3E099910293F11E493BDAFD0FE6D1DFD");

BLECharacteristic ProximityMonitoring_ClientProximity("3E099911293F11E493BDAFD0FE6D1DFD", ProximityMonitoring_ClientProximity_props, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("setup()");

// set advertising packet content
  blePeripheral.setLocalName(device_name);
  
  blePeripheral.setAdvertisedServiceUuid("1803");

// add services and characteristics
  blePeripheral.addAttribute(LinkLoss);
  blePeripheral.addAttribute(LinkLoss_AlertLevel);
  blePeripheral.addAttribute(ImmediateAlert);
  blePeripheral.addAttribute(ImmediateAlert_AlertLevel);
  blePeripheral.addAttribute(TxPower);
  blePeripheral.addAttribute(TxPower_TxPowerLevel);
  blePeripheral.addAttribute(ProximityMonitoring);
  blePeripheral.addAttribute(ProximityMonitoring_ClientProximity);
  Serial.println("attribute table constructed");

  #if defined(ARDUINO_NRF52_PRIMO)
    // Arduino Primo Bluetooth library
    removeDfuService(true);
  #endif
  
// begin advertising
  blePeripheral.begin();
  Serial.println("advertising");
  Serial.println("Bluetooth Developer Starter Kit");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {

        if (LinkLoss_AlertLevel.written()) {
        // application logic for handling WRITE or WRITE_WITHOUT_RESPONSE on characteristic Link Loss Alert Level goes here
         Serial.println("LinkLoss_AlertLevel.written()");

        }
        if (ImmediateAlert_AlertLevel.written()) {
        // application logic for handling WRITE or WRITE_WITHOUT_RESPONSE on characteristic Immediate Alert Alert Level goes here
         Serial.println("ImmediateAlert_AlertLevel.written()");

        }
        if (ProximityMonitoring_ClientProximity.written()) {
        // application logic for handling WRITE or WRITE_WITHOUT_RESPONSE on characteristic Proximity Monitoring Client Proximity goes here
         Serial.println("ProximityMonitoring_ClientProximity.written()");

        }
    }
    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}

