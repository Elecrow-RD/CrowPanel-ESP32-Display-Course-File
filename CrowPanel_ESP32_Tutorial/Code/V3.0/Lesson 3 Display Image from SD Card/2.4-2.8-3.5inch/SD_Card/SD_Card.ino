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




#include <TFT_eSPI.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

int sd_init_flag = 0;

#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18
#define SD_CS 5

TFT_eSPI tft = TFT_eSPI(); 
SPIClass SD_SPI;

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(115200);

  //LCD init
  tft.init();


  if (sd_init() == 0)
  {
    Serial.println("TF init success");
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(115, 230);
    tft.println("TF card successfully mounted");
  } else {
    Serial.println("TF init fail");
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(115, 230);
    tft.println("TF card failed to mount");
  }
  tft.printf("Setup done");

  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

}

void loop() {
  // put your main code here, to run repeatedly:

    drawBmp(SD, "/2-4_1.bmp", 0, 0);//display image1
    delay(1000);
    drawBmp(SD, "/2-4_2.bmp", 0, 0);//display image2
    delay(1000);
    drawBmp(SD, "/2-4_3.bmp", 0, 0);//display image3
    delay(1000);
    drawBmp(SD, "/2-4_4.bmp", 0, 0);//display image4
    delay(1000);
    drawBmp(SD, "/2-4_5.bmp", 0, 0);//display image5
    delay(1000);
}

void drawBmp(fs::FS & fs, const char *filename, int16_t x, int16_t y) {

  if ((x >= tft.width()) || (y >= tft.height())) return;

  // Open requested file on SD card
  File bmpFS = fs.open(filename, "r");

  if (!bmpFS)
  {
    Serial.print("File not found");
    return;
  }

  uint32_t seekOffset;
  uint16_t w, h, row, col;
  uint8_t  r, g, b;

  uint32_t startTime = millis();

  if (read16(bmpFS) == 0x4D42)
  {
    read32(bmpFS);
    read32(bmpFS);
    seekOffset = read32(bmpFS);
    read32(bmpFS);
    w = read32(bmpFS);
    h = read32(bmpFS);

    if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0))
    {
      y += h - 1;

      bool oldSwapBytes = tft.getSwapBytes();
      tft.setSwapBytes(true);
      bmpFS.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3 + padding];

      for (row = 0; row < h; row++) {
        
        bmpFS.read(lineBuffer, sizeof(lineBuffer));
        uint8_t*  bptr = lineBuffer;
        uint16_t* tptr = (uint16_t*)lineBuffer;
        // Convert 24 to 16-bit colours
        for (uint16_t col = 0; col < w; col++)
        {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        // Push the pixel row to screen, pushImage will crop the line if needed
        // y is decremented as the BMP image is drawn bottom up
        tft.pushImage(x, y--, w, 1, (uint16_t*)lineBuffer);
      }
      tft.setSwapBytes(oldSwapBytes);
      Serial.print("Loaded in "); Serial.print(millis() - startTime);
      Serial.println(" ms");
    }
    else Serial.println("BMP format not recognized.");
  }
  bmpFS.close();
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

int sd_init()
{
  SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  if (!SD.begin(SD_CS, SD_SPI, 40000000))
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
  sd_init_flag = 1;
  return 0;
}


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

void createDir(fs::FS & fs, const char *path)
{
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path))
  {
    Serial.println("Dir created");
  }
  else
  {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS & fs, const char *path)
{
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path))
  {
    Serial.println("Dir removed");
  }
  else
  {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS & fs, const char *path)
{
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS & fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS & fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS & fs, const char *path1, const char *path2)
{
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2))
  {
    Serial.println("File renamed");
  }
  else
  {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS & fs, const char *path)
{
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path))
  {
    Serial.println("File deleted");
  }
  else
  {
    Serial.println("Delete failed");
  }
}

void testFileIO(fs::FS & fs, const char *path)
{
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file)
  {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len)
    {
      size_t toRead = len;
      if (toRead > 512)
      {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  }
  else
  {
    Serial.println("Failed to open file for reading");
  }

  file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++)
  {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}
