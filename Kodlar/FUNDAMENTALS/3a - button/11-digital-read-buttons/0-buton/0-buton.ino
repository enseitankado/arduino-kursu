boolean butonDurum;           //butona basılıp basılmadığını kontrol eden değişken
void setup() {
  pinMode(8,INPUT_PULLUP);    //Arduino digital pinlerinde dahili pullup direnci vardır.  
  pinMode(13,OUTPUT);
}
void loop() {
  butonDurum=digitalRead(8);  //Butona basıldı ise 0(LOW), basılmadı ise 1(HIGH) olur
  if(butonDurum==LOW)             
  {
    digitalWrite(13,LOW);     // Butona basıldığında butonu söndürecektir.
  }
  else
  {
    digitalWrite(13,HIGH);   // Butondan elimizi kaldırdığımızda yanacaktır.
  }
}
