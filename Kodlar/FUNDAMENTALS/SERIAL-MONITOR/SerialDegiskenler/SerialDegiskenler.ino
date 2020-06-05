String ad;        //Adınız string bir değişkende tutulacaktır.
int yas;          //Yaşınız int bir değişkende tutulacaktır.
float boy;        //Boyunuz float bir değişkende tutulacaktır. 
 
void setup() {
  Serial.begin(9600);      // Serial port açılıyor.
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}
 
void loop() {
  
  Serial.println("Adınızı giriniz: ");        //Kullanıcı adı girilmesi isteniyor.
  while (Serial.available()==0) {             //Ad girişi için bekleniyor.
  
  }
  ad=Serial.readString();                     // Girilen isim ad isimli değişkene atanıyor.
  
  Serial.println("Yaşınızı giriniz: ");        //Yaşınızın girilmesi isteniyor.
  while (Serial.available()==0)  {
  
  }
  yas=Serial.parseInt();                      //Girilen değer yas isimli değişkene atanıyor
  
  Serial.println("Boyunuzu giriniz: ");      //Boyunuzun girilmesi isteniyor.
  while (Serial.available()==0)  {
  
  }
  boy=Serial.parseFloat();                   //Girilen değer boy isimli değişkene atanıyor.
  
  Serial.print("Merhaba ");                    //Değerler ekrana yazdırılıyor.
  Serial.print(ad);
  Serial.print(", siz ");
  Serial.print(yas);
  Serial.println(" yasindasiniz,");
  Serial.print("ve boyunuz ");
  Serial.print(boy);
  Serial.println(" cm 'dir.");
  Serial.println("");
  if(ad=="guven")                       //ad değeri guven ise 13.pindeki led yanıyor ekranda Tebrikler yazıyor.
  {
    Serial.println("Tebrikler");
    digitalWrite(13,HIGH);
    }
   else                                    //ad değeri guven değil ise 13.pindeki led sönüyor.
   {
    digitalWrite(13,LOW);
    }
}

