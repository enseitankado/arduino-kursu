/*
Siyah kablo == > Data girişi(D2)
Mavi kablo == > GND
Kahverengi kablo == > 5V
Aşağıdaki örnek kod ile deneme yapabilirsiniz.
*/


int sayac, sensor = 2; //mz80 sensörünün bağlı olduğu pin

void setup()
{
  Serial.begin(9600);
  pinMode(sensor, INPUT);
}

void loop()

{
  boolean sensordurum = digitalRead(sensor);
  if (sensordurum == 1)
  {
    sayac++;
    Serial.println(sayac);    
  }
  else
  {
    sayac--;
    Serial.println(sayac);    
  }

  if (sayac<1)
    sayac=1;

  if (sayac>100)
    sayac=100;
  delay(10);
}
