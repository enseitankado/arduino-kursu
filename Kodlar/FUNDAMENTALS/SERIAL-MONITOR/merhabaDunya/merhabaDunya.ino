
int sa[4]; 
  

void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(9600);           // set up Serial library at 9600 bps
  //Serial.println("Motor test!");
  String oneLine = "0101";
  for(int i=0; i<=3; i++)
  {   
    String a = oneLine.substring(i,i+1); 
    sa[i]=a.toInt();
  Serial.println(sa[i]);
}
digitalWrite(13,boolean(sa[2]));
}

void loop() {

}


