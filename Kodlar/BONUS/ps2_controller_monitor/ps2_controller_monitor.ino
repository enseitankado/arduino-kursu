
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
 * Notlar:
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
                                                          
#define dataPin 12
#define cmndPin 11
#define attPin 10
#define clockPin 9
#define readDelay 10      // Kumandanin okuma sikligi (orn: 10ms)
unsigned int data = 0;    // Kumandadan okunan verileri tutar

Psx Ps2;

void setup() {
  Ps2.setupPins(dataPin, cmndPin, attPin, clockPin, readDelay);
  Serial.begin(9600);
}

void loop()
{
  // Kumanda alicisini ok
  data = Ps2.read();
  
  // SOL JOYSTICK KONTROLLERI
  if (data & 8)
    Serial.println("SOL_JOYSTICK: ileri basildi.");
  if (data & 2)
    Serial.println("SOL_JOYSTICK: geri basildi.");
  if (data & 4)
    Serial.println("SOL_JOYSTICK: sag basildi.");   
  if (data & 1)
    Serial.println("SOL_JOYSTICK: sol basildi.");

    
  // SAG JOYSTICK KONTROLLERI
  if (data & 2048)
    Serial.println("SAG_JOYSTICK: ileri basildi.");
  if (data & 512)
    Serial.println("SAG_JOYSTICK: geri basildi.");
  if (data & 1024)
    Serial.println("SAG_JOYSTICK: sag basildi.");    
  if (data & 256)
    Serial.println("SAG_JOYSTICK: sol basildi.");

  // KONTROL BUTONLARI
  if (data & 16)
    Serial.println("start butonuna basildi");
  if (data & 128)
    Serial.println("select butonuna basildi.");
  if (data & 8192)
    Serial.println("L1 butonuna basildi.");
  if (data & 32768)
    Serial.println("L2 butonuna basildi.");
  if (data & 4096)
    Serial.println("R1 butonuna basildi.");
  if (data & 16384)
    Serial.println("R2 butonuna basildi.");
        
  delay(50);
}
