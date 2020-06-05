//  Sketch: SwitchingThings_05
//
//  An  example of using a button switch as a toggle switch to turn a blinking LED on or off
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
boolean newSwitchState1 = LOW;
boolean newSwitchState2 = LOW;
boolean newSwitchState3 = LOW;

// variables to hold the times
unsigned long timeNow = 0;
unsigned long timePrev = 0;
unsigned int timeWait = 100;

// variables used to control the LED
boolean flashingLEDisON = false;
boolean LEDstatus = LOW;
boolean keyPressed = false;

 
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
    newSwitchState1 = digitalRead(pin_switch);      delay(1);
    newSwitchState2 = digitalRead(pin_switch);      delay(1);
    newSwitchState3 = digitalRead(pin_switch);

    if (  (newSwitchState1==newSwitchState2) && (newSwitchState1==newSwitchState3) )
    {
        if ( newSwitchState1 != oldSwitchState ) 
        {
            if ( newSwitchState1 == HIGH ) { keyPressed = true; } else { keyPressed =  false; }
            oldSwitchState = newSwitchState1;
        }   
    }


    if ( keyPressed )
    {   
         // turn on or turn off the blinking LED
         if ( flashingLEDisON == false)  
         {   
             flashingLEDisON = true;  
         } 
         else                            
         { 
              flashingLEDisON = false; 
              // the LED may be on so we turn it off just is case
              LEDstatus = LOW;  digitalWrite(pin_LED, LEDstatus);
         }
         keyPressed = false;
    }


    // if the blinking LED is on. See if it is time to blink it
    if ( flashingLEDisON == true )
    {
        timeNow = millis();
        if (timeNow-timePrev >= timeWait )    
        {   
              timePrev = timeNow;   
              if (LEDstatus == LOW) { LEDstatus = HIGH; } else { LEDstatus = LOW; }   
              digitalWrite(pin_LED, LEDstatus);
        }
    }
}















