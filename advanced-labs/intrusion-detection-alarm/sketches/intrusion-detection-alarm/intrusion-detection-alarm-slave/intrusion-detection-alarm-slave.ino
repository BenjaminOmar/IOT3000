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
    activateSound();
  } else if (x == 'D') {
    noTone(BUZZER_PIN);
  }
  Serial.println(x);
}

void activateSound()
{
  for (float f=3000;f>10;f=f*0.85){
    tone(BUZZER_PIN,2*f);
    delay(5);
    tone(BUZZER_PIN,f);
    delay(5); 
  }
}