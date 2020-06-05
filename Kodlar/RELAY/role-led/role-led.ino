#define role 2             //Röle kontrol ucu 
int led = 4;               //2.Led kontrol ucu 
void setup() 
{ 
pinMode(role, OUTPUT);    //Röle kontrol ucu çıkış olarak tanımlanmıştır 
pinMode(led, OUTPUT);     //Bağladığımız ikinci ledimiz çıkış olarak tanımlanmıştır.
digitalWrite(led,HIGH);  // Ledin kontrol edildiği pin - 2.LED Yanar
} 

void loop() 
{ 
digitalWrite(role,LOW);   // Röle'nin kontrol edildiği pin - 1.LED Yanar 
delay(2000); // 2 Saniye Bekle 
digitalWrite(role,HIGH);  // Röle'nin kontrol edildiği pin - 1.LED Söner
delay(2000); // 2 Saniye Bekle 
} 
