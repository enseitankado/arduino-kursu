#include <IRremote.h>
#define TK19_PIN 2
IRrecv irrecv(TK19_PIN);
decode_results results;
 
void setup()
{
 Serial.begin(9600);
 irrecv.enableIRIn();
}
 
void loop() {
 if (irrecv.decode(&results))
 {
 Serial.println(results.value, HEX); // gelen mesajı daha kolay anlayabilmek için hexadecimal formatta ekrana yazdırıyoruz.
 irrecv.resume();
 }
 delay(100);
}
