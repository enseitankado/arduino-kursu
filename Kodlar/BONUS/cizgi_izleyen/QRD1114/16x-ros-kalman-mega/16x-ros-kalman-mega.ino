#include <Arduino.h>
#include <U8g2lib.h>
#include "SimpleKalmanFilter.h"
#include <SPI.h>
#include <Wire.h>

/*
    BOARD       ARDUINO MEGA!
    -------------------------
    S0          D10
    S1          D11
    S2          D12
    S3          D13
    SIG         A0

*/
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C oled(U8G2_R0);

/*
   Kalman filter is an optimal estimation algorithm.
   https://github.com/denyssene/SimpleKalmanFilter
      SimpleKalmanFilter(e_mea, e_est, q);
        e_mea: Gurultu miktari
        e_est: Tahmin araligi
        q: 1 ile 0.001 arasinda olcumun hareket hizi. Tavsiye edilen 0.01
*/

//const size_t  = 16;
float e_mea = 3, e_est = e_mea, q = 0.08;

SimpleKalmanFilter kalmanFilters[16] = {
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q),
  SimpleKalmanFilter(e_mea, e_est, q)
};

// SB = Sensor Board Pins. PORTB.4 .. PORTB.7
#define SB_S0  10
#define SB_S1  11
#define SB_S2  12
#define SB_S3  13
#define SB_SIG  A0

int readQRD(byte QRD_num, bool applyKalmanFilter = false) {

  PORTB = QRD_num << 4; // PORTB.4 .. PORTB.7
  int ret = 1023 - analogRead(SB_SIG);

  if (ret > 1023)
    ret = 1023;

  if (applyKalmanFilter)
    ret = (int) kalmanFilters[QRD_num].updateEstimate(ret);

  return ret;
}

byte i;

void setup() {
  oled.begin();
  Serial.begin(115200);
  pinMode(SB_S0, OUTPUT);
  pinMode(SB_S1, OUTPUT);
  pinMode(SB_S2, OUTPUT);
  pinMode(SB_S3, OUTPUT);
  pinMode(SB_SIG, INPUT);
}

void loop() {

  /* PLOTTER CALIBRATION */
  /*
  String strLine = "";
  for (i = 0; i <= 15; i++) {
    int qrdAnalog = readQRD(i, false);
    strLine += String(qrdAnalog) + ",";
  }
  Serial.println(strLine);
  delay(1);
  */

    oled.clearBuffer();
    for (byte i = 0; i <= 15; i++) {
      int qrdAnalog = readQRD(i, false);
      byte qrdHeight = map(qrdAnalog, 120, 950, 0, 32);
      oled.drawBox(i * 8, 0, 8, qrdHeight); //x,y,w,h
    }
    oled.sendBuffer();
    delay(1);
 
}
