/* KODLAR
  Serial.begin(baud_rate) -  Seri iletişimi başlatır.
  Serial.available()      -  Serial bağlantının kullanılabilir 
                              olup olmadığını kontrol eder. 
                              if veya while ile kullanılır.
                              if(Serial.available()>0){...}
                              while(Serial.available==0){}
  Serial.end()          -   Seri iletişimi sonlandırır.
  Serial.print()        -   Seri ekrana değer yazar.
  Serial.println()      -   Seri ekrana değer yazar.İmleç alt satıra iner
  Serial.read()         -   Girilen değeri char olarak döndürür
  Serial.readString()   -   Girilen değeri string olarak döndürür
  Serial.parseInt()     -   Girilen değeri int olarak döndürür
  Serial.parseFloat()   -   Girilen değeri float olarak döndürür
  
  
  Serial.print(value, format) şeklinde kullanımı ile gönderilecek değerlerin formatlarını değiştirerek gönderir. 
  Serial.print(64, BIN) Binary çıkış verir "1000000" şeklinde
  Serial.print(64, OCT) Octal çıkış verir  "100" şeklinde
  Serial.print(64, DEC) Decimal çıkış verir "64" şeklinde
  Serial.print(64, HEX) Hexedecimal çıkış verir  "40" şeklinde 

 
*/
String ad;
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Adınızı giriniz:");
  while(Serial.available()==0){}
  ad=Serial.readString();
  Serial.println(ad);
}
