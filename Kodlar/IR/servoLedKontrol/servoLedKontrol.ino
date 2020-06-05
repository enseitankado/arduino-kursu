#include <IRremote.h>  // kızıl ötesi alıcı için gerekli olan kütüphane
#include <Servo.h>    // kapıyı açma kapama için kullandığımız servo motoru kullanabilmek için gerekli olan kütüphane

Servo myservomotor;   // sınıfın bir örneğini aldık
int INFRARED_PIN = 2;   // Arduino nun 2 nolu pini alıcının girişi olarak belirledik
int LED_PIN = 10;   //Arduino nun 10 nolu pini led için çıkış belirledik
IRrecv irrecv(INFRARED_PIN);  // belirlediğimiz 2 nolu pini kızılötesi alıcıdan değerleri oku dedik.

decode_results results;             // ** results değerinde kumandadan bir tuşa bastığımızda alıcının algıladığı değeri okuyacağız. **
void setup()
{
Serial.begin(9600);    // saniyede 9600 defa okuma işlemi yap.
irrecv.enableIRIn();  // alıcıdan değer okumaya başla.
myservomotor.attach(8);   // Arduinonun 8. pinini servo motor için çıkış yaptık.
myservomotor.write(0);    // başlangıçta açısını 0(sıfır) a çektik. (Kapı Kapalı durumu)
}
void loop() {
if (irrecv.decode(&results)) {  // Eğer bir değer geldiyse
Serial.println(results.value);  // Değeri serial monitöre yansıt. Buradan basılan her tuşun ürettiği değeri görebiliriz. Böylece diğer değerleride if ya da  switch case ye bağlayıp  çeşitli komutla üretebilirsiniz.
irrecv.resume();  // Sonraki değeri alamaya devam et.
  }

// Kumandanın 1 nolu tuşu 167241175 değerinde bir değer ürettiği için gelen değer bu değere eşit mi diyoruz.

// Kumanda direk alıcıyı gördüğü yerde bu değeri üretir. alıcı ile kumandanın vericisi birbiriniz görmesede bir değer üretir ama bunlar sürekli değişir.

// Direk gördüğünde ise tek bir değer üretir. Biz bu değeri baz alıyoruz. Farklı değerler üretmesiniz sebebibe gelen ışık dalgalarında bozulmalar olmasıdır.

 if("DEA5F137"==results.value)      
{
Serial.println("1");    // 1 tuşuna basıldıysa ekrana 1 yazdır.
digitalWrite ( LED_PIN , HIGH );    // led i yak
myservomotor.write(90);   // Servo motoru 90 derecelik açı yap. (kapıyı aç)

results.value=0;   // Kumandaya basıldığında algılanan değer results.value de kalır. sürekli aynı değer okunmaya devam etmesin diye o(sıfıra) eşitliyoruz. bunu yapmazsak serial monitörde hep 16724175 değerini bastığını görürsünüz.

 }
 else if("FF18E7"==results.value)   //eğer 2 tuşuna basıldıysa
{
Serial.println("2");    // 2 tuşuna basıldıysa ekrana 2 yazdır.
digitalWrite ( LED_PIN , LOW );    // led i söndür
myservomotor.write(0);     // Servo motoru 0. derece açıya çek. (kapıyı kapat)
results.value=0;
   }
}
