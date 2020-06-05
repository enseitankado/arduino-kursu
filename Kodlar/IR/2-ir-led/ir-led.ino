#include <IRremote.h>

int IR_PIN = 2;
IRrecv IRAlici(IR_PIN);

decode_results sonuc;
byte LED1 = 10;

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

    if (16724175 == sonuc.value)
      digitalWrite(LED1, HIGH);

    if (16718055 == sonuc.value)
      digitalWrite(LED1, LOW);
    
    IRAlici.resume();
  }
  delay(100);
}

