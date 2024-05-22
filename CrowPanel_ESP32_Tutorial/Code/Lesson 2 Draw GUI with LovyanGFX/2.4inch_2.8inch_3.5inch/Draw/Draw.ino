/**************************CrowPanel ESP32 HMI Display Example Code************************
Version		 :	1.0
Suitable for :	CrowPanel ESP32 HMI Display
Product link :	https://www.elecrow.com/esp32-display-series-hmi-touch-screen.html
Code	link :	https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File
Lesson	link :	https://www.youtube.com/watch?v=WHfPH-Kr9XU
Description	 :	The code is currently available based on the course on YouTube, 
				if you have any questions, please refer to the course video: Introduction 
				to ask questions or feedback.
******************************************************************************************/




#include <Wire.h>
#include <SPI.h>


/*******************************************************************************
   Config the display driver and touch driver in User_Setup.h(TFT_eSPI library)
 ******************************************************************************/
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); 

void setup()
{
  Serial.begin(9600);

  //Display Prepare
  tft.begin();
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  tft.setRotation(1); 
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  delay(100);

  tft.fillScreen(TFT_BLUE);
  delay(1000);
  tft.fillScreen(TFT_YELLOW);
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  delay(1000);
  tft.fillScreen(TFT_WHITE);
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  tft.fillCircle    ( 20, 20      , 20, TFT_YELLOW);
  tft.setCursor(50, 100);
  tft.print("Hello, Elecrow");
  Serial.println( "Hello, my Display" );

}

void loop()
{
    delay(5);
}
