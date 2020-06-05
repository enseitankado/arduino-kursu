boolean buton1,buton2;
void setup() {
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(13,OUTPUT);
}
void loop() {
  buton1=digitalRead(8);
  buton2=digitalRead(9);
  if(buton1==LOW)
  {
    digitalWrite(13,HIGH);  
  }
  if(buton2==LOW)
  {
    digitalWrite(13,LOW);  
  }
}
