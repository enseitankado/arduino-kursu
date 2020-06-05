/*

	HC05 Komut Modu Yapılandırma Programı

	Bu program HC05 komut moduna (AT) alındığında seri monitör
	aracılığı ile AT komutlarını çalıştırmak için kullanılır.

	Subat 2020
	Özgür Koca

	Github: https://github.com/enseitankado/arduino/tree/master/HC05/1-hc05-at-mode

*/

/*

   HC-05'i AT moduna (bakım) geçirmek
   ----------------------------------------------------------
   1) HC-05' e enerji vermeden once uzerindeki butona basili tutun.
   2) Led 2sn araliklarla yandiginda komut moduna (AT modu) gecmis demektir.
   3) Asagidaki AT komutlarini gondermeden once seri port ekraninizin
   satir sonu karakterini NL+CR ve hızını 9600 olarak ayarlayin.


   Hatirlatmalar:
   --------------
   1 - HC05'i kullanmaya baslamadan once AT+ORGL ile sifirlayin ardindan su ayarlari yapin:
          AT+ORGL
            Varsayilan ayarlar yeniden yuklenir ve iletisim modunda baslar. Tekrar AT moduna alin.
          AT+NAME=ISMINIZ
          AT+PSWD="1881"
          AT+UART=9600,0,0
            9600 baud rate cogu donanimla uyumlu ve yaygin bir hizdir.
          AT+ROLE=0
          AT+CMODE=0
          AT+CLASS=1 veya 7936 (oyuncak)
          AT+RESET          
          
   2 - IPhone sadece BLE (4.0) cihazları desteklediğinden HC05/06'yi (2.0) gormez. HM10/11 veya AT09 modulunu kullanin.
   3 - Diger AT komutlari icin  "at_komut_ve_hata_kodlari.txt" dosyasina goz atin
   4 - HC05, AT modundaykeyn varsayilan olarak 38400 baud hizinda iletisim kurar.
   5 - AT, Master ve Slave calisma modları vardir. Master baglanti baslatabilir Slave baslatamaz.
   6 - Modulun üzerindeki ledin anlamları:
        a - Led surekli hizli yaniyorsa slave modunda iletisim icin beklemektedir.
        b - Led 2sn araliklarla yaniyorsa AT modundadir.
        c - Led seyrek ve duzensiz yaniyorsa eslesmistir.
   
   Android HC05'i gormemekte israr ediyorsa
   ----------------------------------------
       1) Gecici olarak ucak moduna alarak kararsizligi gidermeye calisin.
       2) Bluetooth aygiti daha once eslestirmisseniz, eslesmeyi silin ve tekrar olusturmayi deneyin.
       3) Telefonunuzun ayarlarindan Reset Wi-Fi, mobile & bluetooth secenegini kullanarak sifirlayin.
       4) Telefonunuzun sarji azalmissa guc tasarruf islevleri engel olusturuyor olabilir. Bu dusuk
       ihtimal ama yine de telefonunuzu sarj ederek deneyin. 


       KEY = PIO11(PIN 34)
       AT Moda gecmek için pin34'u HIGH yap ve enerji ver.
	   
   Kullanilabilecek komutlar:
   --------------------------
   AT
     OK cevabı geliyorsa cihazın AT komut modunda olduğu anlaşılır.
   
   AT+VERSION? 
      Modulun firmware surumunu/ureticisini goruntuler

   AT+NAME? 
      Cihazin adini goruntuler.
      Bu sorgu bazı HC05 kartlarda(ZS-40) butona basmadan cevap verir (mini AT mod). 
      Full AT mod için PIO11(Pin34)'i HIGH'a bagla veya komutu göndermeden önce butona basili tut.
      Full AT mod isteyen diger komutlar: AT+INQ, AT+RNAME?
      
      Adi degistirmek icin AT+NAME=TANKADO

   AT+UART? iletisim hizini goruntuler
      Hizi degistirmek icin AT+UART=9600,0,0
      Azami hiz icin AT+UART=460800,0,0
      AT+UART=230400,0,0
      AT+UART=115200,0,0
      AT+UART=57600,0,0
      9600,19200,38400,57600,115200,230400,460800

   AT+PSWD? cihazin parolasini goruntuler
      Parolayi degistirmek icin AT+PSWD="1881"

   AT+ADDR? 
      cihazin donanimm adresini goruntuler ( NAP: UAP: LAP(Hexadecimal) )

   AT+ROLE? 
      Aktif rolu goruntuler ( MASTER(1), SLAVE(0) )
      Aktif rolu degistirmek icin AT+ROLE=0
      Not-1: Master ve Slave moddaki iki BT sifreleri ve hizlari ayniysa otomatik baglanir.
      Not-2: HC06 sadece slave modda calisir

   AT+CMODE? Eslesme modunu goruntuler
      Degistirmek icin AT+CMODE=1 (1=otomatik eslesme aktif, 0=pasif)
      (Master role'de iken kayitli cihazla otomatik eslesir)

   AT+RESET 
      HC-05 yeniden baslatir ve AT moddan cikar. Pin34'un Low olması gerekir.
   
   AT+ORGL 
      Varsayilan ayarlari (defaults) yukler ve iletisim modunda baslatir.

   AT+CLASS? Bluetooth'un tanimlanan aygit ve gorev sinifini (Class of Device CoD) goruntuler veya degistirir.
      CoD 32 bitlik bir sayıdır ve modulun ne tür bir cihaz olduğunu tanımlar. Varsayılan 0'dır.
      Aygit siniflari: telefon, hoparlor, oyuncak vb.
      Servis siniflari: ağ, telefon, bilgi, ses hizmeti vb.
      Kodu olusturmak icin: http://bluetooth-pentest.narod.ru/software/bluetooth_class_of_device-service_generator.html
      Kılavuz için: "cod_definition.pdf"

   AT+STATE? 
      Bluetooth'un mevcut durumu hakkında bilgi verir. 
      Muhtemel cevaplar: “INITIALIZED”, “READY”, “PAIRABLE”, “PAIRED”, “INQUIRING”, “CONNECTING”, “CONNECTED”, “DISCONNECTED”, “NUKNOW”

   AT+RMAAD 
      Eşlemiş tüm cihazları hafızasından siler. 

   AT+INIT 
      Modulü SPP (Serial Port Profile) moduna alır. SPP, RS232'nin kablosuz şeklidir.

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

   AT+INQC
      Çevredeki bluetooth cihazlarını araştırmayı bırakır.
      Fakat INQUERED modda kalmaya devam eder. AT+RESET ile bu moddan tamamen çıkartılabilir.
    
   AT+IAC  
      GIAC (General Inquire Access Code) kodunu goruntuler ve ayarlar. 
      Araştırma erişim kodu: 9e8b33

   AT+INQM 
      Araştırma modunun parametreleri ayarlar/sorgular. 
      Param1: 0=inquiry_mode_standard, 1=inquiry_mode_rssi
      Param2: Maximum number of Bluetooth devices to respond to
      Param3: Timeout (1-48 : 1.28s to 61.44s)
*/

#include <SoftwareSerial.h>

/*
  Devre şeması şu şekildedir:

  Bluetooth  Arduino
  ------------------
  RX     =>  11 (Tx)
  TX     =>  10 (Rx)
  Vcc    =>  Vcc
  Gnd    =>  Gnd
*/
SoftwareSerial BTSerial(10, 11); //Rx, Tx

char c = ' ';
boolean YeniSatir = true;

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(38400);
  Serial.println("Dikkat: Satir sonunu NL+CR ayarlamayi unutmayin.");
  Serial.println("Arduino hazir. AT komutu verin:");
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
