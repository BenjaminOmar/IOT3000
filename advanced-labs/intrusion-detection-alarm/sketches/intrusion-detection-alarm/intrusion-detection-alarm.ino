#include <IRremote.hpp>
#include <TimerOne.h>

#define PIR_PIN 2
#define R_LED_PIN 3
#define Y_LED_PIN 4
#define G_LED_PIN 5

#define BUZZER_PIN  8
#define IR_PIN      9

#define POWER_HEXCODE 3125149440
#define OK_HEXCODE    3927310080

#define BUZZER_FREQUENCY  500

enum AlarmState {
  NOT_ENGAGED,
  ENGAGED,
  ACTIVATED
};

int state = LOW;
int val = 0;

int buzzerState = LOW;
enum AlarmState alarmState = NOT_ENGAGED;

unsigned long hexcodeReceived;

void setup() {
  Serial.begin(9600);

  IrReceiver.begin(IR_PIN, false);   // Start the receiver
  setup_pir();
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(Y_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  Serial.println("[INFO] Alarm set up and ready");
  
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, buzzerState);
  Timer1.initialize(500);         // Initialiser timeren til å avbryte hver 500 mikrosekunder (~2kHz tone)
  Timer1.attachInterrupt(toggleBuzzerPin); // knytt interrupt-funksjonen til timeren
  // TODO Serial.println

  delay(2000);
}

void loop(){
  handleLights(alarmState);

  read_ir();
  
  if (alarmState == ACTIVATED) {
    // TODO deactivating code

    if (checkFor(OK_HEXCODE)) {
      alarmState = ENGAGED;
      stopTone();
      Serial.println("ALARM IS NOW DEACTIVATED, SLAPP AV!");
    }
    hexcodeReceived = 0;
  } else {
    // delay(1000);

    if (checkFor(POWER_HEXCODE)) {
      if (alarmState == ENGAGED) {
        alarmState = NOT_ENGAGED;
        Serial.println("ALARM IS NOT READY");
      } else {
        alarmState = ENGAGED;
        Serial.println("ALARM IS NOW READY");
      }
    } else if (checkFor(OK_HEXCODE)) {
      alarmState = ACTIVATED;
      Serial.println("ALARM IS NOW ACTIVATED, RUN!");
    }
    hexcodeReceived = 0;
    
    if (alarmState == ENGAGED) {
      // READ MOTION VALUE
      val = digitalRead(PIR_PIN);   // read sensor value
      if (val == HIGH) {           // check if the sensor is HIGH
        if (state == LOW) {
          Serial.println("Motion detected!"); 
          state = HIGH;       // update variable state to HIGH
        }
      } else {
          digitalWrite(R_LED_PIN, LOW); // turn LED OFF
          if (state == HIGH){
            Serial.println("Motion stopped!");
            state = LOW;       // update variable state to LOW
        }
      }
      if (state == HIGH) {
        alarmState = ACTIVATED;
        startTone(BUZZER_FREQUENCY); 
        return;
      }
    }
  }
}

void setup_pir() {
  pinMode(PIR_PIN, INPUT);
}

void read_ir() {
  if (IrReceiver.decode())
  {
    hexcodeReceived = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(hexcodeReceived, HEX);
    Serial.println(hexcodeReceived);
    Serial.println("================");
    IrReceiver.resume(); // Receive the next value
  }
}

bool checkFor(unsigned long hexcode) {
  return (hexcodeReceived == hexcode);
}

void handleLights(enum AlarmState state) {
  switch (state) {
    case NOT_ENGAGED:
      digitalWrite(Y_LED_PIN, LOW);
      digitalWrite(G_LED_PIN, HIGH);
      digitalWrite(R_LED_PIN, LOW);
      break;
    case ENGAGED:
      digitalWrite(Y_LED_PIN, HIGH);
      digitalWrite(G_LED_PIN, LOW);
      digitalWrite(R_LED_PIN, LOW);
      break;
    case ACTIVATED:
      digitalWrite(Y_LED_PIN, LOW);
      digitalWrite(G_LED_PIN, LOW);
      digitalWrite(R_LED_PIN, HIGH);
      break;
  }
}

void myTone(byte pin, uint16_t frequency, uint16_t duration)
{ // input parameters: Arduino pin number, frequency in Hz, duration in milliseconds
  unsigned long startTime=millis();
  unsigned long halfPeriod= 1000000L/frequency/2;
  while (millis()-startTime< duration)
  {
    digitalWrite(BUZZER_PIN,HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(BUZZER_PIN,LOW);
    delayMicroseconds(halfPeriod);
  }
}

void toggleBuzzerPin() {
  buzzerState = !buzzerState;      // Veksle tilstanden
  digitalWrite(BUZZER_PIN, buzzerState); // Sett buzzerPin til den nye tilstanden
}

void startTone(int frequency) {
  Timer1.setPeriod(1000000 / frequency / 2); // For å få ønsket frekvens må vi dele 1 sekund (1000000 mikrosekunder) med frekvensen og deretter dele på 2 for å få både HIGH og LOW
  Timer1.start(); // Start timeren
}

void stopTone() {
  Timer1.stop(); // Stop timeren
}