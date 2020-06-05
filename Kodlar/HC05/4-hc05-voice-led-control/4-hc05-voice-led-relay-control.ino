#include <SoftwareSerial.h>
/*
 * Baglanti semasi:
 * Bluetooth  Arduino
 * ------------------
 * RX     =>  11
 * TX     =>  10
 * 
 * Kullanılan uygulama: Arduino Voice Control
 */
SoftwareSerial BTSerial(10, 11);
const int LED1 = 7;

void setup()
{
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  BTSerial.begin(9600);
  Serial.begin(9600);
  Serial.println("Program basladi.");
}

void loop()
{
  if (BTSerial.available()) {

    String komut = BTSerial.readString();
    Serial.println("Gelen komut: " + komut);
    
    if (String("kapat") == komut)
    {
      digitalWrite(LED1, LOW);
      Serial.println("Işıklar kapatıldı.");
    }

    if (String("aç") == komut)
    {
      digitalWrite(LED1, HIGH);
      Serial.println("Işıklar açıldı.");
    }
  }
}
