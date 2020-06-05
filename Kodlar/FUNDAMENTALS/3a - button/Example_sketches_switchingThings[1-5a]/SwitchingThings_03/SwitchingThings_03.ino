//  Sketch: SwitchingThings_03
//
//  An  example of using a button switch as a toggle switch to turn an LED on or off
//
//  Pins
//  D10 to resister and LED
//  D2 to push button switch
//  
 
// Define the pins being used
int pin_LED = 10;
int pin_switch = 2;
 
// variables to hold the new and old switch states
boolean oldSwitchState = LOW;
boolean newSwitchState = LOW;
 
boolean LEDstatus = LOW;
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    pinMode(pin_LED, OUTPUT);  
    digitalWrite(pin_LED,LOW); 
 
    pinMode(pin_switch, INPUT); 
}
 
void loop()
{
    newSwitchState = digitalRead(pin_switch);
 
    if ( newSwitchState != oldSwitchState ) 
    {
       // has the button switch been closed?
       if ( newSwitchState == HIGH )
       {
           if ( LEDstatus == LOW ) { digitalWrite(pin_LED, HIGH);  LEDstatus = HIGH; }
           else                    { digitalWrite(pin_LED, LOW);   LEDstatus = LOW;  }
       }
       oldSwitchState = newSwitchState;
    }   
}
