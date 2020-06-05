
/*
   Kablo renkleri:
   Kırmızı = Vcc = 5v
   Kahve = Gnd = 0v
   Turuncu = PWM = Sinyal (servoPin)
*/
#include <Servo.h>
Servo ServoA;
byte servoPin = 3;

void setup() {
  ServoA.attach(servoPin);
  ServoA.write(0);
  Serial.begin(9600);
  Serial.println("Seri baglanti basladi.");
}
void loop() {
  int x = analogRead(A0);
  byte derece = map(x, 0, 1023, 0, 180);
  ServoA.write(derece);
  Serial.println("Servo konumu: " + String(derece) + " derece.");
  delay(100);
}
