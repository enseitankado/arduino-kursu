void setup() {
pinMode(2, INPUT_PULLUP);    // Dahili PullUp direncini kullanmamızı sağlayan koddur.
pinMode(13, OUTPUT);
}

void loop() { 
int durum = digitalRead(2);   // 2 nolu pindeki butonun durumunu okur.

if (durum == HIGH)      // Eğer buton basılı durumda ise
{ 
digitalWrite(13, LOW);      // Ledi söndür
}

else {
digitalWrite(13, HIGH);     //Ledi yak
}
}

