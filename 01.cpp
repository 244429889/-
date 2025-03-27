#define BITS 12
#define LED_BUILTIN 2
#define SECOND_LED 4
void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(SECOND_LED,OUTPUT);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
  ledcSetup(0,5000,BITS);
  ledcAttachPin(LED_BUILTIN,0);
  ledcSetup(1,5000,BITS);
  ledcAttachPin(SECOND_LED,1);
}
void loop(){
  uint16_t val = analogRead(32);
  Serial.printf("val=%u\n",val);
  ledcWrite(0,val);
  ledcWrite(1,(4095 - val));
  delay(1000);
}
