/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

String ds;  // distance
int trigPin = 8;
int echoPin = 7;
long duration, distance;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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
  // time to dsplay result
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
  delay(2000);
}
