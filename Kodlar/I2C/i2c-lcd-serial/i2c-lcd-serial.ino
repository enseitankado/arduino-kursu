#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup()
{
  lcd.begin (16, 2);
  Serial.begin(9600);
}

void loop()
{  
  if (Serial.available())
  {
    delay(100);
    lcd.clear();    
    while (Serial.available() > 0)
    {      
      lcd.write(Serial.read());
    }
  }
}

