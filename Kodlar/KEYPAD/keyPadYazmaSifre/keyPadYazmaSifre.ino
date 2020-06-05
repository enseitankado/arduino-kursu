#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
int kontrol=0;
int sayac=0;
String girilenSifre;      //Girilen şifre
String sifre="#1234";      //Belirlediğimiz şifre

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
 

void setup(){
    Serial.begin(9600);
}

void loop(){
  char basilanTus = customKeypad.getKey();
  if (basilanTus)
  {
    Serial.println(basilanTus);
    // # tuşuna basıldı ise control 1 olur password kontrole geçer
    if(basilanTus == '#') 
    {  
      kontrol = 1;
    }
        // # tuşuna basıldı ise gerekli kontrolleri yap
        if(kontrol == 1){
            girilenSifre = girilenSifre + basilanTus;
            sayac++;
            Serial.print("sifre " + girilenSifre );
                // password doğru ise
                if(girilenSifre == sifre) {
                    Serial.print("kapı açıldı"); 
                    girilenSifre = "";
                    kontrol = 0;
                    sayac = 0;
                }
                // password yanlış veya 4 den fazla girildi ise
                else if ((girilenSifre != sifre) && (sayac > 4) ) {
                    Serial.print("hatalı giriş");
                    girilenSifre = "";
                    kontrol = 0;
                    sayac = 0;
                }
        }
  }
}
