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




/******************************************************************************/
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "gfx_conf.h"   //Please modify the setting of display driver according to the board you are using in gfx_conf.h.

//Modify the corresponding pin according to the circuit diagram.
#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10

SPIClass SD_SPI;

/******************************************************************************/


// SD test
void listDir(fs::FS & fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}


int sd_init()
{
  //SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
   SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SD_SPI, 80000000))
  {
    Serial.println("Card Mount Failed");
    return 1;
  }
  else
  {
    Serial.println("Card Mount Successed");
  }
  listDir(SD, "/", 0);
  Serial.println("TF Card init over.");
  return 0;
}

void setup()
{
  Serial.begin( 9600 ); /*Serial init*/

  // Init Display
  tft.begin();
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);

  if (sd_init() == 0)
  {
    Serial.println("TF init success");
  } else {
    Serial.println("TF init fail");
  }

}


void loop() {
  //display images you place in SD Card
  displayPhoto("/5-0_1.bmp",0,0);
  delay(1000);
  displayPhoto("/5-0_2.bmp",0,0);
  delay(1000);
  displayPhoto("/5-0_3.bmp",0,0);
  delay(1000);
  displayPhoto("/5-0_4.bmp",0,0);
  delay(1000);
  displayPhoto("/5-0_5.bmp",0,0);
  delay(1000);
  displayPhoto("/5-0_6.bmp",0,0);
  delay(1000);
  //displayPhoto("/image.bmp",0,0);
}

void displayPhoto(char* filename, int16_t x, int16_t y) {
  File file = SD.open(filename);

  if (!file) {
    Serial.println("Error opening file!");
    return;
  }

  if (read16(file) != 0x4D42) {
    Serial.println("File is not a BMP file!");
    return;
  }

  read32(file);
  read32(file);
  uint32_t offset = read32(file);

  read32(file);
  uint32_t width = read32(file);
  uint32_t height = read32(file);

  if (read16(file) != 1) {
    Serial.println("File is not a 24-bit bitmap!");
    return;
  }

  file.seek(offset);

  uint16_t rowSize = (width * 3 + 3) & ~3;
  uint8_t row[rowSize];

  for (uint16_t rowNumber = height; rowNumber > 0; rowNumber--) {
    file.read(row, rowSize);

    for (uint16_t colNumber = 0; colNumber < width; colNumber++) {
      uint8_t b = row[3 * colNumber + 0];
      uint8_t g = row[3 * colNumber + 1];
      uint8_t r = row[3 * colNumber + 2];

      tft.drawPixel(colNumber + x, rowNumber - 1 + y, tft.color565(r, g, b));
    }
  }
}

uint16_t read16(File& f) {
  uint16_t result;
  ((uint8_t*)&result)[0] = f.read();
  ((uint8_t*)&result)[1] = f.read();
  return result;
}

uint32_t read32(File& f) {
  uint32_t result;
  ((uint8_t*)&result)[0] = f.read();
  ((uint8_t*)&result)[1] = f.read();
  ((uint8_t*)&result)[2] = f.read();
  ((uint8_t*)&result)[3] = f.read();
  return result;
}

