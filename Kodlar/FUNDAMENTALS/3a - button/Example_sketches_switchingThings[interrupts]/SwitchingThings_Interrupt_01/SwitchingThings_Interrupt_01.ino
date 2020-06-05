//  Sketch: SwitchingThings_Interuupt_01
//
//  An  example of using a, interrupt and a button switch to turn an LED on and off
//
//  Pins
//  D10 to resister and LED
//  D2 to push button switch
//  

// Define the pins being used
int pin_LED = 10;
int pin_switch = 2;

// variables used to control the LED
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

    attachInterrupt( digitalPinToInterrupt(pin_switch), blink, RISING );
}
 
void loop()
{
}


void blink()
{
      if (LEDstatus == LOW) { LEDstatus = HIGH; } else { LEDstatus = LOW; }   
      digitalWrite(pin_LED, LEDstatus);
}








