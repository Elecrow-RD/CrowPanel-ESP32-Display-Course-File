/**************************CrowPanel ESP32 HMI Display Example Code************************
Version     :	1.0
Suitable for:	CrowPanel ESP32 HMI Display
Product link:	https://www.elecrow.com/esp32-display-series-hmi-touch-screen.html
Code	  link:	https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File
Lesson	link:	https://www.youtube.com/watch?v=WHfPH-Kr9XU
Description	:	The code is currently available based on the course on YouTube, 
				        if you have any questions, please refer to the course video: Introduction 
				        to ask questions or feedback.
**************************************************************/


#include <Wire.h>
#include <SPI.h>
#include <PCA9557.h>


/*******************************************************************************
   Config the display panel and touch panel in gfx_conf.h
 ******************************************************************************/
#include "gfx_conf.h"

PCA9557 Out;

void setup()
{
  Serial.begin(9600);

  #if defined (CrowPanel_50) || defined (CrowPanel_70)
  //touch timing init
  Wire.begin(19, 20);
  Out.reset();
  Out.setMode(IO_OUTPUT);
  Out.setState(IO0, IO_LOW);
  Out.setState(IO1, IO_LOW);
  delay(20);
  Out.setState(IO0, IO_HIGH);
  delay(100);
  Out.setMode(IO1, IO_INPUT);
  #endif

  //Display Prepare
  tft.begin();
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
}

void loop()
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch( &touchX, &touchY);
  if( touched )
  {
    tft.fillCircle(touchX, touchY, 10, TFT_WHITE);
    Serial.print( "Data x " );
    Serial.println( touchX );

    Serial.print( "Data y " );
    Serial.println( touchY );
  }
}








