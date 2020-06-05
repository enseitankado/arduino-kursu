void setup() {
 
  Serial.begin(9600);
  while (!Serial);
 
  
  for (int i = 0; i < 100; i++) {
    Serial.println (100);             // 0 -  99 arası veri noktaları
  }
 
  for (int i = 0; i < 100; i++) {
    Serial.println (50);              // 100 -  199 arası veri noktaları
  }
 
  for (int i = 0; i < 100; i++) {
    Serial.println (100);             // 200 -  299 arası veri noktaları
  }
 
  for (int i = 0; i < 100; i++) {
    Serial.println (50);              // 300 -  399 arası veri noktaları
  } 
 
  for (int i = 0; i < 100; i++) {
    Serial.println (100);             // 400 -  499 arası veri noktaları
  }
  
}
 
void loop() {
 
}
