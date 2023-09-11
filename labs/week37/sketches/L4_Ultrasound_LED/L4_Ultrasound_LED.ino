#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

String ds;  // distance
int trigPin = 8;
int echoPin = 7;
long duration, distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   // initialize the lcd 
  lcd.init();
  lcd.backlight();
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
  lcd.clear();
  Serial.print(distance);
  Serial.println(" cm");
  // time to display result
  lcd.setCursor(0,0);
  lcd.print("Distance meter");
  ds = String(distance) + " cm";
  lcd.setCursor(0,1);
  lcd.print(ds);
  delay(2000);
}
