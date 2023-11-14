/**
  Credit to LastMinuteEngineers for boilerplate code & inspiration.
  @see https://lastminuteengineers.com/arduino-keypad-tutorial/
*/

// EasyLogger configuration (must be BEFORE include)
#define LOG_LEVEL       LOG_LEVEL_DEBUG
#define LOG_FORMATTING  LOG_FORMATTING_NOTIME

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <EasyLogger.h>

#define BAUD_RATE 9600

#define R_LED_PIN 12
#define G_LED_PIN 13

#define ON  true
#define OFF false

#define MILLIS_BETWEEN_KEY_PRESS 1000
#define MILLIS_AFTER_CODE_CHECK  5000

#define LCD_ROWS 2
#define LCD_COLS 16

#define MAX_CODE_LENGTH 4
#define ENTER_CODE_TEXT "Enter code"
#define SECRET_SYMBOL   '*'

// Mapping of all keys on keyboard, row by row
const byte ROWS = 4;
const byte COLS = 4;
const char ALL_KEYS[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Characters for secret code, in order
const char SECRET_KEYS[MAX_CODE_LENGTH] = {'2', '0', '2', '5'};

const byte rowPins[ROWS] = {9, 8, 7, 6}; // Pins corresponding to rows on keyboard
const byte colPins[COLS] = {5, 4, 3, 2}; // Pins corresponding to columns on keyboard

Keypad keypad = Keypad( makeKeymap(ALL_KEYS), rowPins, colPins, ROWS, COLS );
unsigned int keysLeft = MAX_CODE_LENGTH;  // How many keys are left to type
char keys[MAX_CODE_LENGTH * 2];           // What characters have been typed, in order (with extra safety buffer size)

LiquidCrystal_I2C lcd(0x27,LCD_COLS,LCD_ROWS);
bool displayedEnterCodeText = false;  // Whether initial message has been displayed yet

void setup() {
  Serial.begin(BAUD_RATE);
  
  LOG_DEBUG("SETUP", "=== Access Control System ===");

  // Keypad
  LOG_DEBUG("SETUP", "Keypad [✔]");

  // LEDs
  setupLEDs();
  turnAllLEDs(OFF);
  LOG_DEBUG("SETUP", "LEDs [✔]");
  
  // LCD
  setupLCD();
  displayLines("Getting ready...", "Just a minute!");
  LOG_DEBUG("SETUP", "LCD screen [✔]");

  LOG_DEBUG("SETUP", "All components operational and ready.");

  delay(3000);
  lcd.clear();
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
      LOG_DEBUG("KEYPAD", "Pressed '" + String(key) + "'");
      displayLines(ENTER_CODE_TEXT, getAsSecret(keysLeft));
      delay(MILLIS_BETWEEN_KEY_PRESS);
    }
  } else {
    LOG_DEBUG("KEYPAD", keys);

    bool isValidCode = strncmp(keys, SECRET_KEYS, sizeof(keys)) == 0;
    if (isValidCode) {
      turnOnOneLED(true);
      displayLines("ACCESS GRANTED", "Welcome!");
      LOG_INFO("ACCESS CONTROL", "Access granted");
    } else {
      turnOnOneLED(false);
      displayLines("ACCESS DENIED", "Please try again.");
      LOG_INFO("ACCESS CONTROL", "Access denied");
    }
    
    resetKeys();
    delay(MILLIS_AFTER_CODE_CHECK);
    
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

String truncate(String text, const int maxLength) {
  const String ellipsis = "...";
  return text.length() > maxLength ?
    text.substring(0, maxLength - ellipsis.length()) + ellipsis : text;
}

String getAsSecret(int keysLeft) {
  String hidden = "";
  for (int i = 0; i < MAX_CODE_LENGTH - keysLeft; i++) {
    hidden += SECRET_SYMBOL;
  }
  return hidden;
}

void displayLines(String first, String second) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(truncate(first, LCD_COLS));
  lcd.setCursor(0, 1);
  lcd.print(truncate(second, LCD_COLS));
}

void turnOnOneLED(bool isGreen) {
  digitalWrite(G_LED_PIN, isGreen ? HIGH : LOW);
  digitalWrite(R_LED_PIN, isGreen ? LOW : HIGH);
}

void turnAllLEDs(bool on) {
  digitalWrite(G_LED_PIN, on ? HIGH : LOW);
  digitalWrite(R_LED_PIN, on ? HIGH : LOW);
}