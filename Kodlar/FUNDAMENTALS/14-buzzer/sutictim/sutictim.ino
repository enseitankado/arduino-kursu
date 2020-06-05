#define doo  1000// do komutu ile karismasin diye doo yazdim (kalin do)
#define re   1122
#define mi   1260
#define fa   1335
#define sol  1498
#define la   1681
#define si   1887
#define dom  2000// do komutu ile karismasin diye dom yazdim (ince do)

int melodi[] =     {re,mi,fa,sol,fa,sol,sol,sol, fa, sol,sol,sol ,fa, mi, re,mi,fa,sol,fa,sol,sol,sol, fa, sol,sol,sol ,fa, mi,   re, mi ,fa,sol,mi,fa,re,mi,doo,doo,fa,mi,re,re,re, mi ,fa,sol,mi,fa,re,mi,doo,doo,fa,mi,re,re };
int notasuresi[] = {4,  4,  4,  4, 4,  4,4,  3 ,4,4,4,4,4,2,4,  4,  4,  4, 4,  4,4,  3 ,4,4,4,4,4,2       ,4,4,4,4,4,4,4,4,4,4,4,4,2,2,    4,4,4,4,4,4,4,4,4,4,4,4,2,2};// her notanin süresini array yaptim
int tempo = 1200;
void setup() 
{ 
  for (int Nota = 0; Nota <56; Nota++) 
    {
      int sure = tempo/notasuresi[Nota];
     tone(8, melodi[Nota],sure);// 8 numarali bacaktan notalarin frekanslarini nota uzunluklarina göre gönder
     delay(sure*1.2);// notalar arasinda biraz beklesin. Eger 1 ile çarparsaniz notalar birbirine bitisik olur
    }
 }
void loop() {
 }
