int sayi1 = 0;
int sayi2 = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Serial arabirimi basladi.");
}
void loop() {
  Serial.println("Birinci sayiyi girin:");  
  while (Serial.available() == 0);
  sayi1 = Serial.parseInt();
  
  Serial.println("Ikinci sayiyi girin:");  
  while (Serial.available() == 0);
  sayi2 = Serial.parseInt();

  Serial.print("Sayilarin toplami: ");
  Serial.println(sayi1 + sayi2);
}
