#include <SPI.h>
#include "DMD.h"
#include "TimerOne.h"
#include "SystemFont5x7.h"
#include "Arial_black_16.h"

DMD panel(PANEL_SUTUN_SAY, PANEL_SATIR_SAY); // Panel sayısı: Sutun x Satir

String son_satir1_str, son_satir2_str;

void yaz(String satir2_str, String satir1_str)
{
  char satir1_arr[100];
  char satir2_arr[100];
  satir1_str.toCharArray(satir1_arr, satir1_str.length() + 1);
  satir2_str.toCharArray(satir2_arr, satir2_str.length() + 1);

  if (son_satir1_str == satir1_str && son_satir2_str == satir2_str)
    return;

  son_satir1_str = satir1_str;
  son_satir2_str = satir2_str;

  panel.clearScreen( true );
  panel.selectFont(Arial_Black_16); // System5x7, Arial_Black_16, Arial_Black_16_ISO_8859_1, Arial14
  panel.drawString(  0,  0, satir1_arr, satir1_str.length(), GRAPHICS_NORMAL ); // GRAPHICS_INVERSE, GRAPHICS_NORMAL
  panel.drawString(  0,  17, satir2_arr, satir2_str.length(), GRAPHICS_NORMAL ); // GRAPHICS_INVERSE, GRAPHICS_NORMAL  
}

void paneliTara()
{
  panel.scanDisplayBySPI();
}

void panelTazeleyicisiniBaslat(int tazelemePeryodu) {
  Timer1.initialize( tazelemePeryodu ); // uS
  Timer1.attachInterrupt( paneliTara );
  panel.clearScreen( true );
}
