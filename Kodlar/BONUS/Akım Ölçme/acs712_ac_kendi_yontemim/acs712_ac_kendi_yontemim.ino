
const unsigned long sampleTime = 200000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
const unsigned long numSamples = 2550UL;                                // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
const unsigned long sampleInterval = sampleTime/numSamples;            // the sampling interval, must be longer than then ADC conversion time
const int adc_zero = 511;                                                     // relative digital zero of the arudino input from ACS712 (could make this a variable and auto-adjust it)

void setup()
{
 //analogReference(EXTERNAL);
 Serial.begin(9600);
}

void loop()
{ 
 float voltKareler = 0;
 float adcVolt = 0;
 float VCC = readVcc();
 
 unsigned long currentAcc = 0;
 unsigned int count = 0;
 unsigned long prevMicros = micros() - sampleInterval ;
 
 while (count < numSamples)
 {
   if (micros() - prevMicros >= sampleInterval)
   {
     int adc = analogRead(A0) - adc_zero;
     float adcVolt = adc * VCC / 1023;
     voltKareler += adcVolt * adcVolt;
     
     
     ++count;
     prevMicros += sampleInterval;
   }
 }
  float rms = sqrt((float)voltKareler/(float)numSamples);
 Serial.println(rms);
}


long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
