/*
     Arduino Uno:        1kb EEPROM.
     Arduino Mega:       4kb EEPROM.
*/
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  Serial.println("EEPROM'un kapasitesi " + String( EEPROM.length() ));
  Serial.println("Bir string girin:");

  while (Serial.available() == 0);      // Metin girilinceye kadar bekle
  String metin = Serial.readString();   // Metni string olarak oku

  Serial.println("Girilen metnin uzunlugu: " + String(metin.length()));
  for (int i=0; i<metin.length(); i++)  // Metnin tum byte'larini EEPROM'a yaz
    EEPROM[i] = metin[i];
}

void loop() {

  // EEPROM'un ilk 10 byte'ini yaz
  for (int i=0; i<10; i++) 
    Serial.print(char(EEPROM[i]));

  Serial.println(""); // Alt satıra gec
  delay(1000);
}
