
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Cihazinizi I2C veriyoluna baglayin
LiquidCrystal_I2C lcd1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup()
{
  lcd1.begin(16, 2);
  lcd1.backlight();
  lcd1.blink();  
  
  // Diğer metodlar: backlight,noBacklight,clear,display,noDisplay,
  // blink, noBilink, cursor, noCursor (Dahasi LCD.h icinde)
}

void loop()
{
  lcd1.setCursor(0, 0);
  lcd1.print("Samsun Akademi");
  lcd1.setCursor(0, 1);
  lcd1.print("Arduino Ileri");

  delay(100);
}
