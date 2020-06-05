#include <SoftwareSerial.h>
/*
   Baglanti semasi:
   Bluetooth  Arduino
   ------------------
   RX     =>  3 (Tx)
   TX     =>  2 (Rx)
*/
SoftwareSerial bt(2, 3); //Rx, Tx

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
  bluetooth.begin(9600);
  bluetooth.println("Program basladi.");
  Serial.begin(9600);
  Serial.println("Program basladi.");
}

void loop()
{
  if (bluetooth.available()) {

    char komut = bluetooth.read();

    if ('S' == komut) {
      digitalWrite(MAA, 0);
      digitalWrite(MAK, 0);
      digitalWrite(MBA, 0);
      digitalWrite(MBK, 0);
    }

    if ('F' == komut) {
      digitalWrite(MAA, 1);
      digitalWrite(MAK, 0);
      digitalWrite(MBA, 1);
      digitalWrite(MBK, 0);
    }

    if ('B' == komut) {
      digitalWrite(MAA, 0);
      digitalWrite(MAK, 1);
      digitalWrite(MBA, 0);
      digitalWrite(MBK, 1);
    }

    if ('R' == komut) {
      digitalWrite(MAA, 1);
      digitalWrite(MAK, 0);
      digitalWrite(MBA, 0);
      digitalWrite(MBK, 1);
    }

    if ('L' == komut) {
      digitalWrite(MAA, 0);
      digitalWrite(MAK, 1);
      digitalWrite(MBA, 1);
      digitalWrite(MBK, 0);
    }    

  }
}
