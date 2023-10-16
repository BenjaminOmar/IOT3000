#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define R_LED_PIN 12
#define G_LED_PIN 13

#define MAX_CODE_LENGTH 4

#ifndef STASSID
#define STASSID "Ellens iPhone (2)"
#define STAPSK "abcd1234"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

String s;  // Web page content
ESP8266WebServer server(80);

// See https://lastminuteengineers.com/arduino-keypad-tutorial/

const byte ROWS = 4;
const byte COLS = 4;

char allKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
const char SECRET_CODE[MAX_CODE_LENGTH] = {'1', '2', '3', '4'};

byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

//Create an object of keypad
Keypad keypad = Keypad( makeKeymap(allKeys), rowPins, colPins, ROWS, COLS );

//Create lcd object
LiquidCrystal_I2C lcd(0x27,16,2); //address, cols, rows

int keysLeft = MAX_CODE_LENGTH;
char keys[MAX_CODE_LENGTH];

void setup() {
  // LEDs
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);

  digitalWrite(G_LED_PIN, LOW);
  digitalWrite(R_LED_PIN, LOW);

  lcd.init(); // initialize the lcd
  lcd.home();
  lcd.backlight();
  lcd.setCursor(0, 0); //x,y
  lcd.print("Getting ready...");
  lcd.setCursor(0, 1); //x,y
  lcd.print("Just a minute");

  // Prepare GPIO4 (LED)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 0);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  s = "<!DOCTYPE html>";
  s += "<html>";
  s += "<head>";
  s += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  s += "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN' crossorigin='anonymou'>";
  s += "</head>";
  s += "<body>";
  s += "<div class='container'>";
  s += "<h1>LED</h1>";
  s += "<div class='row g-2 mb-3'>";
  s += "<div class='col-12 col-md-6 col-lg-4'>";
  s += "<a class='btn btn-primary w-100' href='/LedOn'>ON</a>";
  s += "</div>";
  s += "<div class='col-12 col-md-6 col-lg-4'>";
  s += "<a class='btn btn-danger w-100' href='/LedOff'>OFF</a>";
  s += "</div>";
  s += "</div>";
  s += "<h1>Relay</h1>";
  s += "<div class='row g-2'>";
  s += "<div class='col-12 col-md-6 col-lg-4'>";
  s += "<a class='btn btn-primary w-100' href='/RelayOn'>ON</a>";
  s += "</div>";
  s += "<div class='col-12 col-md-6 col-lg-4'>";
  s += "<a class='btn btn-danger w-100' href='/RelayOff'>OFF</a>";
  s += "</div>";
  s += "</div>";
  s += "</div>";
  s += "</body>";
  s += "</html>";

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  // Handle turning the LED on / off
  server.on("/granted", handleGranted);
  server.on("/denied", handleDenied);

  server.begin();
  Serial.println("HTTP server started");

  Serial.begin(9600);

  delay(3000);
}

void loop() {
  server.handleClient();
  lcd.clear();

  if (keysLeft > 0) {
    digitalWrite(R_LED_PIN, LOW);
    digitalWrite(G_LED_PIN, LOW);

    char key = keypad.getKey();// Read the key
    if (key) {
      keys[MAX_CODE_LENGTH - keysLeft] = key;
      keysLeft--;

      Serial.println("Pressed: " + String(key));

      lcd.setCursor(0, 0);
      lcd.print("Enter code");
      lcd.setCursor(0, 1);
      for(int i = 0; i < MAX_CODE_LENGTH - keysLeft; i++) lcd.print("*");

      delay(1000);
    }
  } else {
    Serial.println("You entered: " + String(keys));
    Serial.print("'");
    for (int i = 0; i < MAX_CODE_LENGTH; i++) Serial.print(keys[i]);
    Serial.print("'");
    Serial.println();

    if (strncmp(keys, SECRET_CODE, sizeof(keys)) == 0) {
      digitalWrite(G_LED_PIN, HIGH);
      digitalWrite(R_LED_PIN, LOW);

      lcd.setCursor(0, 0);
      lcd.print("Access granted");
      lcd.setCursor(0, 1);
      lcd.print("Welcome back!");

      server.sendHeader("Location", "/");
      server.send(303);
    } else {
      digitalWrite(G_LED_PIN, LOW);
      digitalWrite(R_LED_PIN, HIGH);
      
      lcd.setCursor(0, 0);
      lcd.print("Access denied");
      lcd.setCursor(0, 1);
      lcd.print("Please try again");
    }

    for (int i = 0; i < MAX_CODE_LENGTH; i++) keys[i] = 0;
    keysLeft = MAX_CODE_LENGTH;

    delay(5000);
  }
}

