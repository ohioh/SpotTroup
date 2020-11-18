/*
   Bluetooth SIG Bluetooth Developer Starter Kit V5.0

   Version History

   V5.0
   Made Bluetooth library selection conditional on board Arduino type.
   Arduino 101 and Arduino Primo supported. Others may require code
   to be manually tweaked.
   Sketch file is now created in a directory of the same name for compatibility with Arduino IDE rules
   Added Health Thermometer Service with Temperature Measurement characteristic

   V4.0
   No change to Arduino lab and code

   V3.1
   Optimised code to only send "Disconnected" message to LCD display once per disconnect event
   If already alerting, alert is cancelled when a new connection is established

   V3.0
   Replaced Arduino Uno with Genuino/Arduino 101
   Introduced use of Bluetooth Developer Studio to generate Arduino skeleton code

   V2.0:
   added Proximity Monitoring Service.
   associated buzzer sound function with the Immediate Alert Service instead of the custom ButtonClick service
   removed the ButtonClick service
   added Time Monitoring Service


   V2.0.5 - March 2015
   Updated Arduino sketches to be compatible with version 0.9.7 of the Nordic Semiconductor Arduino BLE library. See https://github.com/NordicSemiconductor/ble-sdk-arduino.

*/
#include <SoftwareSerial.h>

#define SOUND_BUZZER_FOR_3_SECONDS 3
#define SOUND_BUZZER_FOR_15_SECONDS 15
#define SOUND_BUZZER_FOR_30_SECONDS 30
#define BUZZER_OFF          0
#define ONE_SECOND          1000
#define HALF_SECOND         500

#define ACTION_LINK_LOST                 1
#define ACTION_IMMEDIATE_ALERT_REQUESTED 2
#define ACTION_SET_ALERT_LEVEL_REQUESTED 3

#define ALERT_OFF   0
#define ALERT_MILD  1
#define ALERT_HIGH  2


int ledPin1     = 3;
int ledPin2     = 4;
int ledPin3     = 5;
int speakerPin  = 9; 
int thermistorPin = 2;

int alert_may_be_required = 0;
int alert_counter = BUZZER_OFF;
int buzzer_alert_level = 0;
int action = 0;

int connected = 0;

// temperature
int thermistor_pin = 0;
int thermistor_val = 0;
int temperature_subscribed = 0;
float voltage; 
float celsius;
int celsius_times_10;
unsigned long timestamp;

// optional LCD display
#define lcdTxPin 10
SoftwareSerial LCD = SoftwareSerial(0, lcdTxPin);
const int LCDdelay = 10;
char buf[16];
int test_count = 1;

// bluetooth
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
unsigned char HealthThermometer_TemperatureMeasurement_props = BLEIndicate | 0;

unsigned char initial_ll_alert_level[] = { 0x00 };

// Services and Characteristics
BLEService LinkLoss("1803");
BLECharacteristic LinkLoss_AlertLevel("2A06", LinkLoss_AlertLevel_props, 1);
BLEService ImmediateAlert("1802");
BLECharacteristic ImmediateAlert_AlertLevel("2A06", ImmediateAlert_AlertLevel_props, 1);
BLEService TxPower("1804");
BLECharacteristic TxPower_TxPowerLevel("2A07", TxPower_TxPowerLevel_props, 1);
BLEService ProximityMonitoring("3E099910293F11E493BDAFD0FE6D1DFD");
BLECharacteristic ProximityMonitoring_ClientProximity("3E099911293F11E493BDAFD0FE6D1DFD", ProximityMonitoring_ClientProximity_props, 2);
BLEService HealthThermometer("1809");
BLECharacteristic HealthThermometer_TemperatureMeasurement("2A1C", HealthThermometer_TemperatureMeasurement_props, 20);

int link_loss_alert_level = 0;
int immediate_alert_level = 0;

float readTemperature(int pin)
{
  voltage = (analogRead(pin) * 0.004882814);
  float celsius = (voltage - 0.5) * 100.0;
  return celsius;
}

void beepAndFlashAll(uint16_t delayms) {
  Serial.println(F("BEEP + FLASH ALL"));
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  if (buzzer_alert_level > 0) {
      tone(speakerPin, 262, delayms);
  }
  delay(delayms);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  delay(delayms);
}

void flash(unsigned char led, uint16_t delayms, unsigned char times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(led, HIGH);
    delay(delayms);
    digitalWrite(led, LOW);
    delay(delayms);
  }
}

void allLedsOff() {
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
}

void test() {
  buzzer_alert_level = ALERT_HIGH;
  beepAndFlashAll(ONE_SECOND);
  // optional
  clearLCD();
  test_count++;
  sprintf(buf, "Test #%d", test_count);
  LCD.print(buf);
}

// proximity profile functions

void alert_level_print(int level) {
  switch (level) {
    case ALERT_OFF:
      Serial.println(F("NO_ALERT"));
      break;

    case ALERT_MILD:
      Serial.println(F("MILD_ALERT"));
      break;

    case ALERT_HIGH:
      Serial.println(F("HIGH_ALERT"));
      break;
  }
}

int getPinNumber(uint8_t level) {
  switch (level) {
    case 0:   return ledPin1;
    case 1:   return ledPin2;
    case 2:   return ledPin3;
    default:  return ledPin1;
  }
}

// optional LCD display functions

void lcdPosition(int row, int col) {
  LCD.write(0xFE);   //command flag
  LCD.write((col + row * 64 + 128));  //position
  delay(LCDdelay);
}

void lcdText(String text) {
  clearLCD();
  LCD.print(text);
}

void clearLCD() {
  LCD.write(0xFE);   //command flag
  LCD.write(0x01);   //clear command.
  delay(LCDdelay);
}
void backlightOn() {  //turns on the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(157);    //light level.
  delay(LCDdelay);
}
void backlightOff() { //turns off the backlight
  LCD.write(0x7C);   //command flag for backlight stuff
  LCD.write(128);     //light level for off.
  delay(LCDdelay);
}
void serCommand() {  //a general function to call the command flag for issuing all other commands
  LCD.write(0xFE);
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Arduino setup"));

  Serial.println(F("Switching LEDs off"));

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(thermistorPin, OUTPUT);
  digitalWrite(thermistorPin, HIGH); 

  allLedsOff();

  // set advertising packet content
  blePeripheral.setLocalName(device_name);
  blePeripheral.setDeviceName(device_name);
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
  blePeripheral.addAttribute(HealthThermometer);
  blePeripheral.addAttribute(HealthThermometer_TemperatureMeasurement);

  // establish event handlers for indication subscription/unsubscription
  HealthThermometer_TemperatureMeasurement.setEventHandler(BLESubscribed, temperatureSubscribed);
  HealthThermometer_TemperatureMeasurement.setEventHandler(BLEUnsubscribed, temperatureUnsubscribed);

  LinkLoss_AlertLevel.setValue(initial_ll_alert_level, 1);
  Serial.println("attribute table constructed");

#if defined(ARDUINO_NRF52_PRIMO)
  // Arduino Primo Bluetooth library
  removeDfuService(true);
#endif

  // begin advertising
  blePeripheral.begin();
  Serial.println("advertising");

  // set timestamp which we use for periodic temperature indications
  timestamp = millis();

  // optional LCD display
  LCD.begin(9600);
  backlightOn() ;
  clearLCD();
  lcdPosition(0, 0);
  lcdText("Ready...");
}

void alertControl(void) {
  if (alert_counter > BUZZER_OFF && alert_counter-- > BUZZER_OFF) {
    beepAndFlashAll(HALF_SECOND);
  }
}

void loop()
{
  // uncomment to check your circuit board is OK
  // test();

  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:

  if (central) {
    lcdText("Connected");
    connected = 1;
    // if we're already alerting we should stop when a new connection is established
    alert_counter = 0;
    alert_may_be_required = 1;

    // while the central is still connected to peripheral:
    while (central.connected()) {
      if (temperature_subscribed == 1) {
        if ((millis() - timestamp) >= 1000) {
          celsius = readTemperature(thermistor_pin);
          celsius_times_10 = celsius * 10;
          timestamp = millis();
          // derive temperature measurement value (multiple fields)

          /*
             Ref: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.temperature_measurement.xml

             The Temperature Measurement characteristic is used to send a temperature measurement.
             Included in the characteristic are a Flags field (for showing the units of temperature
             and presence of optional fields), the temperature measurement value and, depending
             upon the contents of the Flags field, the time of that measurement and the temperature type.

             If the value of bit 1 of the Flags field is 0 and bit 2 is 0, the structure of the
             Temperature Measurement characteristic consists of two fields in this order;
                 Flags and Temperature Measurement Value.

             The temperature measurement field is in the IEEE 11073 32-bit FLOAT format
             
          */

          // in IEEE 11073 1st byte is Exponent and the following 3 bytes are the mantissa in big endian format
          // For example, Consider a temperature measurement of 36.4 degrees Celsius with precision of 0.1 degrees Celsius. 
          // The IEEE 11073 FLOAT-Type representation is a 32-bit value consisting of an exponent of an 8-bit signed integer 
          // followed by a mantissa of a 24-bit signed integer; here, the exponent is -1 (0xFF) and the mantissa is 364 (0x00016C). 
          // Therefore, the FLOAT-Type representation of 36.4 is 0xFF00016C.
          
          unsigned char value[5] = {0,0,0,0,0};

          // all our values are to one decimal place so we're sending the mantissa as 10 x celsius value with an exponent of -1
          // value[0] is the FLAGS field and already contains the required value of 0x00

          // little endian
          value[4] = 0xFF; // exponent of -1
          value[3] = (celsius_times_10 >> 16);
          value[2] = (celsius_times_10 >> 8) & 0xFF;
          value[1] =  celsius_times_10 & 0xFF;
          Serial.print(value[0]);
          Serial.print(",");
          Serial.print(value[1]);
          Serial.print(",");
          Serial.print(value[2]);
          Serial.print(",");
          Serial.print(value[3]);
          Serial.print(",");
          Serial.print(value[4]);
          Serial.println(",");

          Serial.print(celsius);
          Serial.println(" degrees C");
          HealthThermometer_TemperatureMeasurement.setValue(value,5);
        }
      }

      if (LinkLoss_AlertLevel.written()) {
        // application logic for handling WRITE or WRITE_WITHOUT_RESPONSE on characteristic Link Loss Alert Level goes here
        Serial.println("LinkLoss_AlertLevel.written()");
        link_loss_alert_level = LinkLoss_AlertLevel.value()[0];
        alert_level_print(link_loss_alert_level);
        int ledpin = getPinNumber(link_loss_alert_level);
        flash(ledpin, 250, 4);
      }
      if (ImmediateAlert_AlertLevel.written()) {
        // application logic for handling WRITE or WRITE_WITHOUT_RESPONSE on characteristic Immediate Alert Alert Level goes here
        Serial.println("ImmediateAlert_AlertLevel.written()");
        immediate_alert_level = ImmediateAlert_AlertLevel.value()[0];
        buzzer_alert_level = immediate_alert_level;
        alert_level_print(immediate_alert_level);
        // flash more times for higher alert levels
        alert_counter = SOUND_BUZZER_FOR_3_SECONDS + immediate_alert_level;
      }
      if (ProximityMonitoring_ClientProximity.written()) {
        // application logic for handling WRITE or WRITE_WITHOUT_RESPONSE on characteristic Proximity Monitoring Client Proximity goes here
        Serial.println("ProximityMonitoring_ClientProximity.written()");
        int proximity_band = ProximityMonitoring_ClientProximity.value()[0];
        int client_rssi = ProximityMonitoring_ClientProximity.value()[1];
        client_rssi = (256 - (int) client_rssi) * -1;
        allLedsOff();
        if (proximity_band == 0) {
          // means the user has turned off proximity sharing so we just want to switch off the LEDs
          return;
        }
        int ledpin = getPinNumber(proximity_band - 1);
        digitalWrite(ledpin, HIGH);
        sprintf(buf, "Client RSSI: %d", client_rssi);
        lcdText(buf);
      }
      alertControl();
    }

  }
  // when the central disconnects, print it out:
  if (connected == 1) {
    connected = 0;
    sprintf(buf, "Disconnected: %d", link_loss_alert_level);
    lcdText(buf);
    allLedsOff();
    if (link_loss_alert_level > 0 && alert_may_be_required == 1) {
      allLedsOff();
      buzzer_alert_level = link_loss_alert_level;
      if (link_loss_alert_level == 1) {
          alert_counter = SOUND_BUZZER_FOR_15_SECONDS;
      } else {
          alert_counter = SOUND_BUZZER_FOR_30_SECONDS;
      }
      alert_may_be_required = 0;
    }
  }
  alertControl();
}

void temperatureSubscribed(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.println(F("subscribed to temperature indications"));
  temperature_subscribed = 1;
}

void temperatureUnsubscribed(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.println(F("unsubscribed from temperature indications"));
  temperature_subscribed = 0;
}


