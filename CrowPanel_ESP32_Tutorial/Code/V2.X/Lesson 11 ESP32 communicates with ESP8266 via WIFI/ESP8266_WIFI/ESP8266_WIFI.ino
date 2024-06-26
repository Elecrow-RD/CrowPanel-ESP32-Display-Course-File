#ifdef ESP8266
#include <ESP8266WiFi.h>   // esp8266
#include <Servo.h>
#else
#include <WiFi.h>          // esp32
#include <ESP32Servo.h>
#endif

Servo myservo;

const char* ssid     = "Hello_Elecrow";  
const char* password = "12345678";  
  
// Note: Using dots to separate the IP address parts  
const char* host = "192.168.4.1";  
const uint16_t port = 22333;  

WiFiClient client;  

void setup() {  
  Serial.begin(115200); 

  myservo.attach(D8);
  
  // Connect to WiFi network  
  Serial.println();  
  Serial.println("Connecting to WiFi...");  
  WiFi.mode(WIFI_STA); // Set to Station mode  
  WiFi.begin(ssid, password);  
  
  while (WiFi.status() != WL_CONNECTED) {  
    delay(500);  
    Serial.print(".");  
  }  
  
  Serial.println("");  
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");  
  Serial.println(WiFi.localIP());  
  
  // Attempt to connect to the server (can be moved to loop() if needed at different times)  
  if (!connectToServer()) {  
    Serial.println("Failed to connect to server");  
    // You can add retry logic here  
  }  
}  
  
void loop() {  
  // If you need to continuously read data, you can add logic here  
  // For example, send requests to the server and then read the responses 
  uint32_t netValue;   
  if (client.connected()) {  
    while (client.available()) {  
      client.readBytes((uint8_t*)&netValue, sizeof(netValue)); 
      int receivedValue = ntohl(netValue); 
      myservo.write(receivedValue );
      Serial.println(receivedValue);
    }  
  }  
}  

bool connectToServer() {  
  if (!client.connect(host, port)) {  
    Serial.println("connection failed");  
    return false;  
  }  
  Serial.println("connected to server");  
  return true;  
}






