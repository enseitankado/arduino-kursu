
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "dht11.h"

// Cihazinizi I2C veriyoluna baglayin
LiquidCrystal_I2C lcd1(0x27, 16, 2);

int dht_pin = 7;
dht11 DHT11;

void setup()
{
  lcd1.begin();
  lcd1.backlight();
  lcd1.blink();  
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  
  // Diğer metodlar: backlight,noBacklight,clear,display,noDisplay,
  // blink, noBilink, cursor, noCursor (Dahasi LCD.h icinde)
}

void loop()
{
  DHT11.read(dht_pin);
  int sicaklik = DHT11.temperature;
  int nem = DHT11.humidity;   
  
  lcd1.setCursor(0, 0);
  lcd1.print("Sicaklik:" + String(sicaklik));
  lcd1.setCursor(0, 1);
  lcd1.print("Nem:" + String(nem));

  if (sicaklik > 28) {
    digitalWrite(13, 1);
    digitalWrite(10, 0);
  } else { 
    digitalWrite(13, 0);
    digitalWrite(10, 1);
  }

  delay(500);
}
