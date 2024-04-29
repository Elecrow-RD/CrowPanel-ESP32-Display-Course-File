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
