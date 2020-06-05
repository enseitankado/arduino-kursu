/*
*	Cok sayida lojik girisi (1/0) arduino veya bir baska 
*	mikrodenetleyicili/mikroislemcili sisteme okutmak gerektiginde
*	parallel input - serial output denilen 74165 gibi entegreler kullanilir.
*	Bu entegreler kaskat seklinde ard arda baglanarak okuma yapilacak
*	giris sayisi artirilabilir. 74165 shift-load isleminin ardindan,
*	sirasiyla tum girislerini seri olarak Qh cikisina aktarir.
*	Ayni isi yapan I2C arayuzune sahip cipler de vardir.
*	
*	Ozgur Koca, Tankado.com, Temmuz/2019
*	
*/


byte DATA_IN = 2;   // Seri verinin okunacagi pin. Once A biti okunur.
byte CLK_PIN = 3;   // Saat (CLK) sinyalinin gonderilecegi pin.
byte SH_LD = 4;     // Girisleri kaydir ve yukle pini.
byte WAIT = 1;      // Yarim periyodluk saat darbesinin suresi.

void setup() {
  pinMode(DATA_IN, INPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(SH_LD, OUTPUT);
  digitalWrite(SH_LD, HIGH);
  Serial.begin(9600);
}

/*
 * Saat (clock) sinyali gonder.
 * Her seri bit okumasindan sonra
 * bir saat palsi gonder.
 */
void clockPulse() {
  digitalWrite(CLK_PIN, 0);
  delay(WAIT);
  digitalWrite(CLK_PIN, 1);
  delay(WAIT);
}

/*
 * Shift & Load
 * Paralel girislerdeki bilgiyi kaydedicilere yukler veya kaydirir.
 * LOW yukler.
 * HIGH kaydirir.
 */
void shiftLoad() {
  digitalWrite(SH_LD, LOW);
  delay(WAIT);
  digitalWrite(SH_LD, HIGH);
  delay(WAIT);
}

void loop() {
  // Girislerdeki bilgiyi cikis kaydedicilerine yukle ve kaydir.
  shiftLoad();  
  for (byte i = 0; i < 16; i++) {    
    Serial.print(digitalRead(DATA_IN));
    clockPulse();
  }    
  Serial.println();
  delay(WAIT);
}
