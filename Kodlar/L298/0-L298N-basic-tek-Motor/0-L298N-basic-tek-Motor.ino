const byte MBA = 8; //L298 IN3
const byte MBK = 9; //L298 IN4
void setup()
{
  pinMode(MBA, OUTPUT);
  pinMode(MBK, OUTPUT);
}
void loop()
{
  digitalWrite(MBA, LOW);
  digitalWrite(MBK, HIGH);
  /*
    // motoru durdurma
  delay(3000);
  digitalWrite(MBA, LOW);
  digitalWrite(MBK, LOW);
  // motorun dönme yönünü değiştirme
  delay(3000);
  digitalWrite(MBA, HIGH);
  digitalWrite(MBK, LOW);
      // motoru durdurma
  delay(3000);
  digitalWrite(MBA, HIGH);
  digitalWrite(MBK, HIGH);
  delay(3000);
  */
}
