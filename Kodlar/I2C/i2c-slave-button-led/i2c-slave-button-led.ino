#include <Wire.h>
#define SLAVE_ADDRESS 0x5E 

byte response[2];
volatile short LDR_value;
const int LDR_pin = A0; 

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS); 
  Wire.onRequest(sendData);
  pinMode(LDR_pin, INPUT);
}

void sendData(){
  LDR_value = analogRead(LDR_pin);  
  LDR_value=map(LDR_value, 0, 1023, 0, 255);
  response[0]= (byte) LDR_value;
  Wire.write(response, 2);
  Serial.println( String(response[0]) + " gonderildi");
}

void loop() {
  delay(1000);
}


