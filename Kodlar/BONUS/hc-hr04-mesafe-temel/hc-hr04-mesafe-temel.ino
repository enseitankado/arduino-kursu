// Sensorun trig pini Arduinonun 
// 7 numaralı ayağına bağlandı
byte trigPin = 12; 
// Sensorun echo pini Arduinonun 
// 7 numaralı ayağına bağlandı
byte echoPin = 13;

int sure;
int uzaklik;

void setup(){
  // trig pini çıkış olarak ayarlandı
  pinMode(trigPin, OUTPUT);
  // echo pini giriş olarak ayarlandı
  pinMode(echoPin, INPUT);
  // Seri haberlesme baslatildi
  Serial.begin(9600);
}
void loop()
{
  digitalWrite(trigPin, LOW); /* sensör pasif hale getirildi */
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH); /* Sensore ses dalgasının üretmesi için emir verildi */
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  /* Yeni dalgaların üretilmemesi için trig pini LOW konumuna getirildi */ 
  sure = pulseIn(echoPin, HIGH); /* ses dalgasının geri dönmesi için geçen sure ölçülüyor */
  uzaklik= sure /29.1/2; /* ölçülen sure uzaklığa çevriliyor */            

  Serial.println(uzaklik); /* hesaplanan uzaklık bilgisayara aktarılıyor */  
  delay(10); 
}
