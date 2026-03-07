
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Cihazinizi I2C veriyoluna baglayin
LiquidCrystal_I2C lcd1(0x27, 16, 2);

#include "dht11.h"
int dht_pin = 7;
dht11 DHT11sensor;

void setup()
{
  lcd1.begin();
  lcd1.backlight();
  lcd1.blink();  
  
  // Diğer metodlar: backlight,noBacklight,clear,display,noDisplay,
  // blink, noBilink, cursor, noCursor (Dahasi LCD.h icinde)
}

void loop()
{

  DHT11sensor.read(dht_pin);
  int sicaklik = DHT11sensor.temperature;
  int nem = DHT11sensor.humidity;  
  
  lcd1.setCursor(0, 0);
  lcd1.print("Sicaklik: " + String(sicaklik));
  lcd1.setCursor(0, 1);
  lcd1.print("Nem: " + String(nem));

  delay(100);
}
