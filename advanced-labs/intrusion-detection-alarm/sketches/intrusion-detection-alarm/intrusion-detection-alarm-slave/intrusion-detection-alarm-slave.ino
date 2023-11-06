#include <Wire.h>

#define BUZZER_PIN        2

#define BUZZER_FREQUENCY  500


void setup() {
  Serial.begin(9600);

  Wire.begin(8);
  Wire.onReceive(receiveEvent);
}

void loop() {
  delay(100);
}

void receiveEvent(int bytes) {
  char x = Wire.read();
  if (x == 'A') {
    zap2();
  } else if (x == 'D') {
    zap3();
  }
  Serial.println(x);
}

void zap3() {
  for (float f=0;f>10;f=f*0.85){
    tone(BUZZER_PIN,2*f);
    delay(5);
    tone(BUZZER_PIN,f);
    delay(5); 
  }
}

void zap2()
{
  for (float f=200;f>10;f=f*0.85){
    tone(BUZZER_PIN,2*f);
    delay(5);
    tone(BUZZER_PIN,f);
    delay(5); 
  }
}