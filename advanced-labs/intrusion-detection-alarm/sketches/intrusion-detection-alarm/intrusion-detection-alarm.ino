#include <IRremote.hpp>
#include <Wire.h>

#define PIR_PIN 2
#define R_LED_PIN 3
#define Y_LED_PIN 4
#define G_LED_PIN 5

#define IR_PIN      9

#define POWER_HEXCODE 3125149440
#define OK_HEXCODE    3927310080

#define MSG_ACTIVATE    'A'
#define MSG_DEACTIVATE  'D'

enum AlarmState {
  NOT_ENGAGED,
  ENGAGED,
  ACTIVATED
};

int state = LOW;
int val = 0;

enum AlarmState alarmState = NOT_ENGAGED;
unsigned long signalCodeReceived;
boolean buzzerIsActivated = false;

void setup() {
  Serial.begin(9600);
  
  Wire.begin();

  IrReceiver.begin(IR_PIN, false);   // Start the receiver
  Serial.println("[SETUP] IR receiver set up and ready");
  
  setupPIR();
  Serial.println("[SETUP] PIR set up and ready");

  pinMode(R_LED_PIN, OUTPUT);
  pinMode(Y_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  Serial.println("[SETUP] LEDs set up and ready");

  delay(2000);
}

void loop(){
  handleState(alarmState);
  
  bool hasReceived = readInfraRedSignalCode();
  if (!hasReceived) return;

  if (alarmState == ACTIVATED) {
    if (signalCodeReceived == OK_HEXCODE) {
      alarmState = ENGAGED;
      Serial.println("ALARM IS NOW DEACTIVATED, SLAPP AV!");
      
      Wire.beginTransmission(8);
      Wire.write('D');
      Wire.endTransmission();
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

      Wire.beginTransmission(8);
      Wire.write('A');
      Wire.endTransmission();

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

        Wire.beginTransmission(8);
        Wire.write('A');
        Wire.endTransmission();

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

void sendToOtherArduino(char msg) {
  Serial.println("[I2C] Sending '" + String(msg) + "' to other Arduino...");
  Wire.beginTransmission(8);
  Wire.write(msg);
  Wire.endTransmission();
  delay(200);
}