void setup() {
  Serial.begin(9600);
  delay(1000);

  // Read "1.1V" reference against AREF
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  unsigned int reading = ADC;
  // unsigned int reading = ADCL | (ADCH << 8);
  float fraction = reading / 1024.0;
  Serial.println(F("Use a meter to measure the voltage between GND and AREF pins."));
  Serial.print(F("Multiply that voltage by "));
  Serial.print(fraction, 7);
  Serial.println(F(" to get the value of the internal voltage reference."));
  Serial.println(F("Then multiply that by 1204000 to get the constant to use in readVCC()."));

  // unsigned long VCC = 1120500UL / reading;
}

void loop() {}
