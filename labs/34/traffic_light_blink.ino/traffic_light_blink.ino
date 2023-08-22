/*
  [Arduino] https://www.arduino.cc/en/Tutorial/BuiltInExamples/
  [Tinkercad] https://www.tinkercad.com/things/iUQuEFFfV6M-multiple-leds-breadboard-blocks
*/

#define RED_LED_PIN 12
#define YELLOW_LED_PIN 11
#define GREEN_LED_PIN 10

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize each digital pin as an output.
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(RED_LED_PIN, HIGH);    // turn the RED LED on (HIGH is the voltage level)
  delay(1000);                        // wait for a second
  
  digitalWrite(YELLOW_LED_PIN, HIGH);
  delay(1000);
  
  digitalWrite(YELLOW_LED_PIN, LOW);  // turn both RED and YELLOW LED off by making the voltage LOW
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(1000);
  
  digitalWrite(GREEN_LED_PIN, LOW);
  delay(1000);
}
