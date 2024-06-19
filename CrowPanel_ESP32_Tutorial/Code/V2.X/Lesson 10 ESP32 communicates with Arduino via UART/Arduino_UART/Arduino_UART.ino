/**************************CrowPanel ESP32 HMI Display Example Code************************
Version      :	1.0
Suitable for :	Arduino UNO
Product link :	https://www.elecrow.com/esp32-display-series-hmi-touch-screen.html
Code	link   :	https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File
Lesson	link :	https://www.youtube.com/watch?v=WHfPH-Kr9XU
Description	 :	The code is currently available based on the course on YouTube, 
				if you have any questions, please refer to the course video: Introduction 
				to ask questions or feedback.
******************************************************************************************/

const int PIN_RED   = 9;
const int PIN_GREEN = 10;
const int PIN_BLUE  = 11;

char msg[16];  
char buf1[] = "Red";  
char buf2[] = "Green"; 
char buf3[] = "Blue";

void setup() 
{  
    pinMode(PIN_RED,   OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE,  OUTPUT);
    Serial.begin(9600);  
    setColor(0, 0, 0);
}  

void setColor(int R, int G, int B) {
  analogWrite(PIN_RED,   R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE,  B);
}

void loop() 
{
  if (Serial.available() > 0) 
  {  
    int len = Serial.readBytesUntil('\n', msg, sizeof(msg) - 1); 
    if (len > 0) 
    {  
      msg[len] = '\0'; 
      if (strcmp(buf1, msg) == 0) 
      {  
        setColor(255, 0, 0);
      }
      else if (strcmp(buf2, msg) == 0) 
      {  
        setColor(0, 255, 0);
      }
      else if (strcmp(buf3, msg) == 0)
      {  
        setColor(0, 0, 255);
      }  
    }  
  }  
}





