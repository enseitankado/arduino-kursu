// Sensorun trig pini Arduinonun
// 7 numaralı ayağına bağlandı
byte trigPin = A0;
// Sensorun echo pini Arduinonun
// 8 numaralı ayağına bağlandı
byte echoPin = A1;

int sure;
int uzaklik;

void setup() {
  // trig pini çıkış olarak ayarlandı
  pinMode(trigPin, OUTPUT);
  // echo pini giriş olarak ayarlandı
  pinMode(echoPin, INPUT);
  // Seri haberlesme baslatildi
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}
void loop()
{
  digitalWrite(trigPin, LOW); /* sensör pasif hale getirildi */
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */
  sure = pulseIn(echoPin, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
  uzaklik = sure / 29.1 / 2; /* ölçülen sure uzaklığa çevriliyor */

  if (uzaklik < 30 and uzaklik > 25) {
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
  }
  
  else if (uzaklik < 25 and uzaklik > 20) {
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
  }
  
  else if (uzaklik < 20 and uzaklik > 15) {
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
  }
  
  else if (uzaklik < 15 and uzaklik > 10) {
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
  }

  else if (uzaklik < 10) {
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      
  } else {
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);    
  }

  

  
  Serial.println(uzaklik); /* hesaplanan uzaklık bilgisayara aktarılıyor */
  delay(10);
}
