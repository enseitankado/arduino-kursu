/*
     Arduino Uno:        1kb EEPROM.
     Arduino Mega:       4kb EEPROM.
*/
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("EEPROM'un kapasitesi " + String( EEPROM.length() ));
  // Bir EEPROM hücresine 0..255 arasında deger yazılabilir.  
  EEPROM[0] = 0;
  EEPROM[2] = 100;  
  EEPROM[3] = 10;
  EEPROM[1] = 56;
}

void loop() {
  Serial.println(EEPROM[0]);
  Serial.println(EEPROM[1]);
  Serial.println(EEPROM[2]);
  delay(2000);
}
