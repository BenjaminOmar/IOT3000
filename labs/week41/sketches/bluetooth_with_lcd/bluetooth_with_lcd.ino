#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Wire.h>

#define RX_PIN 2
#define TX_PIN 4

#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
SoftwareSerial mySerial(TX_PIN, RX_PIN);

String command = "";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Ask for some patience
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  lcd.setCursor(0, 1);
  lcd.print("Ready in a bit!");
  
  Serial.println("Type AT commands!");

  delay(1500);
}

void loop() {
  if (mySerial.available()) {
    lcd.clear();

    // Receive the whole string...
    while (mySerial.available()) {
      command += (char) mySerial.read();
    }
    command.trim(); // ... and remove unwanted leading / trailing characters
    
    // Display in serial monitor
    Serial.println("Received |\t" + command);

    // Display on LCD
    lcd.setCursor(0, 0);
    lcd.print("New message!");
    lcd.setCursor(0, 1);
    lcd.print(command);
    
    command = "";

    delay(1500);
  }

  if (Serial.available()) {
    delay(10);  // Necessary delay
    mySerial.write(Serial.read());
  }
}