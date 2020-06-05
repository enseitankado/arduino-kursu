
#include <Servo.h>

/*
 * Kablo renkleri:
 * Kırmızı = Vcc = 5v
 * Kahve = Gnd = 0v
 * Turuncu = PWM = Sinyal (servoPin)
 * 
 * UYARI: Seri konsolu satır sonu yok olarak ayarlayin
 */
Servo Servo1;
const byte servoPin = 3;
byte derece;

void setup() {
  Servo1.attach(servoPin);
  Servo1.write(0);
  Serial.begin(9600);
  Serial.println("Seri baglanti basladi.");
}

void loop() {
  
  if (Serial.available()){
      derece = Serial.parseInt();
      Servo1.write(derece);
      Serial.println("Servo konumlandirildi: " + String(derece));
  }
}
