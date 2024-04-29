void setup()
{
  Serial.begin(9600);
  Serial.println("Hello World!");
}

void loop()
{
  static uint count = 0;
  count+=1;
  delay(1000);
  Serial.println(count);
  
}
