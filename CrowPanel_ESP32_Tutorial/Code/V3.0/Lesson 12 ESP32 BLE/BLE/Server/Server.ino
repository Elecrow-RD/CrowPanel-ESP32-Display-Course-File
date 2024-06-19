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

//Include the head file about BLE
#include <BLEDevice.h>  
#include <BLEUtils.h>  
#include <BLEServer.h>  
#include <BLE2902.h> 

/**************************LVGL and UI************************
if you want to use the LVGL demo. you need to include <demos/lv_demos.h> and <examples/lv_examples.h>. 
if not, please do not include it. It will waste your Flash space.
**************************************************************/
#include <lvgl.h>
#include "ui.h"
//#include <demos/lv_demos.h>
//#include <examples/lv_examples.h>
/**************************LVGL and UI END************************/

/*******************************************************************************
   Config the display panel and touch panel in gfx_conf.h
 ******************************************************************************/
#include "gfx_conf.h"

static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf1[screenWidth * screenHeight / 8];
static lv_color_t disp_draw_buf2[screenWidth * screenHeight / 8];
static lv_disp_drv_t disp_drv;

PCA9557 Out;    //for touch timing init

//set UUID for Service and Characteristic
//you can generate a globally unique UUID from this website: https://www.uuidgenerator.net/
#define SERVICE_UUID           "12345678-1234-5678-1234-56789abcdef0"  
#define CHARACTERISTIC_UUID    "abcdef01-2345-6789-abcd-ef0123456789"  
BLEServer* pServer = NULL;  
BLECharacteristic* pCharacteristic = NULL;  


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);

   lv_disp_flush_ready( disp );

}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
   uint16_t touchX, touchY;
   bool touched = tft.getTouch( &touchX, &touchY);
   if( !touched )
   {
      data->state = LV_INDEV_STATE_REL;
   }
   else
   {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      // Serial.print( "Data x " );
      // Serial.println( touchX );

      // Serial.print( "Data y " );
      // Serial.println( touchY );
   }
}


void setup()
{
  Serial.begin(9600);

  //GPIO init
#if defined (CrowPanel_50) || defined (CrowPanel_70)
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
  pinMode(42, OUTPUT);
  digitalWrite(42, LOW);

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

#elif defined (CrowPanel_43)
  pinMode(20, OUTPUT);
  digitalWrite(20, LOW);
  pinMode(19, OUTPUT);
  digitalWrite(19, LOW);
  pinMode(35, OUTPUT);
  digitalWrite(35, LOW);
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  pinMode(0, OUTPUT);//TOUCH-CS
#endif

  //Display Prepare
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  delay(200);

  lv_init();

  delay(100);

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight/8);
  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.full_refresh = 1;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  tft.fillScreen(TFT_BLACK);

  ui_init();

  BLEDevice::init("ESP32S3_BLE_Server");  
  
  pServer = BLEDevice::createServer();  
  BLEService* pService = pServer->createService(SERVICE_UUID);  
  pCharacteristic = pService->createCharacteristic(  
    CHARACTERISTIC_UUID,  
    BLECharacteristic::PROPERTY_READ |  
    BLECharacteristic::PROPERTY_WRITE |  
    BLECharacteristic::PROPERTY_NOTIFY  
  );  
  pCharacteristic->addDescriptor(new BLE2902());  
  pService->start();  
  
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();  
  pAdvertising->addServiceUUID(SERVICE_UUID);  
  pAdvertising->setMinPreferred(0x06); 
  pAdvertising->setMinPreferred(0x12);  
  BLEDevice::startAdvertising();  
  
  Serial.println("BLE Server started"); 
  
  Serial.println( "Setup done" );

}

void loop()
{
    lv_timer_handler();
    delay(5);
    if(LED_status == 1)
    {
      String message = "A";  
      pCharacteristic->setValue((uint8_t*)message.c_str(), message.length());  
      pCharacteristic->notify();  
      LED_status = 0;
    }
    else if(LED_status == 2)
    {
      String message = "B";  
      pCharacteristic->setValue((uint8_t*)message.c_str(), message.length());  
      pCharacteristic->notify(); 
      LED_status = 0;
    }
    else{}
}











