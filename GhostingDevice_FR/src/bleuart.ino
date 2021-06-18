#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <Arduino.h>
#include "NewPing.h"
#include <iostream>
#include <thread>
#define TRIGGER_PIN 15
#define ECHO_PIN 7
#define MAX_DISTANCE 400
using namespace std;
int c = 0;
float x1, x2;
int pingsSinceTrigger = 0;
int numLEDPins = 8;
int LEDpins [] = {27, PIN_A5, PIN_A4, PIN_A2, PIN_A1, 16, 11, 30};
int batteryPIN = PIN_A3;
NewPing sonar (TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  
bool lightOn = false;
// BLE Service
BLEDfu  bledfu;  // OTA DFU service
BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
BLEBas  blebas;  // battery
 
unsigned long previousSonarTime = 0;
unsigned long previousLEDTime = 0;
int currentLEDRotationIndex = 0;
bool LEDRotation = false;


void toggleLEDCircle(int ONOFF){
  if(ONOFF == 1){
    for(int i = 0; i < numLEDPins; i++){
      digitalWrite(LEDpins[i],HIGH);
    }
  }
  else if(ONOFF == 0){
    for(int i = 0; i < numLEDPins; i++){
      digitalWrite(LEDpins[i],LOW);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_A0, OUTPUT);
  digitalWrite(PIN_A0, LOW);
  for(int i = 0; i < numLEDPins; i++){
    pinMode(LEDpins[i],OUTPUT);
    digitalWrite(LEDpins[i], LOW);
  }
  pinMode(batteryPIN, OUTPUT);
  digitalWrite(batteryPIN, LOW);

#if CFG_DEBUG
  // Blocking wait for connection when debug mode is enabled via IDE
  while ( !Serial ) yield();
#endif

  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  char name[18];
  name[0] = 'F';
  name[1] = 'R';
  for(int i = 2; i < 18;i++){
    name[i] = getMcuUniqueID()[i-2];
  }
  Bluefruit.setName(name); 
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("SqDev");
  bledis.setModel("GhostingConnectorDevice");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();
  x1 = sonar.ping_cm();
  x2 = sonar.ping_cm();
  pingsSinceTrigger += 2;
}
void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   *
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void loop()
{
      // unsigned long currentTime = millis();

        if (lightOn){
          x1 = x2;
          x2 = sonar.ping_cm();
          pingsSinceTrigger++;
        }
        else{
          x2 = x1;
          pingsSinceTrigger = 0;
        }
    
        if(abs(x2 - x1) >= 3){
          Serial.println(1);
        }
        else{
          Serial.println(0);
        }

        if (abs(x2 - x1) >= 3 && pingsSinceTrigger >= 3 && lightOn){
          bleuart.write("detected");
          Serial.println("detected");
          toggleLEDCircle(0);
          lightOn = false;
          pingsSinceTrigger = 0;
        }
      
     


  

  if ( bleuart.available() )
  {
    uint8_t ch;
    ch = (uint8_t) bleuart.read();
    if ((char)ch == '1'){
      toggleLEDCircle(1);
      lightOn = true;
    }
    else if ((char)ch == '0'){
      toggleLEDCircle(0);
      lightOn = false;
    }
  }

  if(pingsSinceTrigger > 100000){
    pingsSinceTrigger = 100;
  }
  delay(500);
}

// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
  toggleLEDCircle(0);
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}

