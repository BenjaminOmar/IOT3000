// This code is designed for a system detection smoke through analog setup

// Define the pin numbers for the smoke sensor (analog), buzzer, and LED
int smokePinA = A5;
int buzzerPin = 8;
int LEDPin = 4;

void setup() {
  // Start serial communication at a baud rate of 19200
  Serial.begin(19200);
   // Set the pin modes for smokePinA, LEDPin, and buzzerPin
  pinMode(smokePinA, INPUT); 
  pinMode(LEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  // Delay for 5 seconds to allow initialization
  delay(5000);
}

void loop() {
  // Read and print the analog value from the smoke sensor
  Serial.println(analogRead(smokePinA));
  // Check if the analog reading from the smoke sensor is greater than 175
  if (analogRead(smokePinA) > 175) {
    // If the smoke level is high (analog reading greater than 175):
    Serial.println("Warning, value high"); // Print a warning message
    digitalWrite(LEDPin, HIGH); // Turn on the LED
    tone(buzzerPin, 880); // Generate a tone on the buzzer
  } else {
    // If the smoke level is normal (analog reading less than or equal to 175):
    Serial.println("Normal values"); // Print a normal message
    digitalWrite(LEDPin, LOW); // Turn off the LED
    noTone(buzzerPin);  // Turn off the buzzer
  }
  // Delay for 1 second before checking the smoke sensor again
  delay(1000);
}

