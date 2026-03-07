
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Cihazinizi I2C veriyoluna baglayin
LiquidCrystal_I2C lcd1(0x27, 16, 2);

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
  lcd1.setCursor(0, 0);
  lcd1.print("Tankado.com");
  lcd1.setCursor(0, 1);
  lcd1.print("Raspberry Pi");

  delay(100);
}
