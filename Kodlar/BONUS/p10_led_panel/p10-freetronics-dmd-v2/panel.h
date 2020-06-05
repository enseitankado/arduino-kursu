#include <SPI.h>
#include "DMD2.h"
#include "fonts/Arial_Black_16.h"
#include "fonts/SystemFont5x7.h"


SoftDMD panel(PANEL_SUTUN_SAY, PANEL_SATIR_SAY); // Panel sayısı: Sutun x Satir

String son_satir1_str, son_satir2_str;

// String son_satir1_str, son_satir2_str;

void yaz(String satir2_str, String satir1_str)
{
  
  if (son_satir1_str == satir1_str && son_satir2_str == satir2_str)
    return;
    
  son_satir1_str = satir1_str;
  son_satir2_str = satir2_str;

  panel.clearScreen();
  panel.selectFont(Arial_Black_16); // System5x7, Arial_Black_16, Arial_Black_16_ISO_8859_1, Arial14
  panel.drawString(  0,  0, satir1_str, satir1_str.length());
  panel.drawString(  0,  17, satir2_str, satir2_str.length());
  
  
}


void panelTazeleyicisiniBaslat() {
  panel.setBrightness(255);
  panel.selectFont(Arial_Black_16);
  panel.begin();
}
