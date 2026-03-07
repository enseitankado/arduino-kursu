const int analogGiris = A0;
const unsigned long orneklemeSayisi = 10000;
float ortalama;
int i;
void setup() {  
  Serial.begin(9600);
  //analogReference(INTERNAL);
}

void loop() {

/*
  for (i=0; i<orneklemeSayisi; i++)
    ortalama += analogRead(analogGiris);

  ortalama /= (float) orneklemeSayisi;
*/

Serial.println((analogRead(analogGiris)*5/1024.0));
  
}
