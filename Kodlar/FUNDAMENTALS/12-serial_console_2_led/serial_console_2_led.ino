/*
 * Bu program seri konsol aracılığı ile
 * 2 adet led'in açılıp/kapatılmasını
 * göstermektedir.
 */
byte led1=2, led2=3;
String komut="";
void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Seri konsoldan veri gelirse devam et
  while (Serial.available() == 0);

  // Seri konsoldan gelen bilgiyi oku
  komut = Serial.readString();

  if (komut == "led1ac") {
    Serial.print("Led 1 açıldı\n");
    digitalWrite(led1, HIGH);
  }

  if (komut == "led1kapat") {    
    Serial.print("Led 1 kapatıldı\n");
    digitalWrite(led1, LOW);
  }

  if (komut == "led2ac") {
    Serial.print("Led 2 açıldı\n");
    digitalWrite(led2, HIGH);
  }

  if (komut == "led2kapat") {
    digitalWrite(led2, LOW);
    Serial.print("Led 2 kapatıldı\n");   
  } 
}
