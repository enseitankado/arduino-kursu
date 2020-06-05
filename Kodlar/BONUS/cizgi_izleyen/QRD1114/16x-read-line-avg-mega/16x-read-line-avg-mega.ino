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
#define BUTTONS_PIN A1

byte i, qrdMin = 120, qrdMax = 950;
int sensors[16], sensorsMin[16], sensorsMax[16];
unsigned long centerValue = 0;

int readQRD(byte QRD_num, bool applyKalmanFilter = false) {

  PORTB = QRD_num << 4; // PORTB.4 .. PORTB.7
  int ret = 1023 - analogRead(SB_SIG);

  if (ret > 1023)
    ret = 1023;

  if (applyKalmanFilter)
    ret = (int) kalmanFilters[QRD_num].updateEstimate(ret);

  return ret;
}

void drawPlotter() {
  String strLine = "";
  for (byte i = 0; i <= 15; i++) {
    int qrdAnalog = readQRD(i, false);
    strLine += String(qrdAnalog) + ",";
  }
  Serial.println(strLine);
  delay(1);
}

void oledBanner(String line1 = "", String line2 = "", String line3 = "") {
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tr);

  char buf1[30];
  String(line1).toCharArray(buf1, 30);
  oled.drawStr(0, 10, buf1);

  char buf2[30];
  String(line2).toCharArray(buf2, 30);
  oled.drawStr(0, 20, buf2);

  char buf3[30];
  String(line3).toCharArray(buf3, 30);
  oled.drawStr(0, 30, buf3);

  oled.sendBuffer();
}

int map_(long value, long fromLow, long fromHigh, long toLow, long toHigh) {

  // Dim %10 light
  //value = (int) value * 0.90;

  //Serial.print("Orj:" + String(value) + "->");

  // Normalize input range
  if (value > fromHigh)
    value = fromHigh;

  if (value < fromLow * 1.05) //dim %5
    value = fromLow;

  double fromRange = fromHigh - fromLow;
  double toRange = toHigh - toLow;

  double fromValuePercent = (value - fromLow) * 100 / fromRange;
  int ret = (int) (toLow + toRange / 100 * fromValuePercent);

  //Serial.println(String(ret) + " [" + String(fromLow) + "," + String(fromHigh) + "]");
  return ret;
}

unsigned long readLine(bool oledActive = false, bool debugMode = false) {

  unsigned long line = 0, avg = 0, sum = 0;

  if (oledActive)
    oled.clearBuffer();

  for (byte i = 0; i <= 15; i++) {

    sensors[i] = readQRD(i, true);
    sensors[i] = map_(sensors[i], sensorsMin[i], sensorsMax[i], 0, 1023);

    if (debugMode)
      Serial.print(String(sensors[i]) + ",");

    if (oledActive) {
      byte qrdHeight = map(sensors[i], 0, 1023, 0, 32);
      oled.drawBox(i * 8, 0, 8, qrdHeight); //x,y,w,h
    }

    avg += (unsigned long) i * 1000 * sensors[i];
    sum += (unsigned long) sensors[i];
  }
  line = (unsigned long) (avg / sum);

  if (oledActive)
    oled.sendBuffer();

  if (debugMode)
    Serial.println(" [" + String(line) + "] ");

  return line;
}

void waitForButton() {
  while (analogRead(BUTTONS_PIN) < 1000);
}

void setup() {
  oled.begin();
  Serial.begin(115200);
  pinMode(SB_S0, OUTPUT);
  pinMode(SB_S1, OUTPUT);
  pinMode(SB_S2, OUTPUT);
  pinMode(SB_S3, OUTPUT);
  pinMode(SB_SIG, INPUT);
  pinMode(BUTTONS_PIN, INPUT);

  for (byte i = 0; i <= 15; i++) {
    sensorsMin[i] = 1023;
    sensorsMax[i] = 0;
  }

  oledBanner("Press button", "to calibrate sensors.");
  waitForButton();

  // Calibrate
  oledBanner("", "", "Calibrating...");

  // Pre-store data for kalman
  for (int j = 0; j < 100; j++)
    for (byte i = 0; i <= 15; i++)
      sensors[i] = readQRD(i, true);

  int j = 0;
  while ( analogRead(BUTTONS_PIN) < 1000) {

    if ((j % 100) == 0) {
      oledBanner("", "", "Calibrating...");

      // Pre-store data for kalman
      for (int j = 0; j < 100; j++)
        for (byte i = 0; i <= 15; i++) {
          sensors[i] = readQRD(i, true);
        }
    }

    for (byte i = 0; i <= 15; i++) {
      sensors[i] = readQRD(i, true);

      if (sensors[i] < sensorsMin[i])
        sensorsMin[i] = sensors[i];

      if (sensors[i] > sensorsMax[i])
        sensorsMax[i] = sensors[i];

      byte qrdHeight = map(sensors[i], 0, 1023, 0, 32);
      oled.drawBox(i * 8, 0, 8, qrdHeight); //x,y,w,h
    }
    j++;
    oled.sendBuffer();
  }
  oledBanner("", "Calibrated...");
  delay(1000);
  oledBanner("Press button,", "centering the line", "and start robot.");
  waitForButton();
  delay(1000);
  while ( analogRead(BUTTONS_PIN) < 1000) {
    centerValue = readLine(true, true);
  }
  oledBanner("The bot Ready to start!");
}

void loop() {
  unsigned long line = readLine(false, false);  
  Serial.println(String(centerValue)  + " - " + String(line));
}
