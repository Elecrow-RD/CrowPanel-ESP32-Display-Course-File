#include <Wire.h>
#include <SPI.h>


/*******************************************************************************
   Config the display driver and touch pin in User_Setup.h
 ******************************************************************************/
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI(); 

void setup()
{
  Serial.begin(9600);

  //Display Prepare
  tft.begin();
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
}

void loop()
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch( &touchX, &touchY);
  if( touched )
  {
    tft.fillCircle(touchX, touchY, 5, TFT_WHITE);
    Serial.print( "Data x " );
    Serial.println( touchX );

    Serial.print( "Data y " );
    Serial.println( touchY );
  }
}








