int wait = 50;

void setup() {
  for (int  i=0; i<=19; i++)
    pinMode(i, OUTPUT);
}

void loop() {
  for (int i=0; i<=19; i++) {
    digitalWrite(i, HIGH);
    delay(wait);
    digitalWrite(i, LOW);    
  }
}
