String isim = "";
String soyisim = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Serial arabirimi 115200 baud da basladi.");
}

void loop() {  
  Serial.println("Bana adini soyler misin? ");  
  while (Serial.available() == 0);
  isim = Serial.readString();

  Serial.println("Peki bana soyadini soyler misin? ");  
  while (Serial.available() == 0);
  soyisim = Serial.readString();
  
  Serial.println("Merhaba: " + isim + soyisim + " memnun oldum\n");
}
