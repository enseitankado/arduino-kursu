const byte MAK = 11; //L298 IN1
const byte MAA = 10; //L298 IN2
const byte MBA = 9; //L298 IN3
const byte MBK = 8; //L298 IN4
const byte trig = 12; //trig
const byte echo = 13; // echo
int mesafe;
int sure;

const byte MAH = 6; //L298 ENA (Birinci motor hız kontrol)
const byte MBH = 5; //L298 ENB (İkinci motor hız kontrol)

void setup()
{
  pinMode(echo,INPUT);
  pinMode(trig,OUTPUT);
  pinMode(MAA, OUTPUT);
  pinMode(MAK, OUTPUT);
  pinMode(MBA, OUTPUT);
  pinMode(MBK, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(trig,HIGH);
  delay(1);    
  digitalWrite(trig,LOW);     // düzenli olarak ses dalgası gönderiliyor

  sure=pulseIn(echo,HIGH);   //ses dalgasının geri dönüş süresi hesaplanıyor,
  mesafe=(sure*0.017);        //mesafe hesaplanıyor.

  if(mesafe>0 && mesafe<40)
      {
      analogWrite(MAH,  200);
      digitalWrite(MAA, LOW);
      digitalWrite(MAK, LOW);

      analogWrite(MBH, 200);
      digitalWrite(MBA, LOW);
      digitalWrite(MBK, LOW);   
      }

  else
      {
      analogWrite(MAH,  100);
      digitalWrite(MAA, 1);
      digitalWrite(MAK, LOW);
      
      analogWrite(MBH, 100);
      digitalWrite(MBA, LOW);
      digitalWrite(MBK, 1);
       }
  Serial.println(mesafe);
}

