
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
              
char satir[]="Samsun Akademi                  ";
int sure = 100; 

void setup()
{
  lcd.begin(16, 2);
  lcd.backlight();
}

void loop()
{
  lcd.setCursor(15, 0);
  for (int sutun = 0; sutun < 32; sutun++)
  {
    lcd.scrollDisplayLeft();
    lcd.print(satir[sutun]); 
    delay(sure);
  }
  
  lcd.clear();   
}


