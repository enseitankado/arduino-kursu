
#include <SPI.h>

// D10:SS
// D11:MOSI
// D12:MISO
// D13:CLK

volatile byte tampon[8];
volatile int t_poz = 0;
volatile int i, tampon_boyutu;
volatile bool tampon_doldu = false;
volatile long alma_suresi = 0;

void setup()
{
  Serial.begin(9600); 
  Serial.println("\nMerhaba! Serial basladi. ");
  
  /* TAMPONU HAZIRLA */
  tampon_boyutu = sizeof(tampon);
  for (i=0; i < tampon_boyutu; i++) tampon[i] = 0; // Tamponu temizle
    
  /* SPI PORTUNU HAZIRLA */
  SPI.begin();
  pinMode(SS , INPUT);
  SPCR  |= 0b11000000; //SPI yi aktiflestir
  SPSR  |= 0b00000000; // Varsayılan SPI ayarları  
  sei(); // Kesmeleri aktifleştir
  SPDR = 0;
}

void loop()
{
  while (tampon_doldu)
  {
    long toplam = 0;
    tampon_doldu = false;
    t_poz = 0;    
    for (i=0; i < tampon_boyutu; i++) {
      toplam += tampon[i];
      Serial.println(tampon[i]);
    }
    Serial.println("-----------------");
    Serial.println(toplam);
    Serial.println(alma_suresi); 
    Serial.println("-----------------");
    alma_suresi = 0;
  }
  delay(10);
}

volatile unsigned long start;

/* SPI kesmesi olduğunda çalışacak fonksiyon */
ISR(SPI_STC_vect)
{  
  start = micros();  

  cli(); // Kesmeleri durdur
  tampon[t_poz++] = SPDR;

  Serial.println(SPDR);
  
  if (t_poz == tampon_boyutu)
    tampon_doldu = true;
    
  sei(); // Kesmeleri aktifleştir

  alma_suresi += micros() - start;
}



