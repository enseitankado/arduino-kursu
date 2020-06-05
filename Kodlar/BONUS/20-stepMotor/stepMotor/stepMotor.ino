int step=0;
int motorPin1 = 8; // 1. Bobinin bağlı olduğu  pin
int motorPin2 = 9; // 2. Bobinin bağlı olduğu pin
int motorPin3 = 10; // 3. Bobinin bağlı olduğu pin
int motorPin4 = 11; // 4. Bobinin bağlı olduğu pin 

int delayTime = 10; // Bekleme süresi
void setup() {
pinMode(motorPin1, OUTPUT); // Bobinleri çıkış olarak tanımlıyoruz.
pinMode(motorPin2, OUTPUT); // Bobinleri çıkış olarak tanımlıyoruz.
pinMode(motorPin3, OUTPUT); // Bobinleri çıkış olarak tanımlıyoruz.
pinMode(motorPin4, OUTPUT); // Bobinleri çıkış olarak tanımlıyoruz.
Serial.begin(9600);
}

void loop() // Loop döngüsü
{
digitalWrite(motorPin1, HIGH); // 1. Bobini 1 konumuna getiriyoruz.
digitalWrite(motorPin2, LOW); // 2. Bobini 0 konumuna getiriyoruz.
digitalWrite(motorPin3, LOW); // 3. Bobini 0 konumuna getiriyoruz.
digitalWrite(motorPin4, LOW); // 4. Bobini 0 konumuna getiriyoruz.
delay(delayTime); // Bekleme süresi
digitalWrite(motorPin1, LOW); // 1. Bobini 0 konumuna getiriyoruz.
digitalWrite(motorPin2, HIGH); // 2. Bobini 1 konumuna getiriyoruz.
digitalWrite(motorPin3, LOW);// 3. Bobini 0 konumuna getiriyoruz.
digitalWrite(motorPin4, LOW); // 4. Bobini 0 konumuna getiriyoruz.
delay(delayTime); // Bekleme süresi
digitalWrite(motorPin1, LOW); // 1. Bobini 0 konumuna getiriyoruz.
digitalWrite(motorPin2, LOW); // 2. Bobini 0 konumuna getiriyoruz.
digitalWrite(motorPin3, HIGH); // 3. Bobini 1 konumuna getiriyoruz.
digitalWrite(motorPin4, LOW); // 4. Bobini 0 konumuna getiriyoruz.
delay(delayTime); // Bekleme süresi
digitalWrite(motorPin1, LOW); // 1. Bobini 0 konumuna getiriyoruz.
digitalWrite(motorPin2, LOW); // 2. Bobini 0 konumuna getiriyoruz.
digitalWrite(motorPin3, LOW); // 3. Bobini 0 konumuna getiriyoruz.
digitalWrite(motorPin4, HIGH); // 4. Bobini 1konumuna getiriyoruz.
delay(delayTime); // Bekleme süresi
step+=4;
Serial.println(step);
}
