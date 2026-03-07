float maxAmps, minAmps, noise, amps;


void setup() {
  Serial.begin(9600);
}

void loop() {
  amps = analogRead(A0);
  maxAmps = max(maxAmps, amps);
  minAmps = min(minAmps, amps);
  noise = 511-maxAmps - minAmps;
  Serial.print(amps);
  Serial.print("  ");
  Serial.println(noise);
  
  if (Serial.read() != -1) {maxAmps=amps; minAmps=amps;}
  
  delay(100);

}
