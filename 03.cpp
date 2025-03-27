#include <ESP32Servo.h>
const int servoXPin = 12;
const int servoYPin = 13;
Servo servoX,servoY;
void setup(){
  servox.attach(servoXPin,500,2400);
  servoY.attach(servoYPin,500,2400);
}
int pos = 0;
void loop(){
  for(pos = 0;pos<= 180;pos+=1){
    servoX.write(pos);
    servoY.write(pos);
    delay(15);
  }
  for(pos = 180;pos >= 0;pos -= 1){
    servoX.write(pos);
    servoY.write(pos);
    delay(15);
  }
}
