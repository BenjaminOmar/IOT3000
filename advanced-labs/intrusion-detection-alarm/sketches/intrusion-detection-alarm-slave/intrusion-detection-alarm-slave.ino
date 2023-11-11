// Include the Wire library to enable I2C communications
#include <Wire.h>

// Define the pin where the buzzer is connected
#define BUZZER_PIN        2

// Define the frequency for the buzzer sound
#define BUZZER_FREQUENCY  500

void setup() {
  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);

  // Begin I2C communication as a device with address 8
  Wire.begin(8);
  // Register a function to be called when data is received over I2C
  Wire.onReceive(receiveEvent);
}

void loop() {
  // A small delay to prevent overwhelming the processor
  delay(100);
}

// This function is called by the Wire library when data is received
void receiveEvent(int bytes) {
  // Read a character from the I2C buffer
  char x = Wire.read();
  // If the received character is 'A', activate the buzzer sound
  if (x == 'A') {
    activateSound();
    // If the received character is 'D', stop any sound on the buzzer
  } else if (x == 'D') {
    noTone(BUZZER_PIN);
  }
  // Print the received character to the serial monitor
  Serial.println(x);
}

// This function activates a descending tone sequence on the buzzer
void activateSound()
{
  for (float f=3000;f>10;f=f*0.85){
    // Start at a high frequency and decrease the frequency in a loop
    tone(BUZZER_PIN,2*f);
    // Wait for 5 milliseconds
    delay(5);
    // Set the buzzer frequency to the value of 'f'
    tone(BUZZER_PIN,f);
    // Wait for 5 milliseconds
    delay(5); 
  }
}