#include <Wire.h>
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);
  Serial.println("I2C veriyolu tarayicisi basladi\n");
}

void loop()
{
  byte hata, adr;
  int bulunanAygitSayisi = 0;

  Serial.println("Taraniyor...");
  for (adr = 1; adr < 127; adr++ )
  {
    Wire.beginTransmission(adr);
    hata = Wire.endTransmission();
    if (hata == 0)
    {
      Serial.print("Veriyolunda I2C aygiti bulundu, addresi: 0x");
      if (adr < 16)
        Serial.print("0");
      Serial.print(adr, HEX);
      Serial.println("  !");

      bulunanAygitSayisi++;
    }
  }

  if (bulunanAygitSayisi == 0)
    Serial.println("Hic I2C aygiti bulunamadi.\n");
  else
    Serial.println("Bitti.\n");

  delay(5000);
}
