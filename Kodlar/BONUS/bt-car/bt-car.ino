#include <SoftwareSerial.h>
/*
   Baglanti semasi:
   Bluetooth  Arduino
   ------------------
   RX     =>  11 (Tx)
   TX     =>  10 (Rx)
*/
SoftwareSerial maviDis(3, 2);  // RX,TX

const byte MAA = 8; //L298 IN1
const byte MAK = 9; //L298 IN2
const byte MBA = 10; //L298 IN3
const byte MBK = 11; //L298 IN4

void setup()
{
  pinMode(MAA, OUTPUT);
  pinMode(MAK, OUTPUT);
  pinMode(MBA, OUTPUT);
  pinMode(MBK, OUTPUT);
  maviDis.begin(9600);
  maviDis.println("Program basladi.");
  Serial.begin(9600);
  Serial.println("Program basladi.");
}

void loop()
{
  if (maviDis.available()) {

    char komut = maviDis.read();
    if ('F' == komut)
    {
      digitalWrite(MAA, HIGH);
      digitalWrite(MAK, LOW);
      digitalWrite(MBA, LOW);
      digitalWrite(MBK, HIGH);
    }

    if ('B' == komut)
    {
      digitalWrite(MAA, LOW);
      digitalWrite(MAK, HIGH);
      digitalWrite(MBA, HIGH);
      digitalWrite(MBK, LOW);
    }

    if ('L' == komut)
    {
      digitalWrite(MAA, LOW);
      digitalWrite(MAK, HIGH);
      digitalWrite(MBA, LOW);
      digitalWrite(MBK, HIGH);
    }

    if ('R' == komut)
    {
      digitalWrite(MAA, HIGH);
      digitalWrite(MAK, LOW);
      digitalWrite(MBA, HIGH);
      digitalWrite(MBK, LOW);
    }

    if ('S' == komut)
    {
      digitalWrite(MAA, LOW);
      digitalWrite(MAK, LOW);
      digitalWrite(MBA, LOW);
      digitalWrite(MBK, LOW);
    }

  }
}
