void setup() {
  // 9600bits/sn hızında haberleşmeyi başlat
  // Arduino ile PC haberleşmesi için seri konsol açılır
  Serial.begin(9600);
}
void loop() {
  // 0. analog pini (A0) oku
  int okunan = analogRead(A0);   
  // Okunan değeri seri konsola yaz
  Serial.println(okunan);
  delay(10);
}
