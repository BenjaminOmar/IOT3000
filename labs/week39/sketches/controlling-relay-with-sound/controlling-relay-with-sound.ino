#define MIC_PIN 2
#define RELAY_PIN 4 

int relay_state = LOW;
int has_detected = false;
  
void setup() {
  Serial.begin(9600);
  delay(3000);
  pinMode(MIC_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  Serial.print("Initializing Sound Sensor");
}
 
void loop() {
  if (has_detected){
    if (relay_state==HIGH){relay_state=LOW;}else{relay_state=HIGH;}
    digitalWrite(RELAY_PIN, relay_state);
    Serial.println("Switched relay state - now on " + String((relay_state == HIGH) ? "HIGH" : "LOW"));
    has_detected = false;
    delay(1000);
  } 
  if (digitalRead(MIC_PIN) == LOW) {
    Serial.println("Sound detected");
    has_detected = true;
    delay(20);     
  };
}
