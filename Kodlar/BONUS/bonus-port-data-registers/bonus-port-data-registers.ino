// Port registers kaynak: 
// https://www.arduino.cc/en/Reference/PortManipulation
  
void setup() {
  DDRB = B100000; // D13 output
}

void loop() {
  PORTB = B100000;
  delay(200);
  PORTB = B000000;
  delay(200);
}
