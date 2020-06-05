
#include <Servo.h>

/*
 * 
 * SERVO (Sequantally encoded rotational variable operation)
 * Sıralı olarak kodlanmış dönel değişken çalışma
 * 
 * Kablo renkleri:
 * Kırmızı = Vcc = 5v
 * Kahve = Gnd = 0v
 * Turuncu = PWM = Sinyal
 */
Servo Servo1;
Servo Servo2;
byte servoPin = 3;
byte servo2Pin = 5;
byte derece;

void setup() {
  Servo1.attach(servoPin);
  Servo1.write(0);
  Servo2.attach(servoPin);
  Servo2.write(0);
  Serial.begin(9600);
  Serial.println("Seri baglanti basladi.");
}

void loop() {
  
  if (Serial.available()){
    
      derece = Serial.parseInt();
      
      if (derece <= 180) {
        
        Servo1.write(derece);
        Servo2.write(derece);
        Serial.println("Servo konumlandirildi: " + String(derece));
        
      } else {
        
        if (200 == derece) {
          Servo1.detach();
          Serial.println("Servo arduino'dan ayrildi");
        }

        if (201 == derece) {
          Servo1.attach(servoPin);
          Serial.println("Servo arduino'ya baglandi");
        }
        
      }
  }
}
