/*
	Bu program HC05 modulunun otomatik olarak AT moduna alır
  ve Inquery komutlarinin elle (manual) isletilmesi icin
  yardim saglar.

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
  
  digitalWrite(pin34, HIGH);
  digitalWrite(vcc1, HIGH);
  digitalWrite(vcc2, HIGH);
  
  Serial.println("Dikkat: Satir sonunu NL+CR ayarlamayi unutmayin.");
  Serial.println("HC05 AT moduna gecirildi.\r\n");
  delay(500);
}

/*  INQUERY modunda  cihaz taramasi baslatmak icin 
 *  sirasiyla asagidaki komutlari calistirin.
 *  
 * AT+ORGL
 * AT+UART=38400,0,0
 * AT+INIT
 * AT+IAC=9E8B33
 * AT+CLASS=0
 * AT+ROLE=1
 * AT+INQM=1,1000,15
 * AT+INQ
 * 
 * Son komut arastirmayi baslatir ve tamamlanması
 * 15*1.28 saniye surer. Bu sure icinde etraftaki görülebilir (visible)
 * durumdaki bluetooth cihazlarin mac adresleri ve rssi degerleri listelenir.
 * Ornek degerler asagidaki gibidir:
 * 
 * +INQ:F460:E2:BE83F9,5A020C,FFC0
 * 
 * 
 * 
   AT+ORGL 
      Varsayilan ayarlari (defaults) yukler

   AT+UART? iletisim hizini goruntuler
      Hizi degistirmek icin AT+UART=9600,0,0
      Azami hiz icin AT+UART=460800,0,0
      AT+UART=230400,0,0
      AT+UART=115200,0,0
      AT+UART=57600,0,0
      9600,19200,38400,57600,115200,230400,460800
   
   AT+INIT 
      Modulü SPP (Serial Port Profile) moduna alır. SPP, RS232'nin kablosuz şeklidir.
   
   AT+IAC  
      GIAC (General Inquire Access Code) kodunu goruntuler ve ayarlar. 
      Araştırma (inq) erişim kodu: 9e8b33

   AT+INQC
      Çevredeki bluetooth cihazlarını araştırmayı bırakır.
      Fakat INQUERED modda kalmaya devam eder. AT+RESET ile bu moddan tamamen çıkartılabilir.
    
   AT+IAC  
      GIAC (General Inquire Access Code) kodunu goruntuler ve ayarlar. 
      Araştırma (inq) erişim kodu: 9e8b33
      
   AT+CLASS? Bluetooth'un tanimlanan aygit ve gorev sinifini (Class of Device CoD) goruntuler veya degistirir.
      CoD 32 bitlik bir sayıdır ve modulun ne tür bir cihaz olduğunu tanımlar. Varsayılan 0'dır.
      Aygit siniflari: telefon, hoparlor, oyuncak vb.
      Servis siniflari: ağ, telefon, bilgi, ses hizmeti vb.
      Kodu olusturmak icin: http://bluetooth-pentest.narod.ru/software/bluetooth_class_of_device-service_generator.html
      Kılavuz için: "cod_definition.pdf"
      
   AT+ROLE? 
      Aktif rolu goruntuler ( MASTER(1), SLAVE(0) )
      Aktif rolu degistirmek icin AT+ROLE=0
      Not-1: Master ve Slave moddaki iki BT sifreleri ve hizlari ayniysa otomatik baglanir.
      Not-2: HC06 sadece slave modda calisir    
   
   AT+INQM 
      Araştırma modunun parametreleri ayarlar/sorgular. 
      Param1: 0=inquiry_mode_standard, 1=inquiry_mode_rssi
      Param2: Maximum number of Bluetooth devices to respond to
      Param3: Timeout (1-48 : 1.28s to 61.44s)
         
   AT+INQ 
      Çevredeki bluetooth cihazlarını araştırmaya başlar. Oncesinde AT+INIT ile SPP moduna gecilmesi ve 
      ROLE=1 olarak ayarlanmasi gerekir. Daha sonra AT+STATE? ile mod sorgulanabilir.
      AT+INQ ile araştırma komutu verildikten sonra cihaz araştırma sonuçlarını 
      iletişim moduna geçerek yanıtlar. Bu nedenle modulden gelen cevapları almak için iletişim modunun
      hız yapılandırması (UART) kullanılır. Yani araştırma sonuçlarını alabilmek için cihaza 
      AT+UART hızında bağlanılması gerekir.
    
        Param1： Address
        Param2： Device Class
        Param3： RSSI Signal strength (Receive Signal Strength Indication)
 * 
 */

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
