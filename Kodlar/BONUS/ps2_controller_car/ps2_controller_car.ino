
/*
 *  PS2 kumandanin pin isimleri ve baglanti semasi:
 *  
 *     -----------------------------------------------------------
 *    /    #    #    #    |    #    #    #    |    #    #    #    \
 *    -------------------------------------------------------------
 *         A    B    C         D    E    F         G    H    J
 *         
 *    A: -          = Acknowledge (ACK)
 *    B: -          = Bagli degil (NC=Not Connected)
 *    C: clockPin   = Saat sinyali (CLK)
 *    D: attPin     = Attention (ATT)
 *    E: Vcc        = 3.3v
 *    F: GND        = 0v
 *    G: -          = Vibration
 *    H: cmndPin    = Komut pini (CMD)
 *    J: dataPin    = Veri pini 
 *    
*/

/*
 * PS2 kumandasi hakkinda notlar:
 * --------------------------------------------------
 * A) Kumanda belli bir sure kullanilmadiginda uyku moduna gecer.
 * Uyku modundan cikartmak icin kumanda uzerindeki "Start" butonuna basin.
 * 
 * B) Analog kumandalarda joystick yoktur. Dijital kumandalarda
 * analog moda gecildiginde (kumanda uzerindeki analog 
 * butonuna basildiginda mod led'i yanar) joystick'ler kullanilamaz.
 * 
 * C) Kumanda on/off anahtarindan acildiginda etrafta eslesebilecegi bir 
 * kumanda alicisi arar. Bu esnada mode ve power isigi yanıp soner. 
 * Bir alici ile eslestiginde mode isigi soner. 
 * 
 * D) Kumanda alicisi bir kumandaya bagli degilse kirmizi (Tx) isigi 
 * yanip soner. Bir kumanda ile eslesip baglandiginda ise surekli yanar.
 * 
 * 
 * Yazan: Ozgur Koca, Aralik 2018 / Tankado.com
 * Kutuphane: Kevin Ahrendt June 22nd, 2008
 */

#include "Psx.h"                                          

#define consoleDebug false
                                                
#define dataPin 12
#define cmndPin 11
#define attPin 10
#define clockPin 9
#define readDelay 10                               // Kumandanin okuma sikligi (orn: 10ms)
unsigned int komut = 0, sonKomut = komut, temp;    // Kumandadan okunan verileri tutar

const byte MAA = 4; //L298 IN1
const byte MAK = 5; //L298 IN2
const byte MBA = 2; //L298 IN3
const byte MBK = 7; //L298 IN4

const byte MAH = 3; //L298 ENA (Birinci motor hız kontrol)
const byte MBH = 6; //L298 ENB (İkinci motor hız kontrol)

const byte ABS_LED_pin = 13; // ABS ledi

#define TAMHIZ 0
#define SAG 1
#define SOL 2
#define ILERI 3
#define GERI 4
#define DUR 5
#define ABS_DARBE_SURESI 5
#define ABS_YONU -1
#define MIN_BUTON_ARASI 180 // iki buton arasindaki min bekleme suresi (ms)

unsigned int sonButonZamani, sonHareketBaslangici;
bool ABS_AKTIF = true;
byte sonYon;

Psx Ps2;

void setup() {
  pinMode(MAA, OUTPUT);  
  pinMode(MAK, OUTPUT); 
  pinMode(MBA, OUTPUT); 
  pinMode(MBK, OUTPUT);
  pinMode(ABS_LED_pin, OUTPUT);
  if (ABS_AKTIF) 
    digitalWrite(ABS_LED_pin, HIGH);
    
  Serial.begin(9600);  
  Serial.println("Ps2_controller_car basladi. ");
  Serial.println("Yazan: Ozgur Koca, Tankado.com, Aralik/2018\n");
  Serial.println("ABS icin L1 butonunu kullanabilirsiniz.\n");
  delay(500);

  Ps2.setupPins(dataPin, cmndPin, attPin, clockPin, readDelay);
}

void sagSolIleri(byte yon) {
  
  // B motoru yavas donsun
  if (yon == SAG) {
    analogWrite(MAH, 255);
    analogWrite(MBH, 128);
    sonYon = yon;
  } 
  // A motoru yavas donsun
  else if (yon == SOL) {
    analogWrite(MAH, 150);
    analogWrite(MBH, 255);    
  } 
  // Her iki motor da tam hiz donsun
  if (yon == TAMHIZ) {
    analogWrite(MAH, 255);
    analogWrite(MBH, 255);    
  }
}

void ileriGit(byte yon, bool yon_guncelle = true) {

  if (yon_guncelle) {
    sonYon = ILERI;
    sonHareketBaslangici = millis();
  }
  
  digitalWrite(MAA, HIGH);
  digitalWrite(MAK, LOW);
  digitalWrite(MBA, LOW);
  digitalWrite(MBK, HIGH);
  sagSolIleri(yon);
}

void geriGit(byte yon, bool yon_guncelle = true) {
  
  if (yon_guncelle) {
    sonYon = GERI;
    sonHareketBaslangici = millis();
  }

  digitalWrite(MAA, LOW);
  digitalWrite(MAK, HIGH);
  digitalWrite(MBA, HIGH);
  digitalWrite(MBK, LOW);
  sagSolIleri(yon);
}

void sagaDon(byte yon, bool yon_guncelle = true) {
  
  if (yon_guncelle) {
    sonYon = SAG;
    sonHareketBaslangici = millis();
  }
  
  digitalWrite(MAA, HIGH);
  digitalWrite(MAK, LOW);
  digitalWrite(MBA, HIGH);
  digitalWrite(MBK, LOW);
  sagSolIleri(TAMHIZ);
}

void solaDon(byte yon, bool yon_guncelle = true) {
  
  if (yon_guncelle) {
    sonYon = SOL;
    sonHareketBaslangici = millis();
  }  
  digitalWrite(MAA, LOW);
  digitalWrite(MAK, HIGH);
  digitalWrite(MBA, LOW);
  digitalWrite(MBK, HIGH);
  sagSolIleri(TAMHIZ);  
}

void _dur() {
  digitalWrite(MAA, LOW);
  digitalWrite(MAK, LOW);
  digitalWrite(MBA, LOW);
  digitalWrite(MBK, LOW);
  sonHareketBaslangici = millis(); 
}

void absUygula(byte yon) {


    if (yon == DUR)
    exit;
    
    if (!(yon == ILERI | yon == GERI | yon == SAG | yon == SOL))
    exit;
    
    unsigned int hareket_suresi = millis() - sonHareketBaslangici;    
    // 2sn'de max hıza ulasildigi varsayildi
    if (hareket_suresi > 1000)
      hareket_suresi = 1000;
    
    byte ABS_darbe_sayisi = int(hareket_suresi / ABS_DARBE_SURESI / 2 / 10);

    if (consoleDebug)
      Serial.println("sonYon: " + String(yon) + ", ABS_darbe_sayisi: " + String(ABS_darbe_sayisi) + ", Son hareket suresi: "+ String(hareket_suresi)+"ms");
    
    for (byte i=0; i < ABS_darbe_sayisi ; i++) 
    {
      if (yon == ILERI) geriGit(ABS_YONU, false);
      if (yon == GERI) ileriGit(ABS_YONU, false);
      if (yon == SAG) solaDon(ABS_YONU, false);
      if (yon == SOL) sagaDon(ABS_YONU, false);
      
      delay(int(ABS_DARBE_SURESI*10));
      _dur();
      delay(int(ABS_DARBE_SURESI*5));
    }
}

/*
 * Bu fonksiyon ABS'li durus uygular. 
 * Gidis yonunun aksi istikametine; 
 * ABS_MANEVRA_SAYISI kadar, ABS_MANEVRA_SURESI suresince donus uygular.
 */
void dur() {

  if (ABS_AKTIF)
    absUygula(sonYon);
  else
    _dur();
  
  sonYon = DUR;
}

void loop()
{
  // Kumanda alicisini oku
  delay(25);
  komut = Ps2.read();

  if (sonKomut != komut) {

    sonKomut = komut;

    if (consoleDebug) {
      Serial.print(String("Komut: ") + String(komut) + ", BIN:");
      Serial.println(komut, BIN);
    }

    // SOL JOYSTICK KONTROLLERI
    bool ileri = komut & 8;
    bool geri = komut & 2;
    
    // SAG JOYSTICK KONTROLLERI
    bool sag = komut & 1024;
    bool sol = komut & 256;
   
    if (ileri and sag) ileriGit(SAG); 
    else if (ileri and sol) ileriGit(SOL);
    else if (geri and sag) geriGit(SAG);
    else if (geri and sol) geriGit(SOL);
    else if (ileri) ileriGit(TAMHIZ);
    else if (geri) geriGit(TAMHIZ);
    else if (sag) sagaDon(SAG);
    else if (sol) solaDon(SOL);  
    else if (komut == 0) dur();
  
    /*
     * L1 butonu ABS'yi etkinlestirir/etkisizlestirir
     */
    if (komut & 8192) {
      /*
       * Tekrarlanan buton basimini sonumlendir
       * İki buton basimi arasinda en az 250ms olsun.
       */
      if (ABS_AKTIF and (millis() - sonButonZamani > MIN_BUTON_ARASI)) { 
        sonButonZamani = millis();       
        if (consoleDebug)
          Serial.println("ABS etkisizlestirildi.");
        digitalWrite(ABS_LED_pin, LOW);  
        ABS_AKTIF = false;        
      }
      
      else if (millis() - sonButonZamani > MIN_BUTON_ARASI) {
        sonButonZamani = millis();      
        if (consoleDebug)
          Serial.println("ABS etkinlestirildi.");          
        digitalWrite(ABS_LED_pin, HIGH);
        ABS_AKTIF = true;
                      
      }
    }
    
    /*
     * R1 butonu karsi dogrultuya doner
     */    
    if (komut & 4096) {
      
      if (millis() - sonButonZamani > MIN_BUTON_ARASI) {

        sonButonZamani = millis();
        
        if (consoleDebug)
          Serial.println("R1 basıldi. Karsi istikamet manevrasi.");
          
        _dur();
        solaDon(SOL, false);
        delay(300);
        dur();        
        
      }
    }
      
  } // if (sonKomut != komut)
   
        
  delay(20);
}
