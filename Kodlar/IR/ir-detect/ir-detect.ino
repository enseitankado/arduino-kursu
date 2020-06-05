#include <IRremote.h>
int sinyal = 11;
IRrecv IRAlici(sinyal);
decode_results sonuc;
void setup()
{
  Serial.begin(9600);
  Serial.println("IR alici etkinlestirilecek.");
  IRAlici.enableIRIn();  
  Serial.println("Etkinlestirildi.");
}



void loop() {
  if (IRAlici.decode(&sonuc)) {
    Serial.println(sonuc.value);
    IRAlici.resume();
  }
  delay(100);
}

