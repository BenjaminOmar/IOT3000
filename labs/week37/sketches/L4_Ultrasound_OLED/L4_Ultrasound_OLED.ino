//This code measures distance using an ultrasonic sensor and displays the results on an OLED screen.

#include <SPI.h>
#include <Wire.h> // Include the Wire library for I2C communication
#include <Adafruit_GFX.h> // Include the Adafruit graphics library
#include <Adafruit_SSD1306.h>  // Include the Adafruit SSD1306 OLED display library

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

String ds;  // Distance measurement string
int trigPin = 8; //Trigger pin for the ultrasonic sensor
int echoPin = 7; // Echo pin for the ultrasonic sensor
long duration, distance; // Variables to store duration and distance measurements

// Initialize the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  pinMode(trigPin, OUTPUT); // Set the trigger pin as an output
  pinMode(echoPin, INPUT); // Set the echo pin as an input

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
}

void loop() {
  // Signal a quick LOW just before giving a HIGH signal
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // After 2 micro-seconds of LOW signal, give a HIGH signal
  // to trigger the sensor
  digitalWrite(trigPin, HIGH);
  // Keep the digital signal HIGH for at least 10 micro-seconds
  // (required by HC-SR04 to activate emission of ultra-sonic waves)
  delayMicroseconds(10);
  // After 10 micro-seconds, signal a LOW
  digitalWrite(trigPin, LOW);
  // Now wait for the Ultra sonic echo to return from an obstacle
  duration = pulseIn(echoPin, HIGH);
  // Convert the distance to centimeters
  distance = (duration/2) / 29.1;
  // Print the distance on the Serial Monitor window
  Serial.print(distance);

  Serial.println(" cm");
  // time to display the result on the OLED display 
  // using code from testdrawstyles()
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println("Distance measurement");
  display.println();
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  ds = String(distance) + " cm";
  display.println(ds);
  display.display();
  delay(2000); // Delay for 2 seconds before the next measurement
}
