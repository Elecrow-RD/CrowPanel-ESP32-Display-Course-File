/**************************CrowPanel ESP32 HMI Display Example Code************************
Version     :	1.1
Suitable for:	CrowPanel ESP32 HMI Display
Product link:	https://www.elecrow.com/esp32-display-series-hmi-touch-screen.html
Code	  link:	https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File
Lesson	link:	https://www.youtube.com/watch?v=WHfPH-Kr9XU
Description	:	The code is currently available based on the course on YouTube, 
              if you have any questions, please refer to the course video: Introduction 
              to ask questions or feedback.
**************************************************************/


#include <TFT_eSPI.h>
/**************************TFT_eSPI************************
If you don't know how to configure TFT_eSPI library for the display and touch driver
Please refer to the content of the fifth lesson
**************************************************************/

#include <Arduino.h>
#include "BluetoothSerial.h"
 
// LED pin definition  
#define LED_PIN   25  
  
BluetoothSerial SerialBT;  
const char *pin = "1234"; // Bluetooth pairing code, change it according to your needs  
String slaveName = "ESP32-BT-Slave"; // Bluetooth slave device name  
String myName = "ESP32-BT-Master"; // Bluetooth master device name  
  
TFT_eSPI lcd = TFT_eSPI(); /* TFT entity */  
uint16_t calData[5] = { 557, 3263, 369, 3493, 3  };  
  
void setup() {  
  bool connected; // Boolean variable, only has true and false values  
  Serial.begin(115200);  
  
  pinMode(LED_PIN, OUTPUT); // Set LED pin to output mode  
  digitalWrite(LED_PIN, LOW); // Turn off LED  
  
  // LCD initialization  
  lcd.begin();            
  lcd.setRotation(1);   
  lcd.fillScreen(TFT_BLACK);  
  lcd.setTouch(calData);  
  delay(100);  
  // Background light pin  
  pinMode(27, OUTPUT);  
  digitalWrite(27, HIGH);  
  
  SerialBT.begin(myName, true); // Start Bluetooth serial  
  Serial.printf("The device \"%s\" started in master mode, make sure slave BT device is on!\n", myName.c_str());  
  SerialBT.setPin(pin); // Set the pairing code  
  Serial.println("Using PIN");  
  lcd.print("Pairing......");  
    
  // Connect to slave device  
  connected = SerialBT.connect(slaveName);  
  Serial.printf("Connecting to slave BT device named \"%s\"\n", slaveName.c_str());  
  if(connected) {  
    Serial.println("Connected Successfully!");  
    lcd.print("Connected Successfully!");  
  } else {  
    while(!SerialBT.connected(10000)) {  
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");  
      lcd.print("Failed to connect.");  
    }  
  }  
}  
  
void loop() {  
  // Read received bytes and add them to the message (buffer) array  
  if (SerialBT.available()){  
    char command = SerialBT.read();  
    // command.trim(); // Remove whitespace from the beginning and end of a string (not needed for a single char)  
    if (command == 'A')   
    { // If the received command is "ON"  
      digitalWrite(LED_PIN, LOW); // Turn on LED  
      Serial.println("LED is ON");  
    } else if (command == 'B')   
    { // If the received command is "OFF"  
      digitalWrite(LED_PIN, HIGH); // Turn off LED  
      Serial.println("LED is OFF"); 
    } else 
    {
      Serial.println("Unknown command");
    }
  }
}
  
