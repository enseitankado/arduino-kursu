#include "dht11.h"
int dht_pin = 7;
dht11 DHT11sensor;

void setup() {
  Serial.begin(9600);
}

void loop() {
  DHT11sensor.read(dht_pin);
  int sicaklik = DHT11sensor.temperature;
  int nem = DHT11sensor.humidity;   
  Serial.print("Sicaklik: ");
  Serial.print(sicaklik);
  Serial.print(" Nem: ");
  Serial.println(nem);
  delay (250);
}
