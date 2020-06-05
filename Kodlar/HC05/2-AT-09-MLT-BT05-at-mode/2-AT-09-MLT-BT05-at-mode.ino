/*

	AT-09 (MLT-BT05-V4.4) Komut Modu Yapılandırma Programı

	Bu program AT-09 (HM-10 uyumlu) AT komutlarını
  denemek icin yazilmistir.

	Agustor 2018
	Özgür Koca

  Devre şeması şu şekildedir:

  Bluetooth  Arduino
  ------------------
  RX     =>  11 (Tx)
  TX     =>  10 (Rx)
*/

/*  AT-09 modulu (breakout board), yavru kart olarak Bolutek firması tarafından uretilen CC41A modulunu kullanir, 
    bu modul de TI (Texas Instruments)'ın CC2541 cipini kullanir. Modulun belkemigi
    CC2541'dir. Bolutek: http://www.bolutek.com/ProductController/findDetail.do?id=47
    
    MLT-BT05 adıyla bilinen kart da aslında AT-09'dur. HM-10'da benzer kart tasarimina sahip 
    olmasina karsin firmware'i farklidir, dokumantasyonu ve kullanici kitlesi daha
    fazladir ve iBeacon tanımlanabilir. 
    
    MLT-05'e komut kumesi daha genis ve gelismis olan HM-10'nun firmware'i 
    yazilabilir (https://forum.arduino.cc/index.php?topic=393655)
    
    CC41A Pins
    ----------
    Pin 23 (PIO0):  System Key
    Pin 24 (PIO1):  System LED. 
                    1) Uyku modunda sonuk.
                    2) Bagliyken surekli yanik.
                    3) Broadcast modunda iken 0.5sn araliklarla yanip soner.
    
    AT-09 bluetooth 4.0 ve BLE (Bluetooth Low Energy) standartlarini destekler. 
    HC-05'in aksine IOS cihazlar tarafindan gorulebilir. CC41A, Uyku modunda 400uA~1.5mA,
    etkin durumdayken 8.5mA akım tuketir. HC-05'den farkli olarak AT komutlarinda
    ? ve = kullanilmaz. Asagidaki komut orneklerini inceleyebilirsiniz.
    Klasorde resmi yer alan modul CC2541 kullanan iki ureticiden birisi olan
    Bolutek'in uretimidir (diger uretici ise HM-10'un ureticisi olan JNHuaMao) ve 
    ayni klasordeki AT komutlarini destekler. 
    
    Modulun kablolari baglanip on tanimli hiz olan 9600 baud'da iletisim baslatildiginda
    asagidaki AT komutlari uygulanabilir. 

    AT
      AT komutuna OK ciktisini verir. AT komutlarini almaya hazir oldugunu soyler.
      AT komutlarinin calisabilmesi icin modulun bir baska bluetooth cihazi ile
      bagli olmamasi gerekir.

   AT+HELP
      Modul uzerinde calistirilabilecek AT komutlarini ve gorevlerini listeler.

   AT+VERSION
      Modulun model numarasini ve firmware (bellenim) yazilim surumunu goruntuler.
      Komutu ciktisi modulun gercek adini verir. Internet arastirmalarinda bu ciktinin
      kullanilmasi daha dogru kaynaklara erismeyi kolaylastirir.
      >AT+VERSION
      MLT-BT05-V4.4

   AT+DEFAULT
      Modulun fabrika ayarlarini yukler. Varsayılan ayarlar:
      Name: HMSoft; Baud: 9600, N, 8, 1; Pin code: 000000; transmit Version

   AT+BAUD
      Modulun iletisim hizini goruntuler ya da ayarlar. Iletisim hizi asagidakilerden biri olabilir.
      
      iletisim hizini 115200 baud yapmak icin:
      AT+BAUD4

   AT+RESET
      Modulu yeniden baslatir.
      
   AT+ROLE
      Modulun rolunu ayarlar. 0: Slave, 1: Master. 
      Master yapmak icin AT+ROLE1

------ AT+INQ
Etraftaki slave aygıtları arar ve listeler.

------ AT+CONN 
Slave aygıta baglanir. Baglanti gerceklestiginde led surekli yanar. 1 numaralı slave aygıta baglanmak icin AT+CONN1
      
   AT+DISC
      Baglantiyi sonlandirir.
      
   AT+POWE
      RF (radyo frekans) yayin gucunu goruntuler veya ayarlar.
      -23dbm, -6dbm, 0dbm, 6dbm'den biri ayarlanabilir. Varsayılan 2'dir.

   AT+PWRM
      Modulun uyku modunu sorgular ve ayarlar.
      0: Otomatik uyku moduna gec.
      1: Uyku modundan cik.
      
   AT+NAME
      Modulun adini goruntuler veya degistirir. Modulun adini degistirmek icin:
      AT+NAMEtankadocom
      
   AT+LADDR
      Modulun MAC adresini goruntuler.
         
   AT+SLEEP
      Modulu daha az enerji tuketmesi amaciyla uyku moduna alir. Uyku modundayken led soner.

   AT+TYPE
      Baglanti icin Pin kodunun gerekli olup olmadigini goruntuler veya tanimlar.
      (0 = No password, 1 = Password pairing, 2 = Password pairing and binding, 3 = Not documented)

   AT+PIN
      Modulun pin kodunu goruntuler veya degistirir. Varsayilan pin 123456'dir.
      6 haneli bir pin kodu tanimlanmalidir. Pin kodunu degistirmek icin oncelikle,
      AT+TYPE ile parola korumali calisma kipine alinir ardindan asagidaki gibi 
      uygulanabilir:
        AT+TYPE2
        AT+PIN123456
      

*/
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11);

char c = ' ';
boolean YeniSatir = true;
void beginATDevice(SoftwareSerial &ATdevice);

void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting...");
  beginATDevice(BTSerial);

  Serial.println("Warning: Set line endings to NL+CR.");
  Serial.println("Ready. Take an AT command:");
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

/*
 * Auto diagnoze moduls's configured speed and connect.
 * Try to connect supported baud and send AT command 
 * and wait for OK reply.
 */
void beginATDevice(SoftwareSerial &ATdevice) {
  String okey;
  byte i;
  int bauds[] = {1200,2400,4800,9600,19200,38400,57600,115200,230400};

  for (i=0; i<=8; i++) {
    ATdevice.begin(bauds[i]);
    delay(100);
    ATdevice.println("AT");
    delay(100);
    okey =  ATdevice.readStringUntil('\r\n');
    if (okey.substring(0, 2) == "OK") {
      Serial.println("AT device connected at " + String(bauds[i]) + " bauds.");
      break;
    }
  }
}
