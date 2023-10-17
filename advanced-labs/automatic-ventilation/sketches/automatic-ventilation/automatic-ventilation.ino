#include <Stepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN    2
#define DHTTYPE   DHT11
#define MAX_TEMP  25

#define MOTOR_PIN 5
#define IN1 11
#define IN2 10
#define IN3 9
#define IN4 8
#define STEPS_PER_REVOLUTION 2038
#define MAX_SPEED 15

#define R_LED_PIN 4
#define G_LED_PIN 3

//Create lcd object
LiquidCrystal_I2C lcd(0x27,16,2); //address, cols, rows
//Create dht object
DHT dht(DHTPIN, DHTTYPE); // create the sensor object
// Create stepper object
Stepper stepper = Stepper(STEPS_PER_REVOLUTION, IN1, IN3, IN2, IN4);

int steps, motor_speed;
 
void setup(){
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);

  pinMode(MOTOR_PIN, 5);
  stepper.setSpeed(MAX_SPEED);

  lcd.init(); // initialize the lcd
  lcd.home();
  lcd.backlight();
  lcd.setCursor(0, 0); //x,y
  lcd.print("Getting ready...");
  lcd.setCursor(0, 1); //x,y
  lcd.print("Just a minute");
  
  dht.begin();
  
  Serial.begin(9600);

  delay(3000);
}

void loop(){
  float t = dht.readTemperature(); // read temperature
  Serial.println(t);
  float h = dht.readHumidity();    // read relative humidity
  // check if temperature was read properly
  lcd.clear();
  if (isnan(t)){
    lcd.println("Reading failed");
    return;}

  if (t > MAX_TEMP) {
    steps = STEPS_PER_REVOLUTION;

    // Turn on RED light only
    digitalWrite(R_LED_PIN, HIGH);
    digitalWrite(G_LED_PIN, LOW);

    // print the temperature/humidity in the Serial Monitor
    lcd.setCursor(0, 0); //x,y
    lcd.print("WARNING!");
    lcd.setCursor(0, 1); //x,y
    lcd.print("Temp.  " + String(t) + "C");
  } else {
    steps = 0;

    // Turn on GREEN light only
    digitalWrite(R_LED_PIN, LOW);
    digitalWrite(G_LED_PIN, HIGH);

    // print the temperature/humidity in the Serial Monitor
    lcd.setCursor(0, 0); //x,y
    lcd.print("Temp. " + String(t) + "C");
  }

  stepper.step(steps);
  if (t < MAX_TEMP) delay(1500);
}
