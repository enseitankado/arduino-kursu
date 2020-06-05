byte a=0;
byte b=0;
byte c=100;

void setup() {
// put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {
// put your main code here, to run repeatedly:
Serial.print(a+=2);
Serial.print(",");
Serial.print(b++);
Serial.print(",");
Serial.println(c);
}
