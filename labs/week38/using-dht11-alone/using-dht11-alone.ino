#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    Serial.println("Starting Temperature/Humidity Monitor.");
  }
  dht.begin();
  delay(3000);
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(t)) {
    Serial.println("Reading from DHT sensor failed");
    return;
  }
  Serial.println("Temperature: " + String(t) + " *C");
  Serial.println("Rel. humidity: " + String(h) + "%");
  delay(2000);
}
