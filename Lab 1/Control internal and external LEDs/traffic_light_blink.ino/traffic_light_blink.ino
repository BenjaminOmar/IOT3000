/*
  [Arduino] https://www.arduino.cc/en/Tutorial/BuiltInExamples/
  [Tinkercad] https://www.tinkercad.com/things/iUQuEFFfV6M-multiple-leds-breadboard-blocks
*/


// definerer pins som LED'erne er knyttet til
#define RED_LED_PIN 12
#define YELLOW_LED_PIN 11
#define GREEN_LED_PIN 10

// the setup function runs once when you press reset or power the board
void setup() {
  // initialiserer pins som output
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(RED_LED_PIN, HIGH);    // Skrur på rød LED ved å sette spenning til HIGH
  delay(1000);                        // venter 1 sekund
  
  digitalWrite(YELLOW_LED_PIN, HIGH); // Skrur på gul LED ved å sette spenning til HIGH
  delay(1000);                       // venter 1 sekund
  
  digitalWrite(YELLOW_LED_PIN, LOW);  // Skrur av gul LED ved å sette spenning til LOW
  digitalWrite(RED_LED_PIN, LOW);     // Skrur av rød LED ved å sette spenning til LOW samtidig
  digitalWrite(GREEN_LED_PIN, HIGH);  // Skrur på grønn LED ved å sette spenning til HIGH
  delay(1000);                       // venter 1 sekund
  
  digitalWrite(GREEN_LED_PIN, LOW);  // Skrur av grønn LED ved å sette spenning til LOW
  delay(1000);                      // venter 1 sekund
}
