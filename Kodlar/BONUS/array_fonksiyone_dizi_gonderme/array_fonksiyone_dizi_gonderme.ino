


void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Arduino basladi.");

  Serial.print(1 << 1);
}


byte arr1[96];
byte arr2[12];

void func(byte *arr1, byte *arr2)
{
  int i,j;
  for (i=0; i<12; i++)
  {
    arr2[i] = 0;
    for (j=0; j<8; j++)
      arr2[i] += arr1[i*8+j] << j;
  }
}

void loop() {
  
  int i;
  for (i=0; i<96; i++) {
    arr1[i] = i % 2;
    Serial.print(arr1[i]);
  }
  
  Serial.print("\n----------\n"); 

  func(arr1, arr2);

  for (i=0; i<12; i++)
  {
    Serial.print(arr2[i]);
  }
  
  Serial.print("\n----------\n"); 
  
  delay(100);
  exit(0);
}
