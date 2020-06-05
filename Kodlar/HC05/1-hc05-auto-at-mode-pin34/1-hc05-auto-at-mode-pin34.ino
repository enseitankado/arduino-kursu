/*

	Bu program HC05 modulunun iletisim hizini sinar.

	Devre şeması şu şekildedir:

	Bluetooth   Arduino
	------------------
	RX          =>  11 (Tx)
	TX          =>  10 (Rx)
  Vcc         =>  6+7
  Key (pin34) =>  2 (Modulun key pini yoksa Pin34'e [PIO11] bir kablo lehimleyin)


	Temmuz 2018
	Özgür Koca
*/

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11);
#define vcc1  6  // Supply ~40mA
#define vcc2  7  // Supply ~40mA
#define pin34  2 // HC05's key pin to switch AT mode

char c = ' ';
boolean YeniSatir = true;

void setup()
{
  pinMode(vcc1, OUTPUT);
  pinMode(vcc2, OUTPUT);
  pinMode(pin34, OUTPUT);
  
  digitalWrite(vcc1, LOW);
  digitalWrite(vcc2, LOW);
  digitalWrite(pin34, LOW);
  
  Serial.begin(9600);
  BTSerial.begin(38400);
  Serial.println("Dikkat: Satir sonunu NL+CR ayarlamayi unutmayin.");

  digitalWrite(pin34, HIGH);
  digitalWrite(vcc1, HIGH);
  digitalWrite(vcc2, HIGH);
  Serial.println("HC05 AT moduna gecirildi.\r\n");
  delay(1000);
  BTSerial.print("AT+NAME?");
}

void loop()
{
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  if (Serial.available()) {

    c = Serial.read();
    BTSerial.write(c);

    // AT komutunu seri konsola yazar.
    // '>' kullanici komutu oldugunu belirtir.
    if (YeniSatir) {
      Serial.print(">");
      YeniSatir = false;
    }

    Serial.write(c);
    // CR gelirse yeni satira basla
    if (c == 10) YeniSatir = true;
  }
}
