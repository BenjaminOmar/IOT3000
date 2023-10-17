/**
  Credit to LastMinuteEngineers for boilerplate code & inspiration.
  @see https://lastminuteengineers.com/arduino-keypad-tutorial/
*/

#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define R_LED_PIN 12
#define G_LED_PIN 13

#define ON  true
#define OFF false

#define MILLIS_BETWEEN_KEY_PRESS      1000
#define MILLIS_WAIT_AFTER_CODE_CHECK  5000

#define LCD_ROWS 2
#define LCD_COLS 16

#define MAX_CODE_LENGTH 4
#define ENTER_CODE_TEXT "Enter code"

const byte ROWS = 4;
const byte COLS = 4;

const char ALL_KEYS[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
const char SECRET_CODE[MAX_CODE_LENGTH] = {'1', '2', '3', '4'};

const byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
const byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

bool displayedEnterCodeText = false;

//Create an object of keypad
Keypad keypad = Keypad( makeKeymap(ALL_KEYS), rowPins, colPins, ROWS, COLS );

//Create lcd object
LiquidCrystal_I2C lcd(0x27,LCD_COLS,LCD_ROWS); //address, cols, rows

int keysLeft = MAX_CODE_LENGTH;
char keys[MAX_CODE_LENGTH];

void setup() {
  Serial.begin(9600);
  
  Serial.println("=== Access Control System ===");

  // LEDs
  setupLEDs();
  turnAllLEDs(OFF);
  Serial.println("\tLEDs ready ✔");
  
  // LCD
  setupLCD();
  displayLines("Getting ready...", "Just a minute!");
  Serial.println("\tLCD ready ✔");

  Serial.println("All components operational and ready.");

  delay(3000);
  lcd.clear();
}

void loop() {
  if (keysLeft > 0) {
    turnAllLEDs(OFF);
    if (!displayedEnterCodeText) {
      displayLines(ENTER_CODE_TEXT, "");
      displayedEnterCodeText = true;
    }

    char key = keypad.getKey();
    if (key) {
      addNewKey(key);
      Serial.println("Pressed: " + String(key));
      displayLines(ENTER_CODE_TEXT, getAsSecret('*'));
      delay(MILLIS_BETWEEN_KEY_PRESS);
    }
  } else {
    const String asString = keys;
    Serial.println("You entered: " + asString.substring(0, asString.length() - 1));

    bool isValidCode = strncmp(keys, SECRET_CODE, sizeof(keys)) == 0;
    if (isValidCode) {
      turnOnOneLED(true);
      displayLines("ACCESS GRANTED", "Welcome!");
    } else {
      turnOnOneLED(false);
      displayLines("ACCESS DENIED", "Please try again.");
    }
    
    resetKeys();
    delay(MILLIS_WAIT_AFTER_CODE_CHECK);
    
    lcd.clear();
    displayedEnterCodeText = false;
  }
}

void addNewKey(char key) {
  keys[MAX_CODE_LENGTH - keysLeft] = key;
  keysLeft--;
}

void resetKeys() {
  for (int i = 0; i < MAX_CODE_LENGTH; i++) keys[i] = 0;
  keysLeft = MAX_CODE_LENGTH;
}

void setupLEDs() {
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
}

void setupLCD() {
  lcd.init();
  lcd.home();
  lcd.backlight();
}

void displayLines(String first, String second) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(truncate(first, LCD_COLS));
  lcd.setCursor(0, 1);
  lcd.print(truncate(second, LCD_COLS));
}

String truncate(String text, const int maxLength) {
  const String ellipsis = "...";
  return text.length() > maxLength ?
    text.substring(0, maxLength - ellipsis.length()) + ellipsis : text;
}

void turnOnOneLED(bool isGreen) {
  digitalWrite(G_LED_PIN, isGreen ? HIGH : LOW);
  digitalWrite(R_LED_PIN, isGreen ? LOW : HIGH);
}

void turnAllLEDs(bool on) {
  digitalWrite(G_LED_PIN, on ? HIGH : LOW);
  digitalWrite(R_LED_PIN, on ? HIGH : LOW);
}

String getAsSecret(char symbol) {
  String hidden = "";
  for (int i = 0; i < MAX_CODE_LENGTH - keysLeft; i++) {
    hidden += symbol;
  }
  return hidden;
}
