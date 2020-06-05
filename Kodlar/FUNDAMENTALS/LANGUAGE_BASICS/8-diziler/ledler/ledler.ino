int ledler[]={2,3,4};
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("ledler[] dizisinin ikinci elemanı=");
  Serial.println(ledler[1]);
  delay(1000);
}
