#include <SPI.h>

int LATCH_PIN = 13;
int RESET_PIN = 12;
int LATCH_DELAY_US = 1;
int DUMY_SLAVE = 4;
char tBuffer[80];

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(DUMY_SLAVE, SPI_CLOCK_DIV2);
  
  SPI.begin(DUMY_SLAVE);
  //SPI.setClockDivider(DUMY_SLAVE, 8); //SPI_CLOCK_DIV2
  
  Serial.begin(115200);
  
  sprintf(tBuffer, "SS963 PWM Driver started. CLK=%d MHz\n", F_CPU/(1000*1000));
  Serial.print(tBuffer);
}

void loop() {
  SPI.transfer(DUMY_SLAVE, B11110000, SPI_CONTINUE);
  //SPI.transfer(DUMY_SLAVE, B00001111, SPI_CONTINUE);
  //SPI.transfer(DUMY_SLAVE, B00000000, SPI_CONTINUE);
  latch();
}

void latch() {
  digitalWrite(LATCH_PIN, HIGH);
  delayMicroseconds(LATCH_DELAY_US);
  digitalWrite(LATCH_PIN, LOW);
}

