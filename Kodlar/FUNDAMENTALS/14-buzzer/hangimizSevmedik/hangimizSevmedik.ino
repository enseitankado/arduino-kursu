#define doo  1000// do komutu ile karismasin diye doo yazdim (kalin do)
#define re   1122
#define mi   1260
#define fa   1335
#define sol  1498
#define la   1681
#define si   1887
#define dom  2000// do komutu ile karismasin diye dom yazdim (ince do)

int melodi[] =     {doo,re,dom ,si,doo,si,doo,re,dom ,si,doo,si,doo,re,dom ,si,doo,si,  la,si,la,sol,fa,la };
int notasuresi[] = {6,  6,  2,  6, 6,  2,6,  6,  2, 6 , 6,  2,6,  6,  2,  6, 6,  2,  6,9,8,5,5,2 };// her notanin süresini array yaptim
int tempo = 1200;
void setup() 
{ 
  for (int Nota = 0; Nota <24; Nota++) 
    {
      int sure = tempo/notasuresi[Nota];
     tone(8, melodi[Nota],sure);// 8 numarali bacaktan notalarin frekanslarini nota uzunluklarina göre gönder
     delay(sure*1.2);// notalar arasinda biraz beklesin. Eger 1 ile çarparsaniz notalar birbirine bitisik olur
    }
 }
void loop() {
 }
