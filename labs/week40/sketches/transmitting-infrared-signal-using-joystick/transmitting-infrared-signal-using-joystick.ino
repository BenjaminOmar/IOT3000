#include <IRremote.h>

#define HEX_CODE_PLUS_BTN   0xAE629AD3
#define HEX_CODE_MINUS_BTN  0x85B144C2
#define X_AXIS_MIN  0
#define X_AXIS_MAX  1023

IRsend My_Sender;

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("IR sender");
}

void loop() {
  int i = analogRead(0);
  if (i == X_AXIS_MIN){
     My_Sender.sendNEC(HEX_CODE_PLUS_BTN, 32);
     My_Sender.sendNEC(0xFFFFFFFF, 32);
     Serial.println("Sending HEX = " + String(HEX_CODE_PLUS_BTN));  // Or Serial.println(HEX_CODE_PLUS_BTN, HEX);
  } else if (i == X_AXIS_MAX){
     My_Sender.sendNEC(HEX_CODE_MINUS_BTN, 32);
     My_Sender.sendNEC(0xFFFFFFFF, 32);
     Serial.println("Sending HEX = " + String(HEX_CODE_MINUS_BTN)); // Or Serial.println(HEX_CODE_MINUS_BTN, HEX);
  }
  delay(50);
}
