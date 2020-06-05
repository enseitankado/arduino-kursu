//  Sketch: SwitchingThings_04a
//
//  An  example of using a single button switch to set multiple states or conditions
//  Now using an array to store the LED pins
//
//  Pins
//  D10 to resister and green LED
//  D9 to resister and yellow LED
//  D8 to resister and red LED
//  D2 to push button switch
//  
//  state holds the current status.
//  0 = all off.
//  1 = green LED
//  2 = yellow LED
//  3 = red LED

// Define the pins being used fro the LEDs
//                    green/yellow/red
char LED_Pins_array[] = { 10, 9, 8};

// Array to hold the LED sequence. green, yellow, red, yellow, green.
// position 0 is not used (considered not good practice but keeps the code easy to understand)
char LED_Sequence_array[] = { 10, 9, 8, 9};
byte squenceLength = 4;

int pin_switch = 2;

// variables to hold the new and old switch states
boolean oldSwitchState = LOW;
boolean newSwitchState1 = LOW;
boolean newSwitchState2 = LOW;
boolean newSwitchState3 = LOW;

byte state = -1;

void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");

    for (byte i=0; i< 3; i++)
    {    
         pinMode(LED_Pins_array[i], OUTPUT);    digitalWrite(LED_Pins_array[i],LOW); 
    }
    pinMode(pin_switch, INPUT); 
}
 
void loop()
{
    newSwitchState1 = digitalRead(pin_switch);
    delay(1);
    newSwitchState2 = digitalRead(pin_switch);
    delay(1);
    newSwitchState3 = digitalRead(pin_switch);

    // if all 3 values are the same we can continue
    if (  (newSwitchState1==newSwitchState2) && (newSwitchState1==newSwitchState3) )
    {
        if ( newSwitchState1 != oldSwitchState ) 
        {
           
           // has the button switch been closed?
           if ( newSwitchState1 == HIGH )
           {

                state++;
                if (state > (squenceLength -1) ) { state = 0; }

                // turn all LEDs off. Doing it this way means we do not need to care about the individual LEDs
                // simply turn them all off and then turn on the correct one. 
                for (byte i=0; i< 3; i++)
                {
                     digitalWrite(LED_Pins_array[i],LOW); 
                }

                // Turn on the next LED
                digitalWrite(LED_Sequence_array[state],HIGH); 
           }
           oldSwitchState = newSwitchState1;
        }  
    }
}




