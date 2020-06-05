#define RELAY 2               //Röle(Relay) kontrol ucu 
void setup() 
{ 
pinMode(RELAY, OUTPUT);       //Röle kontrol ucu çıkış olarak tanımlanmıştır 
Serial.begin(9600);           // seri haberleşme 9600 hızı ile başlatılmıştır. 

} 

void loop() 
{ 

digitalWrite(RELAY,LOW);     // Röle'ye enerji verilmediğinden Normalde kapalı konumdadır.
                             // Ledi söndürmek için röleye enerji vermek gerekir.
//delay(2000);
//digitalWrite(RELAY,HIGH);  
//delay(5000);                           
}
