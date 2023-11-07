// Include necessary libraries for IR remote and I2C communication
#include <IRremote.hpp>
#include <Wire.h>

//Define pin numbers for various components
#define TRIG_PIN  8   // Trigger pin of the ultrasonic sensor
#define ECHO_PIN  7   // Echo pin for the ultrasonic sensor

#define R_LED_PIN 3   // Red LED pin
#define Y_LED_PIN 4   // Yellow LED pin
#define G_LED_PIN 5   // Green LED pin

#define IR_PIN      9 // Infrared receiver pin

// Define hex codes that the IR remote will sendt for specific buttons
#define POWER_HEXCODE 3125149440
#define OK_HEXCODE    3927310080

// Define messages to send to other Arduino
#define MSG_ACTIVATE    'A'
#define MSG_DEACTIVATE  'D'

// Create an enum for different states of the alarm
enum AlarmState {
  NOT_ENGAGED,  // The initial state of the alarm system
  ENGAGED,      // The state when the alarm is set but not triggered
  ACTIVATED     // The state when the alarm has been triggered
};

//Decalre a variable to hold the current state of the alarm and initialize it
enum AlarmState alarmState = NOT_ENGAGED;
unsigned long signalCodeReceived; //Variable to store the received IR code 

bool initialDistanceRead = false; //Flag to chech if the initial distance is read
long initialDistance = 0;         //Varable to store the initial distance

void setup() {
  Serial.begin(9600);   // Start serial communication at 9600 baud rate  
  Wire.begin();         // Initialize I2C communication

  // Set the ultrasonic sensor pins as inout/putput
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("[SETUP] Ultrasound sensor set up and ready");

  IrReceiver.begin(IR_PIN, false);   // Initialize the IR receiver
  Serial.println("[SETUP] IR receiver set up and ready");

  // Set the LED pins as output
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(Y_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  Serial.println("[SETUP] LEDs set up and ready");

  delay(2000); // Dealy 2 seconds before loop starts
}

void loop(){
  handleState(alarmState); //Handle the LED indicators based on the alam state

// If the alarm is engaged, measure the distance to chech for instrusion
  if (alarmState == ENGAGED) {
    int currentDistance = measureDistance();  //Get the current distance reading
    if (currentDistance == 0) return;         //If no distance is read, return

    // If it's the first reading, set it as the initial distance
    if (!initialDistanceRead) {
      initialDistance = currentDistance;
      initialDistanceRead = true;
    } else {
      // If the current distance varies to much from initial, trigger the alarm
      if (currentDistance > initialDistance + 5 || currentDistance < initialDistance - 5) {
        alarmState = ACTIVATED;
        sendToOtherArduino('A'); //Send activation message to slave arduino
      }
    }
  }
  
  // Chech if an IR signal code has been received
  bool hasReceived = readInfraRedSignalCode();
  if (!hasReceived) return;

  //If the alarm is activared, chech if the correct code is received to deactivate
  if (alarmState == ACTIVATED) {
    if (signalCodeReceived == OK_HEXCODE || signalCodeReceived == POWER_HEXCODE) {
      // Change the alarm state based on the code received and print the status
      if (signalCodeReceived == OK_HEXCODE) {
        alarmState = ENGAGED;
        Serial.println("ENGAGED");
      } else {
        alarmState = NOT_ENGAGED;
        Serial.println("NOT_ENGAGED");
      }

      Serial.println("ALARM IS NOW DEACTIVATED, SLAPP AV!"); 
      sendToOtherArduino('D'); // Send a deactivation message to slave Arduino
    }
  } else {
    delay(1000); // Delay to prevent too rapid state changes

     // Toggle the alarm state between ENGAGED and NOT_ENGAGED with the POWER button
    if (signalCodeReceived == POWER_HEXCODE) {
      if (alarmState == ENGAGED) {
        alarmState = NOT_ENGAGED;
        initialDistanceRead = false;
        Serial.println("ALARM IS NOT READY");
      } else {
        alarmState = ENGAGED;
        Serial.println("ALARM IS NOW READY");
      }
      // If the  button is pressed, activate the alarm
    } else if (signalCodeReceived == OK_HEXCODE) {
      alarmState = ACTIVATED;    

      Serial.println("ALARM IS NOW ACTIVATED, RUN!");
      sendToOtherArduino('A');
    }
  }
  signalCodeReceived = 0; // Reset the signal code received for the next loop
}

// Function to read the IR signal
bool readInfraRedSignalCode() {
  // Store the received code and print it out
  if (IrReceiver.decode())
  {
    signalCodeReceived = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(signalCodeReceived, HEX);
    Serial.println(signalCodeReceived);
    Serial.println("================");
    IrReceiver.resume();  // Prepare the IR receiver for the next value
    return true;          // Return true if a code was received
  }
  return false;           // Return false if no code was received
}

// Function to change the LED states based on the current alarm state
void handleState(enum AlarmState state) {
  switch (state) {
    case NOT_ENGAGED:
      // Turn on green LED and turn off others to indicate that the alarm is not set
      digitalWrite(Y_LED_PIN, LOW);
      digitalWrite(G_LED_PIN, HIGH);
      digitalWrite(R_LED_PIN, LOW);
      break;
    case ENGAGED:
      // Turn on yellow LED and turn off others to indicate that the alarm is set      
      digitalWrite(Y_LED_PIN, HIGH);
      digitalWrite(G_LED_PIN, LOW);
      digitalWrite(R_LED_PIN, LOW);
      break;
    case ACTIVATED:
      // Turn on red LED and turn off others to indicate that the alarm is triggered
      digitalWrite(Y_LED_PIN, LOW);
      digitalWrite(G_LED_PIN, LOW);
      digitalWrite(R_LED_PIN, HIGH);
      break;
  }
}

// Function to send a message to slave Arduino over I2C
void sendToOtherArduino(char msg) {
  Serial.println("[I2C] Sending '" + String(msg) + "' to other Arduino...");
  Wire.beginTransmission(8);  // Begin transmission to another Arduino with I2C address 8
  Wire.write(msg);            // Send the message
  Wire.endTransmission();     // End transmission
  delay(200);                 // Delay to ensure message is sent properly
}

// Function to measure distance using the ultrasonic sensor
long measureDistance() {
  // Trigger the ultrasonic pulse
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