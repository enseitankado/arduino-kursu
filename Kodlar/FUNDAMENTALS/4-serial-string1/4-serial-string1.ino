

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("\n\nSerial basariyla baglandi.");
}

void loop() {
  
  String str1 = "Merhaba";
  String str2 = "Dunya";
  Serial.println(str1 + " " + str2);

  float pi = 3.14;
  Serial.println(str1 + " " + String(pi));
  while (true);
}
