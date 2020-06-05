#include <SoftwareSerial.h>
/*
 * Baglanti semasi:
 * Bluetooth  Arduino
 * ------------------
 * RX     =>  11 (Tx)
 * TX     =>  10 (Rx)
 */

SoftwareSerial BTSerial(10, 11); //Rx, Tx

void setup() 
{
  Serial.begin(9600);  
  BTSerial.begin(9600);
  
  Serial.println("PC-Serial <-> Bluetoth-Serial cift yonlu dinlemeye alindi.");
  Serial.println("\nSerial gondermeye hazir.\n");
}

void loop()
{  
  if (BTSerial.available())    
    Serial.write(BTSerial.read());
  
  if (Serial.available())
    BTSerial.write(Serial.read());
}
