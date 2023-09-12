//this code is designed to measure the distance between the sensor and an obstacle
#include <LiquidCrystal_I2C.h> //include the LiquidCrystal_I2C library for controlling the LCD
#include <SPI.h>
#include <Wire.h> //Wire library for I2C communication

LiquidCrystal_I2C lcd(0x27,16,2);  // initialize the LCD address to 0x27, 
//16 characters wide and 2 lines

String ds;  // Create a string variable to store distance
int trigPin = 8; // Define the trigger pin for the ultrasonic sensor
int echoPin = 7; // Define the echo pin for the ultrasonic sensor
long duration, distance; // Variables to store duration and distance measurements

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  pinMode(trigPin, OUTPUT); // Set the trigger pin as an output
  pinMode(echoPin, INPUT); // Set the echo pin as an input   
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight
}

void loop() {
  // Signal a quick LOW just before giving a HIGH signal
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // After 2 micro-seconds of LOW signal, give a HIGH signal
  // to trigger the sensor
  digitalWrite(trigPin, HIGH);
  // Keep the digital signal HIGH for at least 10 microseconds
  // (required by HC-SR04 to activate emission of ultrasonic waves)
  delayMicroseconds(10);
  // After 10 micro-seconds, signal a LOW
  digitalWrite(trigPin, LOW);
  // Now wait for the Ultra sonic echo to return from an obstacle
  duration = pulseIn(echoPin, HIGH);
  // Convert the distance to centimeters
  distance = (duration/2) / 29.1;
  // Print the distance on the Serial Monitor window
  lcd.clear(); // Clear the LCD screen
  Serial.print(distance);
  Serial.println(" cm");
  // time to display result on the LCD
  lcd.setCursor(0,0); // Set the cursor to the first row, first column
  lcd.print("Distance meter"); // Display "Distance meter"
  ds = String(distance) + " cm"; // Convert the distance to a string and add " cm"
  lcd.setCursor(0,1); // Set the cursor to the second row, first column
  lcd.print(ds); // Display the distance with " cm" on the second row
  delay(2000); // Delay for 2 seconds before the next measurement
}

