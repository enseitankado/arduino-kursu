void setup() {
  Serial.begin(9600);
}

int pot1 = 0;
float volt = 0;

float haritala(int x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
  
  pot1 = analogRead(A0); //0..1023
  volt = haritala(pot1, 0.0, 1023.0, 0.0, 5.0); 
  
  Serial.println(volt);
  delay(100);
}
