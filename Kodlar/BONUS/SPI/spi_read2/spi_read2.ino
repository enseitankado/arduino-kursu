volatile byte tampon[8];
volatile int t_poz = 0;
volatile int i, tampon_boyutu;
volatile bool tampon_doldu = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("\nMerhaba! Serial basladi. ");
  pinMode(MISO, OUTPUT);
  pinMode(SS , INPUT);
    
  SPCR  |= 0b11000000;
  SPSR  |= 0b00000000;
  
  sei();  

  /* TAMPONU HAZIRLA */
  tampon_boyutu = sizeof(tampon);
  for (i=0; i < tampon_boyutu; i++) tampon[i] = 0; // Tamponu temizle
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
  }
  delay(10);
}

/* SPI kesmesi olduğunda çalışacak fonksiyon */
ISR(SPI_STC_vect)
{
  cli();
  /* Interruptlar durdurulmuştur */
  tampon[t_poz++] = SPDR;
  
  if (t_poz == tampon_boyutu)
    tampon_doldu = true;
    
  sei();
  /* Interruptlar başlatılmıştır */
}
