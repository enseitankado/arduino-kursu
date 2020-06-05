#include <IRremote.h>

int IR_PIN = 11;
IRrecv IRAlici(IR_PIN);

decode_results sonuc;
byte LED1 = 7;

void setup()
{
  pinMode(LED1, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("IR alici etkinlestirilecek.");
  IRAlici.enableIRIn();
  Serial.println("Etkinlestirildi.");
}

void loop() {
  if (IRAlici.decode(&sonuc)) {
    
    Serial.println(sonuc.value);

    if (16753245 == sonuc.value)
      digitalWrite(LED1, HIGH);

    if (16736925 == sonuc.value)
      digitalWrite(LED1, LOW);
    
    IRAlici.resume();
  }
  delay(100);
}

