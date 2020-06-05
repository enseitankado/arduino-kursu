#include <SPI.h>
#include <MFRC522.h>
MFRC522 kartOkuyucu(10, 9); //SS_PIN, RST_PIN

void setup() {
  Serial.begin(9600);
  SPI.begin();
  kartOkuyucu.PCD_Init();
  Serial.println("RFID Kart uygulamasi basladi, bir kart okutun.");
  Serial.println("----------------------------------------------");
}

void loop()
{
  // Yeni kart okuma oturumu basladi mi?
  if (kartOkuyucu.PICC_IsNewCardPresent()){

    // Karti oku
    kartOkuyucu.PICC_ReadCardSerial(); 
    Serial.print("\nKart ID'si: ");

    // Okunan uid'in byte'larını yaz
    for (int i = 0; i < kartOkuyucu.uid.size; i++)
      Serial.print(kartOkuyucu.uid.uidByte[i]);
    
    // Yeni kart iicin oturumunu sonlandir
    kartOkuyucu.PICC_HaltA(); 
  }
}

