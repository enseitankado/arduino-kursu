byte deger=1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  switch(deger)
  {
  case 0:    Serial.println("Sifir");    break;  
  case 1:    Serial.println("Bir");    break; 
  case 2:    Serial.println("İki");    break; 
  default:   Serial.println("Hicbiri"); break;
  }
Serial.end();
}
