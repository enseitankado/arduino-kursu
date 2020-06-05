const byte MAK = 11; //L298 IN1
const byte MAA = 10; //L298 IN2
const byte MBA = 9; //L298 IN3
const byte MBK = 8; //L298 IN4

const byte MAH = 6; //L298 ENA (Birinci motor hız kontrol)
const byte MBH = 5; //L298 ENB (İkinci motor hız kontrol)

void setup()
{
  pinMode(MAA, OUTPUT);
  pinMode(MAK, OUTPUT);
  pinMode(MBA, OUTPUT);
  pinMode(MBK, OUTPUT);
  Serial.begin(9600);
}

void loop()
{

  // MOTOR A ve B potansiyometre hizinda donsun
  // int potans = map(analogRead(A0), 507, 1023, 0, 255);
  analogWrite(MAH,  100);
  digitalWrite(MAA, HIGH);
  digitalWrite(MAK, LOW);

  // MOTOR B saat yonunun tersine donsun
  analogWrite(MBH, 100);
  digitalWrite(MBA, LOW);
  digitalWrite(MBK, HIGH);

  //Serial.println("Ham deger: " + String(analogRead(A0)));
  //Serial.println(" Maplenmis deger: " + String(potans));
}

