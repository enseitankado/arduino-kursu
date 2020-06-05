 
/*
   Bu ornek program p10 led matriks ekranlarin surulmesini orneklemek icin hazirlanmistir.

   - Panellerin veri konnektorleri out->in sirasiyla ard arda seri olarak baglandiktan sonra,
      ekrani olusturan panellerin sutun ve satir sayisi PANEL_SUTUN_SAY ve PANEL_SATIR_SAY
      degiskenleri ile tanimlanir.

   - panelTazeleyicisiniBaslat(sure) alt programı bir timer araciligi ile ekrani tazeler. 
        Parametre verilen sure kadar aralıklarla goruntu verisi panele gonderilerek
        ekran tazelenir.

   - Arduino Mega icin gerekli degisikligin detaylari:
        http://forum.freetronics.com/viewtopic.php?t=5600
        
   - Kullanilan p10 panelin baglanti sekli asagidaki gibidir:

      Arduino Pin      P10 PANEL
      -----------------------------------------
      6                A = A0 line select for two 74HC138 (4x 74HC595)
      7                B = A1 line select for two 74HC138 (4x 74HC595)
      11               R = MOSI
      9                OE = Output Enable (Ouput Enable(active low) for 74HC595
      8                STB = Latch (data latch for 74HC595)/Strobe
      13               CLK = CLOCK(shift register clock input for 74HC595)
      Gnd              GND
*/

#define PANEL_SUTUN_SAY 3
#define PANEL_SATIR_SAY 2
#include "panel.h"

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Basladi");
  panelTazeleyicisiniBaslat(2000);
  yaz("MEB.MEB.MEB.MEB", "MEB13.MEB13.MEB13.MEB13");
}

void loop(void)
{
  
  delay(100);
}
