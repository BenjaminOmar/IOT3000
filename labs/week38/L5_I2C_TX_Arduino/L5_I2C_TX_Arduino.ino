
#include <Wire.h>
#include <DHT.h>
#define dhtpin 2
#define dhttype DHT11
DHT dht(dhtpin,dhttype);
String s = "";

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("Temperature sensor");
dht.begin();
Wire.begin(0x10);
Wire.onReceive(receiveEvent);
Wire.onRequest(requestEvent);
}

void receiveEvent(int n){
  while (0 < Wire.available()){
    char c = Wire.read();
    Serial.print(c); 
    }
    Serial.println();
  }

void requestEvent() {
  char s1[12];
  s.toCharArray(s1,12);
  Wire.write(s1);
  }

void loop() {
  //update every 5 second 
  delay(5000);
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  s = String(t) +  " " + String(h);;
  Serial.println(s);
}
