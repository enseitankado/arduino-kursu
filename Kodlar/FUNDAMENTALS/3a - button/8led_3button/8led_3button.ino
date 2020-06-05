/*
 * Can not be use 3 button with interrupt
 * because of Arduino Uno support only 2 (d2,d3) input for interrupt.
 */
const byte leftButton = 12;
const byte blinkButton = 11;
const byte rightButton = 10;

const byte buttonReadCount = 3; // Read multiple times to avoid button bounce
byte led = 2;                   // leds attached 9 from 2
byte ledDelay = 50;                         
unsigned long time;
unsigned long buttonResolution = 500;
volatile unsigned long lastButtonClick;


String signalDirection = "nodirection";

void setup() {  
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(blinkButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  for (byte i= 2; i<=9; i++)
    pinMode(i, OUTPUT);

  Serial.begin(9600);
  Serial.println("Program started.");
}

String readButtons(String signalDirection) {

  if (millis() - lastButtonClick < buttonResolution)
    return signalDirection;
  
  String oldDirection = signalDirection;
  
  byte leftButtonSum = 0;
  byte blinkButtonSum = 0;
  byte rightButtonSum = 0;
  
  for (byte i=0; i<buttonReadCount; i++) {
    leftButtonSum += digitalRead(leftButton);
    blinkButtonSum += digitalRead(blinkButton);
    rightButtonSum += digitalRead(rightButton);
    delayMicroseconds(50);
  }

  if (leftButtonSum == 0) {
    lastButtonClick = millis();  
    if (oldDirection != "left")
      signalDirection = "left";
    else
      signalDirection = "noDirection";
  }

  if (blinkButtonSum == 0) {
    lastButtonClick = millis();
    if (oldDirection != "blink")
      signalDirection ="blink";
    else
      signalDirection = "noDirection";
  }

  if (rightButtonSum == 0) {
    lastButtonClick = millis();
    if (oldDirection != "right")
      signalDirection = "right";
    else
      signalDirection = "noDirection";
  }

  Serial.println("Direction: " + signalDirection);
  return signalDirection;
}


void loop() {
    
    signalDirection = readButtons(signalDirection);

    if (signalDirection == "left"
      or signalDirection == "blink"
      or signalDirection == "right") {

        if (signalDirection == "left" or signalDirection == "right") {

          Serial.println("Led: " + String(led));
          
          digitalWrite(led, OUTPUT);
          delay(ledDelay);
          digitalWrite(led, LOW);

          if (signalDirection == "left") {
            led--;
            if (led < 2)
            led = 9;
          }
  
          if (signalDirection == "right") {
            led++;
            if (led > 9)
            led = 2;
          }
        }

        if (signalDirection == "blink") {
          
          for (byte i=2; i<=9; i++)
            digitalWrite(i, HIGH);
          delay(ledDelay);          
          for (byte i=2; i<=9; i++)
            digitalWrite(i, LOW);
          delay(ledDelay);         
        }        
        
      } else {
                 
          // noDirection
          for (byte i=2; i<=9; i++)
            digitalWrite(i, LOW); 
      }

      
}
