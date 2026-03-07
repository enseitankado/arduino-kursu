/*
  ss963   Uno         Logic Analyzer
  -----   ---         --------------
  Latch   9           3
  CLK     13  (SCL)   2
  Data    11  (MOSI)  1
  Reset   8           4
  Gnd     Gnd         Gnd
*/

#include <SPI.h>

byte ports[96];
byte byte_array[12];
byte card_bytes[96];

int DATA_PIN = 11; /* MOSI */
int LATCH_PIN = 9;
int RESET_PIN = 8;
int LATCH_DELAY_MS = 1;
int i, j;

void setup() {
  pinMode(RESET_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); /* 16MHz/2 = 8MHz hiza ayarla */

  Serial.begin(115200);
  Serial.print("Arduino basladi.");
  
  for (i=0; i<48; i++)
    ports[i] = 255;

  for (i=48; i<96; i++)
    ports[i] = 128;
}

void convert_bit_array_to_byte_array(byte *ports, byte *byte_array) {
  int i, j;
  for (i = 0; i < 12; i++)
  {
    byte_array[i] = 0;
    for (j = 0; j < 8; j++)
      byte_array[i] += ports[i * 8 + j] << j;
  }
}

void fill_ports(byte *ports, byte on_off)
{
  int i;
  for (i = 0; i < 96; i++)
    ports[i] = on_off;
}

void write_ports(byte *ports)
{
  convert_bit_array_to_byte_array(ports, byte_array);

  for (j = 11; j >= 0; j--) {
    SPI.transfer(byte_array[j]);
  }

  latch();
}

int phase, port;

void loop() {

  for (phase=0; phase<255; phase++)
  {
    for (port=0; port<96; port++)
    {
     // byte_order = 
      //card_bytes[i] |=
    }
  }
}


void latch() {
  digitalWrite(LATCH_PIN, HIGH);
  delay(LATCH_DELAY_MS);
  digitalWrite(LATCH_PIN, LOW);
}
