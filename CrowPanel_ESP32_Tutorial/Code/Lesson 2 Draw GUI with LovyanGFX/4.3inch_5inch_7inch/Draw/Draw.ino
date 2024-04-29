#include <Wire.h>
#include <SPI.h>


/*******************************************************************************
   Config the display panel and touch panel in gfx_conf.h
 ******************************************************************************/
#include "gfx_conf.h"


void setup()
{
  Serial.begin(9600);

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
  tft.fillCircle    ( 100, 100      , 50, TFT_YELLOW);
  tft.setCursor(200, 240);
  tft.print("Hello, Elecrow");
  Serial.println( "Hello, my Display" );

}

void loop()
{
    delay(5);
}
