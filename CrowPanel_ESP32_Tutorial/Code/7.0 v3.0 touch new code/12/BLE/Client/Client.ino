#include <BLEDevice.h>
#include <Wire.h>
#include "gfx_conf.h"

#define LED_Pin 38

//BLE Server name (the other ESP32 name running the server sketch)
#define bleServerName "ESP32S3_BLE_Server"

/* UUID's of the service, characteristic that we want to read*/
// BLE Service
static BLEUUID bleServiceUUID("12345678-1234-5678-1234-56789abcdef0");


//Temperature Celsius Characteristic
static BLEUUID LEDCharacteristicUUID("abcdef01-2345-6789-abcd-ef0123456789");


//Address of the peripheral device. Address will be found during scanning...
static BLEAddress* pServerAddress;

//Characteristicd that we want to read
static BLERemoteCharacteristic* LEDCharacteristic;

static boolean doConnect = false;
static boolean connected = false;

//Variables to store temperature and humidity
char LEDChar[16] = {};

//Connect to the BLE Server that has the name, Service, and Characteristics
bool connectToServer(BLEAddress pAddress) {
  BLEClient* pClient = BLEDevice::createClient();

  // Connect to the remove BLE Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(bleServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(bleServiceUUID.toString().c_str());
    return (false);
  }

  // Obtain a reference to the characteristics in the service of the remote BLE server.
  LEDCharacteristic = pRemoteService->getCharacteristic(LEDCharacteristicUUID);

  if (LEDCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return false;
  }
  Serial.println(" - Found our characteristics");

  //Assign callback functions for the Characteristics
  LEDCharacteristic->registerForNotify(LEDNotifyCallback);
  return true;
}

//Callback function that gets called, when another device's advertisement has been received
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) {                 //Check if the name of the advertiser matches
      advertisedDevice.getScan()->stop();                              //Scan can be stopped, we found what we are looking for
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());  //Address of advertiser is the one we need
      doConnect = true;
      Serial.println("Device found. Connecting!");
      tft.println("Device found. Connecting!");
    }
  }
};

//When the BLE Server sends a new humidity reading with the notify property
static void LEDNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,
                              uint8_t* pData, size_t length, bool isNotify) {
  //store humidity value
  strncpy(LEDChar, (char*)pData, length);
  LEDChar[length] = '\0';
  Serial.println(LEDChar);
  if ((strcmp(LEDChar, "A")) == 0) {
    digitalWrite(LED_Pin, HIGH);
  } else if ((strcmp(LEDChar, "B")) == 0) {
    digitalWrite(LED_Pin, LOW);
  }
}


void setup() {

  //Start serial communication
  Serial.begin(9600);

  pinMode(LED_Pin, OUTPUT);

  //Display Prepare
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setFont(&Font2);
  tft.setTextSize(3);
  tft.setCursor(0, 0);
  Serial.println("Starting BLE Client application...");
  tft.println("Starting BLE Client application...");
  delay(200);

  //Init BLE device
  BLEDevice::init("ESP32S3_BLE_Client");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 30 seconds.
  tft.println("Scaning......");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
}

void loop() {
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
    }
    doConnect = false;
  }

  delay(1000);  // Delay a second between loops.
}
