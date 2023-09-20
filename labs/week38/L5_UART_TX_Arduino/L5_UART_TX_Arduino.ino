#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); // create the sensor object

String s = "";

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(10000);
}

void loop() {
  float t = dht.readTemperature(); // read temperature
  float h = dht.readHumidity();    // read relative humidity
  // check if temperature was read properly
  if (isnan(t)){
    Serial.println("Reading from DHT sensor failed");
    return;}
  // print the temperature/humidity in the Serial Monitor
  Serial.println("Temp. " + String(t) + " *C  " + "Rel. hum. " + String(h) + "%");
  delay(2000); // Update every second second
}
