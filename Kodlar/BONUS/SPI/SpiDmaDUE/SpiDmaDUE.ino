//SpiDmaDUE.ino SPI via DMA for the Arduino DUE

/* SPI.h clocks in at 12 seconds in this test sketch.
The DMA version clocks in at 0.59 seconds for the same loop.
DMA-SPI is 21 x faster than SPI. (This is partly because
the SPI.h requires a loop transferring byt-by byte.
The DMA version transfers the entire array in on service)
*/

#include "SpiDmaDUE.h"
#include <SPI.h>

#define CS 10

#define SPI_BUFF_SIZE 1024
uint8_t rx_buffer[SPI_BUFF_SIZE];
uint8_t tx_buffer[SPI_BUFF_SIZE];
  
  
  long loop_cnt = 4000;

  
void setup() {
    Serial.begin(9600);
    Serial.println("initializing array");
    for (int i = 0; i < SPI_BUFF_SIZE; i++){
        tx_buffer[i] = 0b11010101;
    }
    Serial.println("Start");

    float t1, t2;
    double mbs;
    char str[64];   //for printing
  
    int spi_rate;

    // standard test loop
    SPI.begin(CS);       
    spi_rate = 2;       //fastest speed
    SPI.setClockDivider(spi_rate);
    t1 = micros();
    for (long i = loop_cnt; i > 0; i--){
        for (int j = SPI_BUFF_SIZE-1; j > 0; j--){
        SPI.transfer(CS,tx_buffer[j],SPI_CONTINUE);  //on byte at a time
        }
     SPI.transfer(CS,tx_buffer[0],SPI_LAST);  //last byte   
    }

    t1 = micros() - t1;
    //mbs = 8*SPI_BUFF_SIZE/(float)t1;
    sprintf(str,"SPI.h spi-rate: %d - %d seconds", spi_rate, (int) t1/1000000);
    Serial.println(str);


    // DMA test loop
    spi.begin(CS);       
    spi_rate = 1;       //fastest speed
    spi.setClockDivider(spi_rate);
    t2 = micros();
    for (long i = loop_cnt; i > 0; i--){
        spi.transfer(CS,tx_buffer,SPI_BUFF_SIZE);  //now transfer
    }
    t2 = micros() - t2;
    //mbs = 8*SPI_BUFF_SIZE/(float)t1;
    sprintf(str,"SpiDmaDUE.h spi-rate: %d - %g seconds", spi_rate, t2/1000000);
    Serial.println(str);
    sprintf(str,"SpiDmaDUE is: %g x faster", t1/t2);
    Serial.println(str);
}

void loop() {

   
    for (long i = loop_cnt; i > 0; i--){
        spi.transfer(CS,tx_buffer,SPI_BUFF_SIZE);  //now transfer
    }
   
}
