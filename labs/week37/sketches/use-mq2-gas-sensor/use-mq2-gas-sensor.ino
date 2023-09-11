int smokePinD = 2;
int buzzerPin = 8;
int LEDPin = 4;

void setup() {
  Serial.begin(19200);
  pinMode(smokePinD, INPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  delay(5000);
}

void loop() {
  if (digitalRead(smokePinD) == HIGH) {
    Serial.println("Warning, value high");
    digitalWrite(LEDPin, HIGH);
    tone(buzzerPin, 880);
  } else {
    Serial.println("Normal values");
    digitalWrite(LEDPin, LOW);
    noTone(buzzerPin);
  }
  delay(5000);
}
