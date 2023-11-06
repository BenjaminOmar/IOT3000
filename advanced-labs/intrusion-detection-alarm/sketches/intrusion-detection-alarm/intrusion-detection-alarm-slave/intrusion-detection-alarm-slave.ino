#include <Wire.h>

#define BUZZER_PIN        2
#define OTHER_ARDUINO_PIN 9

#define BUZZER_FREQUENCY  500

bool activated = false;

void setup() {
  Serial.begin(9600);

  Wire.begin(OTHER_ARDUINO_PIN);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // TODO
}

void receiveEvent(int bytes) {
  activated = (bool) Wire.read();
  if (activated == true) {
    Serial.println("Setting off buzzer!");
    zap2();
  }
  Wire.endTransmission();
}

void zap2()
{
  for (float f=3000;f>10;f=f*0.85){
    tone(BUZZER_PIN,2*f);
    delay(5);
    tone(BUZZER_PIN,f);
    delay(5); 
  }
}