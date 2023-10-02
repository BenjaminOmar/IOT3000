// This sketch has been modified based on the original sketch
// written by Ken Shirriff
#include <IRremote.h>

#define RECV_PIN  11 //Pin for IR-receiver
#define LED_PIN   2
#define HEX_CODE_PLUS_BTN   0xAE629AD3
#define HEX_CODE_MINUS_BTN  0x85B144C2

IRrecv irrecv(RECV_PIN); // Create IRrecv object
decode_results results;  // Create object to store results

void setup()
{
  irrecv.enableIRIn();   // Start the receiver
  Serial.begin(9600);    // Start serial communications
  delay(2000);
  Serial.println("Infrared Receiver Started...");
  pinMode(LED_PIN, OUTPUT);     // Connect LED to GPIO2
  digitalWrite(LED_PIN, LOW);  // Turn LED of from beginning
}

void loop()
{
  if (irrecv.decode(&results))
  {
    Serial.println(results.value, HEX);
    if (results.value == HEX_CODE_PLUS_BTN) {digitalWrite(LED_PIN, HIGH);}
    if (results.value == HEX_CODE_MINUS_BTN) {digitalWrite(LED_PIN, LOW);}
    irrecv.resume(); // Receive the next value
  }
}
