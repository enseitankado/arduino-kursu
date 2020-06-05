int i;
void setup() {
  for (i=2; i<=9; i++)
  {
  pinMode(i, OUTPUT);
  }
}

void loop() {
  for(i=2;i<=9;i++)
  {
	  digitalWrite(i,HIGH);
	  delay(1000);
  }
}
