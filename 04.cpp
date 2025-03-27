#include<Adafruit_MPU6050.h>
#include<Adafruit_Sensor.h>
#include<Wire.h>
#include<Adafruit_SSD1306.h>
#include<RTClib.h>

#define S_WIDTH 128	// OLED display width, in pixels
#define S_HEIGHT 64	// OLED display height, in pixels

RTC_DS1307 rtc;
Adafruit_MPU6050 mpu_sensor;

float x_acc, y_acc, z_acc; int earthquake_lvl = 0;
void setup() { 
  Serial.begin(115200);
  while(!Serial)
  delay(20); 
  if(!mpu_sensor.begin(0x69)){
    while(1){ 
      delay(20);
    }
  }
  rtc.begin();
  if(!scrn.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed")); 
    for (;;)
      ;
  }
  scrn.display(); 
  delay(2000);
 
  scrn.clearDisplay();
}


void loop() { 
  delay(2000);
  sensors_event_t acc, gcc, temp; mpu_sensor.getEvent(&acc,&gcc,&temp);
  
  x_acc = abs(acc.acceleration.x);
  y_acc = abs(acc.acceleration.y); 
  z_acc = abs(acc.acceleration.z);

  else if (max_acceleration >= 0.8 && max_acceleration < 2.5) earthquake_lvl = 1;
  else if (max_acceleration >= 2.5 && max_acceleration < 8.0) earthquake_lvl = 2;
  else if (max_acceleration >= 8.0 && max_acceleration < 25.0) earthquake_lvl= 3;
  else if (max_acceleration >= 25.0 && max_acceleration < 80.0) earthquake_lvl= 4;
  else if (max_acceleration >= 80.0 && max_acceleration < 250.0) earthquake_lvl = 5;
  else if (max_acceleration >= 250.0 && max_acceleration < 400.0) earthquake_lvl = 6;
  else earthquake_lvl = 7;

  scrn.clearDisplay(); 
  scrn.setTextSize(1.5);
 

  int16_t x = (S_WIDTH - (6 * 10)) / 2; 
  scrn.setCursor(x, 0); 
  scrn.print("earthquake:");

  scrn.setTextSize(1.5); 
  scrn.setTextColor(SSD1306_WHITE); 
  scrn.setCursor(0, 10);

  DateTime now = rtc.now(); 
  scrn.print("Date: "); 
  scrn.print(now.year(), DEC);
    scrn.print("/"); 
  scrn.print(now.month(), DEC); 
  scrn.print("/"); 
  scrn.println(now.day(), DEC);


  scrn.print("Time: "); 
  scrn.print(now.hour(), DEC); 
  scrn.print(":"); 
  scrn.print(now.minute(), DEC); 
  scrn.print(":"); 
  scrn.println(now.second(), DEC);

  scrn.print("Grade: "); 
  scrn.println(earthquake_lvl);


  Serial.print("Acceleration X: "); 
  Serial.print(acc.acceleration.x); 
  Serial.print(", Y: "); 
  Serial.print(acc.acceleration.y); 
  Serial.print(", Z: "); 
  Serial.print(acc.acceleration.z); 
  Serial.println(" m/s^2"); 

  Serial.print("Rotation X: "); 
  Serial.print(gcc.gyro.x); 
  Serial.print(", Y:"); 
  Serial.print(gcc.gyro.y);
  Serial.print(", Z: "); 
  Serial.print(gcc.gyro.z); 
  Serial.println(" rad/s"); 

  Serial.print("Temperature: ");
  Serial.print(temp.temperature); 
  Serial.println(" degC"); 
  Serial.println(""); 
  scrn.display();
}
