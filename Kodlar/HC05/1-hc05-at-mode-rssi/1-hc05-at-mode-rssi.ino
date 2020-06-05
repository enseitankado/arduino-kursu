/*

	Bu program HC05 modulunu arastirma moduna alarak,
  arastirma sonuclarini ekrana yazar.

	Devre şeması şu şekildedir:

	Bluetooth   Arduino
	------------------
	RX          =>  11 (Tx)
	TX          =>  10 (Rx)
  Vcc         =>  6+7 (power over GPIO)
  Key (pin34) =>  2 (Modulun key pini yoksa Pin34'e [PIO11] bir kablo lehimleyin)


	Temmuz 2018
	Özgür Koca
*/

#include <SoftwareSerial.h>


SoftwareSerial BTSerial(10, 11);
#define vcc1  6         // Supply ~40mA
#define vcc2  7         // Supply ~40mA
#define pin34  2        // HC05's key pin to switch AT mode
#define INQinterval 15  // INQuery timeout

char c = ' ';
boolean YeniSatir = true, plotterMode = false;
String deviceAddress, deviceClass, deviceRSSI;
unsigned long lastMillis;
void printBanner();
void sendATCmd(String atCommand, bool fetchBTBuffer = true, bool plotterMode = false);

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
  if (not plotterMode) {
    printBanner();  
    Serial.println("Dikkat: Arduino yeniden basladi.");
    Serial.println("Dikkat: Satir sonunu NL+CR ayarlamayi unutmayin.");
  }

  digitalWrite(pin34, HIGH);
  digitalWrite(vcc1, HIGH);
  digitalWrite(vcc2, HIGH);
  if (not plotterMode) {
    Serial.println("HC05 AT moduna gecirildi.\r\n");
    Serial.println("Cizim modu icin p yazin.\r\n");
  }
  delay(1000);
  
  sendATCmd("AT+NAME?");
  sendATCmd("AT+UART=38400,0,0");
  sendATCmd("AT+INIT");
  sendATCmd("AT+IAC=9E8B33");
  sendATCmd("AT+CLASS=0");
  sendATCmd("AT+ROLE=1");
  sendATCmd(String("AT+INQM=1,1000,") + String(INQinterval));

  lastMillis = millis();
  sendATCmd("AT+INQ");
  sendATCmd("AT+STATE");
  
  if (not plotterMode)
    Serial.println("Cihaz arastirmasi basladi, sinyal siddetleri yazdiriliyor...");
}


void loop()
{
  // Bluetooth modulunden gelen verileri oku
  BTSerial.listen();
  if (BTSerial.available()) 
  {
    String str = BTSerial.readStringUntil('\r\n');

    // Inquery cevabı gelmisse RSSI (Received signal strength indication) degerini yaz
    // Gelen veri: +INQ:8EF:3B:258C20,240428,FFCA
    if (str.substring(0, 5) == "+INQ:") {
      String INQReply = str;
      byte rssi = getRSSIDecValue(INQReply);
      
      Serial.println(rssi);
    } 
    else 
      if (not plotterMode)
      Serial.print(str);
  }

  // Seri konsoldan AT komutlarını oku
  if (Serial.available()) {

    c = Serial.read();
    BTSerial.write(c);

    // Switch plotter mode
    if (c = 'p') {
      if (plotterMode == true) {
        plotterMode = false;
        Serial.println("Plotter mode: Off");
      } else {
        plotterMode = true;
        Serial.println("Plotter mode: On");
      }
      delay(500);
    }
        

    // AT komutunu seri konsola yazar.
    // '>' kullanici komutu oldugunu belirtir.
    if (YeniSatir ) {
      if (not plotterMode)
        Serial.print(">");
      YeniSatir = false;
    }

    if (not plotterMode)
    Serial.write(c);
    // CR gelirse yeni satira basla
    if (c == 10) YeniSatir = true;
  }

  // INQuery timeout dolmuşsa yeniden başlat
  if ( (millis() - lastMillis) > INQinterval * 1000 *1.28 ) {
    lastMillis = millis();
    sendATCmd("AT+INQ", false, plotterMode);
  }
}

int getRSSIDecValue(String INQReply) {    
    // Örnek: +INQ:8EF:3B:258C20,240428,FFCA
    int index1 = INQReply.indexOf(',');  
    deviceAddress = INQReply.substring(0, index1);
    int index2 = INQReply.indexOf(',', index1+1 );
    deviceClass = INQReply.substring(index1+1, index2+1);
    deviceRSSI = INQReply.substring(index2+1);
    char bf[4];
    deviceRSSI.substring(2,4).toCharArray(bf, 4);
    int dec = strtol(bf,0,16);
    if (dec > 100 and dec < 255)
      return dec;
}

void sendATCmd(String atCommand, bool fetchBTBuffer = true, bool plotterMode = false) {
  if (!plotterMode)
  Serial.print( String(">" + atCommand + "\r\n") );
  BTSerial.print( String(atCommand + "\r\n") );
  delay(300);
  if (fetchBTBuffer and not plotterMode)
    Serial.print(BTSerial.readStringUntil("\n"));
  else
  BTSerial.flush();
}

#define LINE(name,val) Serial.print(name); Serial.print("\t"); Serial.println(val);

void printBanner() {
  Serial.println("\nCPU:");
  // Arduino SW version
  LINE("ARDUINO", ARDUINO);
  // Oscillator frequency
  LINE("F_CPU", F_CPU);
}
