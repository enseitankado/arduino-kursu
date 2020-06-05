/*
Siyah kablo == > Data girişi(D2)
Mavi kablo == > GND
Kahverengi kablo == > 5V
Aşağıdaki örnek kod ile deneme yapabilirsiniz.
*/


int sensor = 2; //mz80 sensörünün bağlı olduğu pin
int ledpini = 13; //led pini
void setup()
{

  Serial.begin(9600);
  pinMode(sensor, INPUT);
  pinMode(ledpini, OUTPUT);
}

void loop()

{
  boolean sensordurum = digitalRead(sensor);
  if (sensordurum == 1)
  {
    Serial.println("high");
    digitalWrite(ledpini, HIGH);
  }
  else
  {
    Serial.println("low");
    digitalWrite(ledpini, LOW);
  }

  delay(10);

}
