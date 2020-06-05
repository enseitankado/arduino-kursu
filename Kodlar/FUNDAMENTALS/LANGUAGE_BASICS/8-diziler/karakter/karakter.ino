char karakter[]={"samsun"};
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("char ile dizi işlemleri");
  Serial.println("-----------------------");
  Serial.println(karakter[1]);
  delay(1000);
  Serial.println(karakter[5]);
  delay(1000);
  Serial.println(karakter[15]);
  delay(1000);
}
