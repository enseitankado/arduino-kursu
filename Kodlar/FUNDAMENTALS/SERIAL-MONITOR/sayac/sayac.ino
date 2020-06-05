int sayac = 0;

void setup()
{ 
  Serial.begin(9600);
} 

void loop()
{ 
  Serial.print("Seri haberlesme: ");
  Serial.println(sayac);
  sayac++;
  delay(1000);
}

