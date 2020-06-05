#include <IRremote.h>     //kızıl ötesi alıcı için gerekli olan kütüphane
#include <Servo.h>        //servo kütüphanesi


int IR_PIN = 2;
IRrecv IRAlici(IR_PIN);   // Arduino nun 2 nolu pini alıcının girişi

decode_results sonuc;     // ** results değerinde kumandadan bir tuşa bastığımızda alıcının algıladığı değeri okuyacağız. **
byte LED1 = 10;
Servo servo1;
void setup()
{
  pinMode(LED1, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("IR alici etkinlestirilecek.");
  IRAlici.enableIRIn();
  Serial.println("Etkinlestirildi.");
  servo1.attach(9);          // Arduinonun 9. pinini servo motor için çıkış yaptık
  servo1.write(0);            // başlangıçta açısını 0(sıfır) a çektik. (Kapı Kapalı durumu)
}

void loop() {
  if (IRAlici.decode(&sonuc)) {   // Eğer değer geldiyse
    
    Serial.println(sonuc.value);

    if (16724175 == sonuc.value){
      digitalWrite(LED1, HIGH);
      servo1.write(90);
      sonuc.value=0;
      }

    if (16718055 == sonuc.value){
      digitalWrite(LED1, LOW);
      servo1.write(0);
      sonuc.value=0;
      }
    
    IRAlici.resume();
  }
  delay(100);
}

