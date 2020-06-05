#include "dht11.h"
/*
 * Program DHT11 sensorunu okuyarak sicaklik ve nem (bagil)
 * degerlerini seri port ekranina yazmaktadir. 
 * 
 * 
 * Baglanti semasi
 * DHT11   ARDUINO
 * ---------------
 * Vcc     5v
 * Gnd     Gnd
 * Data    7
*/
int dht_pin = 7;
dht11 DHT11;

void setup() {
  Serial.begin(9600);
}

void loop() {
  DHT11.read(dht_pin);
  int sicaklik = DHT11.temperature;
  int nem = DHT11.humidity;   
  Serial.print("Sicaklik: ");
  Serial.print(sicaklik);
  Serial.print(" Nem: ");
  Serial.println(nem);
  delay (250);
}
