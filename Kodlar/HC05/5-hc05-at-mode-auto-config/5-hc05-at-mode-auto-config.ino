/*
	Bu program HC05 modulunu otomatik olarak AT moda alarak su sekilde yapilandirir:
	Programin kendisi AT moda otomatik gecis yaptigi icin, key pin baglantisi 
	mutlaka yapilmalidir.
	
	  writeConfig("AT+NAME=tankado");
	  writeConfig("AT+PSWD=\"1881\"");
	  writeConfig("AT+UART=9600,0,0");
	  writeConfig("AT+ROLE=0");
	  writeConfig("AT+CMODE=0");
	  writeConfig("AT+CLASS=1");

	Devre bağlantı şeması şu şekildedir:
	Bluetooth       Arduino
	------------------
	RX          =>  11 (Tx)
	TX          =>  10 (Rx)
    Vcc         =>  6 ve 7
    Key (pin34) =>  2 (Modulun key pini yoksa Pin34'e [PIO11] bir kablo lehimleyin)

	Subat 2020, Özgür Koca
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
  Serial.begin(9600);
  Serial.println("HC05 AT moduna geciriliyor...");
  
  pinMode(vcc1, OUTPUT);
  pinMode(vcc2, OUTPUT);
  pinMode(pin34, OUTPUT);

  // Enerji vermeden once key pini Low(0) yapilir
  digitalWrite(vcc1, LOW);

  // Modulun enerjisini kes
  digitalWrite(vcc2, LOW);
  digitalWrite(pin34, LOW);
  
  BTSerial.begin(38400);

  // Module enerji ver
  digitalWrite(vcc1, HIGH);
  digitalWrite(vcc2, HIGH);

  // Enerji verdikten sonra key pini High(1) yapilir
  digitalWrite(pin34, HIGH);
  
  // Modulun AT moda gecmesi icin zaman tani
  delay(1000);
 
  Serial.println("AT moda gecildi. Yapilandirma uygulaniyor...");
  writeConfig("AT+ORGL");
  writeConfig("AT+NAME=TankadoCom");
  writeConfig("AT+PSWD=\"1881\"");
  writeConfig("AT+UART=9600,0,0");
  writeConfig("AT+ROLE=0");
  writeConfig("AT+CMODE=0");
  writeConfig("AT+CLASS=1");

  digitalWrite(pin34, LOW);
  digitalWrite(vcc1, LOW);
  digitalWrite(vcc2, LOW);
  delay(50);
  digitalWrite(vcc1, HIGH);
  digitalWrite(vcc2, HIGH);
  Serial.println("\r\nHC05 iletisim moduna alindi.");
}

void writeConfig(String atCommand) {
  Serial.print( String(">" + atCommand + "\r\n") );
  BTSerial.print( String(atCommand + "\r\n") );
  delay(100);
  Serial.print(BTSerial.readString());
}

void loop()
{

}
