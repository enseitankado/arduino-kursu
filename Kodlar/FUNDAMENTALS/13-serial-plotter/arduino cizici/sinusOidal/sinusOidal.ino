void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
for(int a=0;a<360;a++)
{
  Serial.println(sin(a*(PI/180)));
  }
}
