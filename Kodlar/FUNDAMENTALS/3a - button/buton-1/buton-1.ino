void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}
byte durum;
void loop() {
  byte buton = digitalRead(2);

  if (buton == 0)
    if (durum == 1)
      durum = 0;
    else
      durum = 1; 
    
  if (durum == 1)
    digitalWrite(13, 1);
  else
    digitalWrite(13, 0);
    
  Serial.println(buton);
}
