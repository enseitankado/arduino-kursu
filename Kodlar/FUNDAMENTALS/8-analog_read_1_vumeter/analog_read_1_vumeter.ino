int i;
void setup() {
  for (i=2; i<=9; i++)
  pinMode(i, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:  
  int okunan = analogRead(A0);
  
  if (okunan>200 and okunan<300)
  digitalWrite(2, HIGH);
  else digitalWrite(2, LOW);

  if (okunan>300 and okunan<400)
  digitalWrite(3, HIGH);
  else digitalWrite(3, LOW);

  if (okunan>400 and okunan<500)
  digitalWrite(4, HIGH);
  else digitalWrite(4, LOW);

    if (okunan>500 and okunan<600)
  digitalWrite(5, HIGH);
  else digitalWrite(5, LOW);

    if (okunan>200 and okunan<300)
  digitalWrite(2, HIGH);
  else digitalWrite(2, LOW);

    if (okunan>600 and okunan<700)
  digitalWrite(6, HIGH);
  else digitalWrite(6, LOW);

    if (okunan>700 and okunan<800)
  digitalWrite(9, HIGH);
  else digitalWrite(9, LOW);

  if (okunan>800 and okunan<900)
  digitalWrite(8, HIGH);
  else digitalWrite(8, LOW);
}
