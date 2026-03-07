volatile byte tampon[1024];
volatile int t_poz = 0;
volatile int i, tampon_boyutu;
int toplam = 0, hatali=0;

void setup()
{
  Serial.begin(9600);  
  Serial.print(F_CPU);
  Serial.println(" Hz. \nSS963 PWM kontrolcusu v.1 basladi. ");
    
  pinMode(MISO, OUTPUT);
  pinMode(SS , INPUT);  
  SPCR  |= 0b11000000;
  SPSR  |= 0b00000000;
  
  sei(); // Kesmeleri aktiflestir
  delay(500); 
}

void loop()
{
  delay(100);
}


void tamponu_guncelle()
{
  // Kesmeleri durdur
  cli();
  
  for (i=0; i < t_poz; i++)
    toplam = toplam + tampon[i];
  
  t_poz = 0;
/*  
  Serial.print("--- > Byte toplami: ");
  Serial.print(toplam);    
  Serial.print(", Byte miktari: ");
  Serial.println(i);
*/
  if (toplam != 96)
  //if (hatali % 100 == 0)
  Serial.println(++hatali);
  
  toplam = 0;
  
  // Kesmeleri aktiflestir
  sei();
}

/*
 * Kesme vektörü. 
 * 255 verisini görene kadar tamponu gunceller
 */
ISR(SPI_STC_vect)
{
  if (SPDR == 255)
    tamponu_guncelle();
  else
    tampon[t_poz++] = SPDR;
}
