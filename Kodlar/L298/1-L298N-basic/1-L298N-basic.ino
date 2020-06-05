const byte MAK = 8; //L298 IN1
const byte MAA = 9; //L298 IN2
const byte MBA = 10; //L298 IN3
const byte MBK = 11; //L298 IN4

void setup()
{
  pinMode(MAA, OUTPUT);
  pinMode(MAK, OUTPUT);
  pinMode(MBA, OUTPUT);
  pinMode(MBK, OUTPUT);
}

void loop()
{
  digitalWrite(MAA, HIGH);
  digitalWrite(MAK, LOW);
  digitalWrite(MBA, LOW);
  digitalWrite(MBK, HIGH);
  analogWrite(5, 90);
  analogWrite(6, 90);
  delay(1000);
  digitalWrite(MAA, LOW);
  digitalWrite(MAK, HIGH);
  digitalWrite(MBA, HIGH);
  digitalWrite(MBK, LOW);
  analogWrite(5, 255);
  analogWrite(6, 255);
  delay(1000);
  digitalWrite(MAA, LOW);
  digitalWrite(MAK, LOW);
  digitalWrite(MBA, LOW);
  digitalWrite(MBK, LOW);
  delay(1000);
}

