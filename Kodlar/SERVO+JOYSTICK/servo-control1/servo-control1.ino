/*
 * Kablo renkleri:
 * Kırmızı = Vcc = 5v
 * Kahve = Gnd = 0v
 * Turuncu = PWM = Sinyal = 3
 */
#include <Servo.h>
Servo Servo1;
int servoPin = 3; // Turuncu

void setup() {
  Servo1.attach(servoPin);
}
void loop() {
  Servo1.write(0);
  delay(1000);
  Servo1.write(90);
  delay(1000);
  Servo1.write(180);
  delay(1000);
}
