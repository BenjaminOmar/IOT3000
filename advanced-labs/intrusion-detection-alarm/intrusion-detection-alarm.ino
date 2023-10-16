#include <IRremote.hpp>

#define PIR_PIN 2
#define R_LED_PIN 3
#define Y_LED_PIN 4
#define G_LED_PIN 5

#define BUZZER_PIN  8
#define IR_PIN      9

#define POWER_HEXCODE 4061003520
#define PLUS_HEXCODE  0xFF02FD
#define MINUS_HEXCODE 0xFF9867
#define DECODE_NEC

// Buzzer
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494

int state = LOW;
int val = 0;

bool alarm_red = false;
bool alarm_yellow = false;

unsigned long hexcode_received;

void setup() {
  IrReceiver.begin(IR_PIN, false);   // Start the receiver
  setup_pir();
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(Y_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  delay(2000); 
  Serial.begin(9600);
}

void loop(){
  if (alarm_red) {
    // TODO deactivating code
    zap2();
    digitalWrite(R_LED_PIN, HIGH);
    digitalWrite(Y_LED_PIN, LOW);
    digitalWrite(G_LED_PIN, LOW);
  } else {
    delay(3000);
    if (!alarm_yellow) { 
        digitalWrite(G_LED_PIN, HIGH);
        digitalWrite(Y_LED_PIN, LOW);
        digitalWrite(R_LED_PIN, LOW);

      bool got_power = check_for(POWER_HEXCODE);
      if (got_power) {
        alarm_yellow = true;
        Serial.println("ALARM IS NOW READY");

        digitalWrite(Y_LED_PIN, HIGH);
        digitalWrite(G_LED_PIN, LOW);
        digitalWrite(R_LED_PIN, LOW);
      }
    } else {
      bool got_power = check_for(POWER_HEXCODE);
      if (got_power) {
        alarm_yellow = false;
        Serial.println("ALARM IS NOT READY");
        digitalWrite(G_LED_PIN, HIGH);
        digitalWrite(Y_LED_PIN, LOW);
        digitalWrite(R_LED_PIN, LOW);
        return;
      }

      // READ MOTION VALUE
      val = digitalRead(PIR_PIN);   // read sensor value
      if (val == HIGH) {           // check if the sensor is HIGH
        if (state == LOW) {
          Serial.println("Motion detected!"); 
          state = HIGH;       // update variable state to HIGH
        }
      } 
      else {
          digitalWrite(R_LED_PIN, LOW); // turn LED OFF
          if (state == HIGH){
            Serial.println("Motion stopped!");
            state = LOW;       // update variable state to LOW
        }
      }
      if (state == HIGH) alarm_red = true; return;
    }
  }
}

void setup_pir() {
  pinMode(PIR_PIN, INPUT);
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

void read_ir() {
  if (IrReceiver.decode())
  {
    hexcode_received = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(hexcode_received, HEX);
    Serial.println(hexcode_received);
    Serial.println("================");
    IrReceiver.resume(); // Receive the next value
  }
}

bool check_for(unsigned long hexcode) {
  read_ir();
  return (hexcode_received == hexcode);
}
