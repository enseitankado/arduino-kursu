/*
   Bu ornek program gurultu iceren sinyallerin kalman filtresi
   kullanilarak temizlenmesini orneklemektedir. A0 girisinden okunan
   ve gurultulu oldugu varsayilan sinyal kalman filtresinin Q ve R
   parametreleri kalibre edilerek temizlenmektedir.

   Yazan: Ozgur Koca, Mart 2019, URL: Tankado.com
*/
void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);
}
/*
   Kalman filtresi bir sinyal icindeki istenmeyen gurultulerden
   kurtulmak icin tahmine dayali bir filtreleme sunar.
   Filtre onceki tahminine Q ve R parametreleri yardimiyla duzeltme
   uygulayarak yeni tahminler uretir.

   DİKKAT!: Bu fonksiyon, en iyi filtreleme icin R ve Q parametreleri
   ile kalibre edilmelidir.
*/

float kalman_filter (float veri, float &kalman_eski, float &ss_onceki)
{
  /*
     Q = Gurultu Kovaryansı = Standart sapma
     ------------------------------
     Yeni standart sapmayı/kovaryansı (SS/kovaryans=Q) hesapla
     SS degeri gercek sinyalin icerdigi gurultunun miktarina gore belirlenir.
     SS ufak verilirse (orn: 0.001); hesaplanacak kalman tahmininin gercek degere yakinsamasi
     daha cok sayida ornege ihtiyac duyar ve iyi tahminin uretilmesi zaman alir
     ancak tahmin gercek degere daha yakin olur. SS buyuk verilirse daha az veri ornegi
     ile daha kaba bir tahmin yapilir ve tahminin gercek degere ulasmasi daha kisa surer.
     Ancak bu durumda; tahmin edilen sinyal gurultulerden daha fazla etkilenir.
  */
  float Q = 0.01;
  float ss_yeni = ss_onceki + Q;

  /*
     R = Hata kovaryansı
     -------------------
     Kalman kazancini, eski kazanci ve yeni standart sapmayi kullanarak
     yeniden hesapla. R hata kovaryansi asagidaki kriterlere gore elle
     kalibre edilmelidir. Tahmin edilen sinyaldeki hata orani fazla ise
     R artirilarak kapatilmaya calisilir. R, Q'da oldugu gibi dusuk
     degerlerden baslanarak artirilir.

        Q>R secildiginde hatalarin modelin kendinde daha baskin oldugu,
        R>Q secildiginde olcumlerdeki hatanin daha baskin oldugu
        Q=R durumunda ise hatanin hangisinde daha baskin oldugunun bilinmedigi varsayilmis olur
  */
  float R = 0.2;
  float kalman_kazanci = ss_yeni / (ss_yeni + R);
  float kalman_yeni = kalman_eski + (kalman_kazanci * (veri - kalman_eski));

  // Yeni standart sapmayi hesapla
  ss_yeni = (1 - kalman_kazanci) * ss_onceki;
  ss_onceki = ss_yeni;

  // Tahmin edilen degeri, bir sonraki tahmin icin hatirla
  kalman_eski = kalman_yeni;

  return kalman_yeni;
}

float kalman_eski, ss_onceki;

void loop() {

  int potvalue_normal = analogRead(A0);
  int potvalue_kalman = kalman_filter(potvalue_normal, kalman_eski, ss_onceki);

  //delay(5);
  //Serial.println(String(potvalue_kalman) + "," + String(potvalue_kalman));


  int v = potvalue_kalman;

  v = v - 90;
  v = 60 + v * 30 / 145;

 delay(10);

 //Serial.println(String(potvalue_normal) + "," + String(v));



Serial.println(String(v));

















}
