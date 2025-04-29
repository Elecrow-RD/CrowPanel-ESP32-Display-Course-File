#include <PCA9557.h>
#include <lvgl.h>

#include <SPI.h>
#include <Wire.h>

#include "ui.h"



//UI
#define TFT_BL 2


SPIClass& spi = SPI;
#include "gfx_conf.h"

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
//static lv_color_t *disp_draw_buf;
static lv_color_t disp_draw_buf[800 * 480 / 15];
static lv_disp_drv_t disp_drv;

/* Display flushing */
const char* ssid = "Hello_Elecrow";  
const char* password = "12345678"; // Set a password 

// These settings will affect the IP address allocation for devices connected to the Access Point  
IPAddress local_ip(192, 168, 4, 1);  // The ESP32's IP address as an Access Point is typically 192.168.4.1  
IPAddress gateway(192, 168, 4, 1);    // Gateway address  
IPAddress subnet(255, 255, 255, 0);   // Subnet mask  

WiFiServer server(22333); // Declare the server object
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{

  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  

  //lcd.fillScreen(TFT_WHITE);
#if (LV_COLOR_16_SWAP != 0)
 lcd.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);
#else
  lcd.pushImageDMA(area->x1, area->y1, w, h,(lgfx::rgb565_t*)&color_p->full);//
#endif

  lv_disp_flush_ready(disp);

}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
      Serial.print( "Data x " );
      Serial.println( data->point.x );
      Serial.print( "Data y " );
      Serial.println( data->point.y );
      uint16_t touchX = data->point.x;
      uint16_t touchY = data->point.y;
      //lcd.fillCircle(touchX, touchY, 10, TFT_BLUE); // 使用正的坐标值绘制圆 
 
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
  delay(15);
}



PCA9557 Out;
void setup()
{
  
  Serial.begin(9600);
  

  //IO
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);
  


  
  Wire.begin(19, 20);
  
  Out.reset();
  Out.setMode(IO_OUTPUT);  //Set to output mode

  Out.setState(IO0, IO_LOW);
  Out.setState(IO1, IO_LOW);
  delay(20);
  Out.setState(IO0, IO_HIGH);
  delay(100);
  Out.setMode(IO1, IO_INPUT);
  //dht20.begin();


  
  // Init Display
  lcd.begin();
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextSize(2);
  delay(200);
  


  lv_init();


  // Init touch device
  touch_init();

  screenWidth = lcd.width();
  screenHeight = lcd.height();

  lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight / 15); //4

  /* Initialize the display */
  lv_disp_drv_init(&disp_drv);
  /* Change the following line to your display resolution */
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /* Initialize the (dummy) input device driver */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
#ifdef TFT_BL
 
  //digitalWrite(TFT_BL, HIGH);
  ledcSetup(1, 300, 8);
  ledcAttachPin(TFT_BL, 1);
  ledcWrite(1, 0); /* Screen brightness can be modified by adjusting this parameter. (0-255) */
#endif
 
  #ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW); 
  delay(500);
  digitalWrite(TFT_BL, HIGH);
  #endif
  lcd.fillScreen(TFT_BLACK);
  //lv_demo_widgets(); 
  ui_init();
   // Set to Access Point mode  
  WiFi.mode(WIFI_AP);  
  
  // Configure Access Point parameters  
  WiFi.softAPConfig(local_ip, gateway, subnet);  
  WiFi.softAP(ssid, password);  
  
  // Print Access Point information  
  Serial.print("AP IP address: ");  
  Serial.println(WiFi.softAPIP());  
  
  server.begin(); // Start the server listening on port 22333  
  
  Serial.println( "Setup done" );
}

void loop()
{
   WiFiClient client = server.available(); // Try to establish a client object  
    if (client) // If a client is available  
    {  
        Serial.println("[Client connected]");  
        while (client.connected()) // If the client is still connected  
        {  
          lv_timer_handler();
          delay(5);
          if(slider_changed == 1)
          {
            int current_slider_value = lv_slider_get_value(ui_Slider1);
            uint32_t netValue = htonl(current_slider_value);  
            client.write((uint8_t*)&netValue, sizeof(netValue));  
            Serial.println(current_slider_value); // Print to the serial port  
            slider_changed = 0;
          }
        }  
        client.stop(); // End the current connection  
        Serial.println("[Client disconnected]");  
    }
    lv_timer_handler();
    delay(5);
}
