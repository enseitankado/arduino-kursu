// RGB Led'in pinlerini tanımlama
const int kirmiziPin = 3;
const int yesilPin = 5;
const int maviPin = 6;
int red = 0;
int green = 0;
int blue = 0;
void setup() { 
  Serial.begin(9600);       // Seri iletişim başlasın.
  pinMode(kirmiziPin, OUTPUT);
  pinMode(yesilPin, OUTPUT);
  pinMode(maviPin, OUTPUT);

}

void loop() {
    Serial.println("Kirmizi rengin degerini gir:");
    while(Serial.available()==0) {}
    red = Serial.parseInt();
    delay(1000);

    Serial.println("Yesil rengin degerini gir:");
    while(Serial.available()==0) {}
    green = Serial.parseInt();
    delay(1000);

    Serial.println("Mavi rengin değerini gir:");
    while(Serial.available()==0) {}
    blue = Serial.parseInt();
    delay(1000);
    
    
      red = 255 - constrain(red, 0, 255);
      green = 255 - constrain(green, 0, 255);
      blue = 255 - constrain(blue, 0, 255);

      analogWrite(kirmiziPin, red);
      analogWrite(yesilPin, green);
      analogWrite(maviPin, blue);

      // Oluşan rengin Hexadecimal karşılığı
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.println(blue, HEX);
 }
