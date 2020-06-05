void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

#define analog_buff_size 40
float analog_avg;
float analog_sum, analog_val;
int analog_arr[analog_buff_size];

int analog, i, j;

void loop() {
  analog_val = analogRead(A0);

  // Dizi elemanlarini sola kaydir
  for (int i = 1; i < analog_buff_size; i++)
    analog_arr[i - 1] = analog_arr[i];

  // Okunan degeri dizinin sonuna yaz
  analog_arr[analog_buff_size - 1] = analog_val;

  // Dizideki tum elemanlarin geometrik ortalamasini hesapla
  analog_sum = 0;
  for (int i = 0; i < analog_buff_size; i++)
    analog_sum += pow(analog_arr[i],2);
  analog_avg = (int) sqrt(analog_sum / analog_buff_size);

  // Ham degeri ve ortalama degeri yaz
  j++;
  if (j % analog_buff_size == 0)
  Serial.println(String(analog_val) + "," + String(analog_avg - 5));
}
