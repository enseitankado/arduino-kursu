#include <SoftwareSerial.h>
/*
   Baglanti semasi:
   Bluetooth  Arduino
   ------------------
   RX     =>  11
   TX     =>  10
*/
SoftwareSerial maviDis(10, 11);

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  maviDis.begin(9600);
  Serial.println("Program basladi.");
}


void loop()
{
  if (maviDis.available()) {

    char komut = maviDis.read();
    if ('A' == komut)
    {
		digitalWrite(13, HIGH);
		Serial.println("Led yandi.");
    }

    if ('K' == komut)
    {
		digitalWrite(13, LOW);
		Serial.println("Led sondu.");
    }
	
  }
}

