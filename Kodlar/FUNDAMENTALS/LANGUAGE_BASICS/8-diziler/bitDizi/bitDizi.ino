int led[4]; 
String satir="0101";
  
void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(9600);           
  for(int i=0; i<=3; i++)
  {   
    String a = satir.substring(i,i+1);     //satir içindeki karakteri sıra ile a değişkenine atar
    led[i]=a.toInt();
    Serial.println(led[i]);
  }
digitalWrite(13,boolean(led[3]));
}

void loop() {
}

// 8 ledi 8bitlik data ile kontrol ediniz.
