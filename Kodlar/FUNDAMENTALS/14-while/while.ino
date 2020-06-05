/*
 * Bu uygulama 13. pine bagli 
 * led'i 10 kere kısa aralıklarla yakar
 * ve ardından uzun süre bekler.
 * Adından tekrar aynı işi yapar.
 */

byte LED = 13;
byte sayac = 0;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {

  sayac = 0;

  while (sayac < 10) {
    
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
    
    // sayac degiskenin degerini 1 artır
    sayac = sayac + 1;
  }
  
  delay(2000);

  
}
