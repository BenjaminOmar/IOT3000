#include <IRremote.hpp>
#include <Wire.h>

#define TRIG_PIN  8
#define ECHO_PIN  7

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

enum AlarmState alarmState = NOT_ENGAGED;
unsigned long signalCodeReceived;

bool initialDistanceRead = false;
long initialDistance = 0;

void setup() {
  Serial.begin(9600);
  
  Wire.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("[SETUP] Ultrasound sensor set up and ready");

  IrReceiver.begin(IR_PIN, false);   // Start the receiver
  Serial.println("[SETUP] IR receiver set up and ready");

  pinMode(R_LED_PIN, OUTPUT);
  pinMode(Y_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  Serial.println("[SETUP] LEDs set up and ready");

  delay(2000);
}

void loop(){
  handleState(alarmState);

  if (alarmState == ENGAGED) {
    int currentDistance = measureDistance();
    if (currentDistance == 0) return;
    
    if (!initialDistanceRead) {
      initialDistance = currentDistance;
      initialDistanceRead = true;
    } else {
      if (currentDistance > initialDistance + 5 || currentDistance < initialDistance - 5) {
        alarmState = ACTIVATED;
        sendToOtherArduino('A');
      }
    }
  }
  
  bool hasReceived = readInfraRedSignalCode();
  if (!hasReceived) return;

  if (alarmState == ACTIVATED) {
    if (signalCodeReceived == OK_HEXCODE || signalCodeReceived == POWER_HEXCODE) {
      if (signalCodeReceived == OK_HEXCODE) {
        alarmState = ENGAGED;
        Serial.println("ENGAGED");
      } else {
        alarmState = NOT_ENGAGED;
        Serial.println("NOT_ENGAGED");
      }

      Serial.println("ALARM IS NOW DEACTIVATED, SLAPP AV!");
      sendToOtherArduino('D');
    }
  } else {
    delay(1000);
    
    if (signalCodeReceived == POWER_HEXCODE) {
      if (alarmState == ENGAGED) {
        alarmState = NOT_ENGAGED;
        initialDistanceRead = false;
        Serial.println("ALARM IS NOT READY");
      } else {
        alarmState = ENGAGED;
        Serial.println("ALARM IS NOW READY");
      }
    } else if (signalCodeReceived == OK_HEXCODE) {
      alarmState = ACTIVATED;

      Serial.println("ALARM IS NOW ACTIVATED, RUN!");
      sendToOtherArduino('A');
    }
  }

  signalCodeReceived = 0;
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

long measureDistance() {
  // digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // After 2 micro-seconds of LOW signal, give a HIGH signal
  // to trigger the sensor
  digitalWrite(TRIG_PIN, HIGH);
  // Keep the digital signal HIGH for at least 10 micro-seconds
  // (required by HC-SR04 to activate emission of ultra-sonic waves)
  delayMicroseconds(10);
  // After 10 micro-seconds, signal a LOW
  digitalWrite(TRIG_PIN, LOW);
  // Now wait for the Ultra sonic echo to return from an obstacle
  long duration = pulseIn(ECHO_PIN, HIGH);
  Serial.println("[ULTRASOUND] Duration = " + String(duration));
  // Convert the distance to centimeters
  long distance = (duration/2) / 29.1;
  Serial.println("[ULTRASOUND] Distance = " + String(distance) + "cm");

  return distance;
}