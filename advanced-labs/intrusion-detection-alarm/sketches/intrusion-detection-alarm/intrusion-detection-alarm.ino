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

enum AlarmState alarmState = NOT_ENGAGED;
unsigned long signalCodeReceived;

void setup() {
  Serial.begin(9600);

  IrReceiver.begin(IR_PIN, false);   // Start the receiver
  Serial.println("[SETUP] IR receiver set up and ready");
  
  setupPIR();
  Serial.println("[SETUP] PIR set up and ready");

  pinMode(R_LED_PIN, OUTPUT);
  pinMode(Y_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  Serial.println("[SETUP] LEDs set up and ready");
  
  pinMode(BUZZER_PIN, OUTPUT);
  Timer1.initialize(500);         // Initialiser timeren til å avbryte hver 500 mikrosekunder (~2kHz tone)
  Timer1.attachInterrupt(toggleBuzzerPin); // knytt interrupt-funksjonen til timeren
  Serial.println("[SETUP] Buzzer set up and ready");

  delay(2000);
}

void loop(){
  handleState(alarmState);
  
  bool hasChanged = readInfraRedSignalCode();
  if (!hasChanged) return;

  if (alarmState == ACTIVATED) {
    if (signalCodeReceived == OK_HEXCODE) {
      alarmState = ENGAGED;
      stopTone();
      Serial.println("ALARM IS NOW DEACTIVATED, SLAPP AV!");
    }
  } else {
    if (signalCodeReceived == POWER_HEXCODE) {
      if (alarmState == ENGAGED) {
        alarmState = NOT_ENGAGED;
        Serial.println("ALARM IS NOT READY");
      } else {
        alarmState = ENGAGED;
        Serial.println("ALARM IS NOW READY");
      }
    } else if (signalCodeReceived == OK_HEXCODE) {
      alarmState = ACTIVATED;
      Serial.println("ALARM IS NOW ACTIVATED, RUN!");
    }
    
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

void setupPIR() {
  pinMode(PIR_PIN, INPUT);
}

bool readInfraRedSignalCode() {
  if (IrReceiver.decode())
  {
    signalCodeReceived = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(signalCodeReceived, HEX);
    Serial.println(signalCodeReceived);
    Serial.println("================");
    IrReceiver.resume();
    return true;
  }
  return false;
}

void handleState(enum AlarmState state) {
  switch (state) {
    case NOT_ENGAGED:
      startTone(BUZZER_FREQUENCY);
      digitalWrite(Y_LED_PIN, LOW);
      digitalWrite(G_LED_PIN, HIGH);
      digitalWrite(R_LED_PIN, LOW);
      break;
    case ENGAGED:
      stopTone();
      digitalWrite(Y_LED_PIN, HIGH);
      digitalWrite(G_LED_PIN, LOW);
      digitalWrite(R_LED_PIN, LOW);
      break;
    case ACTIVATED:
      stopTone();
      digitalWrite(Y_LED_PIN, LOW);
      digitalWrite(G_LED_PIN, LOW);
      digitalWrite(R_LED_PIN, HIGH);
      break;
  }
}

void toggleBuzzerPin() {
  int buzzerState = (alarmState == ACTIVATED) ? HIGH : LOW;      // Veksle tilstanden
  digitalWrite(BUZZER_PIN, buzzerState); // Sett buzzerPin til den nye tilstanden
}

void startTone(int frequency) {
  Timer1.setPeriod(1000000 / frequency / 2); // For å få ønsket frekvens må vi dele 1 sekund (1000000 mikrosekunder) med frekvensen og deretter dele på 2 for å få både HIGH og LOW
  Timer1.start(); // Start timeren
}

void stopTone() {
  Timer1.stop(); // Stop timeren
}