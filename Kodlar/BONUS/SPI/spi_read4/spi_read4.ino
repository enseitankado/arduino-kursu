// Based on tutorial from http://gammon.com.au/spi

#include <SPI.h>

volatile boolean process_it;
uint8_t clr;
uint8_t randNumber;

void setup (void)
{
  Serial.begin (115200); // Turned off as it apparently affects interrupts
  pinMode(MISO, OUTPUT); // master in, *slave out* 
  SPCR |= _BV(SPE); // turn on SPI in slave mode
  process_it = false;
  clr=SPDR; // clear out any junk from past runs 
  SPI.attachInterrupt(); 
}  // end of setup

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  process_it = true;
} 

void loop (void) // wait for flag set in interrupt routine
{
  if (process_it)
  {
    randNumber = random(15);
    SPI.transfer(randNumber);
    process_it = false;
  }  
}
