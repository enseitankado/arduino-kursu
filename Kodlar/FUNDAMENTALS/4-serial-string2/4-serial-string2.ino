

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("\n\nSerial basariyla baglandi.");
  Serial.println("\n\nBir string girin:");
}

void loop() {

  while (Serial.available() <= 0);
  
  String str1 = Serial.readString();
  Serial.println("Girilen metin: " + str1);
  Serial.println("Metnin uzunlugu: " + String(str1.length()) );
  
  while (true);
}
