/*
 * CIRCUIT DETAILS:
 * ----------------
 * LEDs on digital 3 to 19
 * Button on digital digital 2 connected with 10K pull-up resistor.
 */

const byte wait = 200;
const byte buttonPin = 2;     // Interrupt Pins (Uno): 2, 3
volatile byte dir = 1;
byte led = 0;

void buttonPressed() {
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis(); 
 if (interrupt_time - last_interrupt_time > 100) 
 {
    if (digitalRead(buttonPin) == 0) {
      dir = !dir;  
      //Serial.println("buttonPressed(). direction=" + String(dir));      
    }
 }
 last_interrupt_time = interrupt_time;
}

void setup() {
  //Serial.begin(9600);
  //Serial.println("Setup started.");
  
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, LOW);
  
  for (int  i=0; i<=19; i++)
    if (i != buttonPin)
      pinMode(i, OUTPUT);
    else
      continue;
}

void loop() {
  
  digitalWrite(led, HIGH);
  delay(wait);
  digitalWrite(led, LOW);
    
  if (dir) {
    led++;
    if (led > 19) led = 2;
  } else { 
    led--;
    if (led < 3) led = 19;
  }
  //Serial.println("dir:" + String(dir) + " led:" + String(led));
}
