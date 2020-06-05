#include <SPI.h>
#include <MFRC522.h>
MFRC522 kartOkuyucu(10, 9); //SS_PIN, RST_PIN

String sahipID;
int i;

void setup() {
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  kartOkuyucu.PCD_Init();
  delay(300);
  Serial.println("RFID Kart uygulamasi basladi, bir kart okutun.");
  Serial.println("----------------------------------------------");
  delay(500);
}

void loop()
{
  // Yeni kart okuma oturumu basladi mi?
  if (kartOkuyucu.PICC_IsNewCardPresent()){

    // Karti oku
    kartOkuyucu.PICC_ReadCardSerial(); 
    //Serial.print("\nKart ID'si: ");

    // Okunan uid'in byte'larını konsola yaz ve 
    // sahipID String'inin icinde biriktir 
    sahipID = "";
    for (int i = 0; i < kartOkuyucu.uid.size; i++) {
      //Serial.print(kartOkuyucu.uid.uidByte[i]);
      sahipID = sahipID + String(kartOkuyucu.uid.uidByte[i]);
    }

    if (sahipID == String("20884135167")) {  
      Serial.println(" Girise izin verildi.");    
      digitalWrite(7, HIGH);
      tone(3, 3000, 500);      
      delay(500);
      digitalWrite(7, LOW);
    } else {
      Serial.println(" Yetkisiz kart.");
      tone(3, 1500, 800);
      delay(1000);
      tone(3, 1500, 800);
    }
    
    // Oturumunu sonlandir
    kartOkuyucu.PICC_HaltA(); 
  }
}
