#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define Adres 0x3F

const byte ROWS = 4;
const byte COLS = 4;
String yazi="";
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(Adres, 2, 1, 0, 4, 5, 6, 7, 3, 0);  

void setup(){
  lcd.begin(16,2);   
  lcd.backlight();
 
}

void loop(){
  char customKey = customKeypad.getKey();
  if (customKey){
    yazi=yazi+customKey;
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print(yazi);
  }
}


