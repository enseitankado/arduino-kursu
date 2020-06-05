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
byte btnSum = 0;

void setup() {
  //Serial.begin(9600);
  //Serial.println("Setup started.");
  
  pinMode(buttonPin, INPUT_PULLUP);  
  
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

  btnSum = 0;
  for (int i=0; i<2; i++) {
    btnSum += digitalRead(buttonPin);
    delayMicroseconds(50);
  }

  if (btnSum == 0) {
    dir = !dir;  
    //Serial.println("->buttonPressed(). direction=" + String(dir));
  }
    
  if (dir) {
    led++;
    if (led > 19) led = 2;
  } else { 
    led--;
    if (led < 3) led = 19;
  }

  //Serial.println("dir:" + String(dir) + " led:" + String(led));
}
