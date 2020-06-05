/*
 * Baglanti semasi
 * Joystic    Arduino
 * ------------------
 * VRX        A0
 * VRY        A1
 * SW(Switch) 7
 */
void setup() {
  pinMode(7, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int x = analogRead(A0);
  int y = analogRead(A1);
  byte buton = digitalRead(7);
  Serial.print("X: " + String(x));
  Serial.print(", Y: " + String(y));
  Serial.println(", B: " + String(buton));
  delay(100);
}
