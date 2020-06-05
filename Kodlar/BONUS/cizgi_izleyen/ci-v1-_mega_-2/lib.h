#define LION_CELL_FULL_V 4.2 	//4.20V Maximum
#define LION_CELL_EMPTY_V 3.0 	//3.00V Cutoff
#define MEGA_ADC_VREF 5.01

float BATT_M_V_PROPORTION;
float BATT_A_V_PROPORTION;
float MOTOR_V_PROPORTION;

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

void motorsSpeed(byte A_PWM, byte B_PWM) {
  analogWrite(MOTORA_EN, A_PWM);
  analogWrite(MOTORB_EN, B_PWM);

Serial.println(", aPWM:" + String(A_PWM) + ", bPWM:" + String(B_PWM));
}

void botDirection(byte motorDirection) {

  if (motorDirection == M_FORWARD) {
    digitalWrite(L298_IN1 , 0);
    digitalWrite(L298_IN2 , 1);
    digitalWrite(L298_IN3 , 1);
    digitalWrite(L298_IN4 , 0);
  }

  if (motorDirection == M_BACKWARD) {
    digitalWrite(L298_IN1 , 1);
    digitalWrite(L298_IN2 , 0);
    digitalWrite(L298_IN3 , 0);
    digitalWrite(L298_IN4 , 1);
  }

  if (motorDirection == M_LEFT) {
    digitalWrite(L298_IN1 , 1);
    digitalWrite(L298_IN2 , 0);
    digitalWrite(L298_IN3 , 1);
    digitalWrite(L298_IN4 , 0);
  }

  if (motorDirection == M_HALF_LEFT) {
    digitalWrite(L298_IN1 , 1);
    digitalWrite(L298_IN2 , 0);
    digitalWrite(L298_IN3 , 0);
    digitalWrite(L298_IN4 , 0);
  }

  if (motorDirection == M_RIGHT) {
    digitalWrite(L298_IN1 , 0);
    digitalWrite(L298_IN2 , 1);
    digitalWrite(L298_IN3 , 0);
    digitalWrite(L298_IN4 , 1);
  }

  if (motorDirection == M_HALF_RIGHT) {
    digitalWrite(L298_IN1 , 0);
    digitalWrite(L298_IN2 , 0);
    digitalWrite(L298_IN3 , 0);
    digitalWrite(L298_IN4 , 1);
  }

  if (motorDirection == M_STOP) {
    motorsSpeed(0, 0);
    digitalWrite(L298_IN1 , 0);
    digitalWrite(L298_IN2 , 0);
    digitalWrite(L298_IN3 , 0);
    digitalWrite(L298_IN4 , 0);
  }
}

void beep(int freq, int beepDelay) {
  analogWrite(BUZZER_PIN, freq);
  delay(beepDelay);
  analogWrite(BUZZER_PIN, 0);
}

void multiBeep(byte beepCount, byte lengthFactor = 1) {
  byte beepPeriod = 100;
  for (byte i = 0; i < beepCount; i++) {
    beep(1000, beepPeriod * lengthFactor);
    delay(beepPeriod * lengthFactor);
  }
}

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
  float Q = 0.001;
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

int readQRD(byte QRD_num, bool applyKalmanFilter = false) {

  PORTB = QRD_num << 4; // MEGA => PORTB.4 .. PORTB.7
  int ret = 1023 - analogRead(SB_SIG_PIN);

  if (ret > 1023)
    ret = 1023;

  if (applyKalmanFilter)
    ret = (int) kalmanFilters[QRD_num].updateEstimate(ret);

  return ret;
}

void calculateVoltageDividers() {

  // Voltaj bolucu direnclerin degerleri
  float BATT_M_R1 = 9.96, BATT_M_R2 = 46.35;
  float BATT_A_R1 = 9.80, BATT_A_R2 = 10.3;
  float MOTOR_R1 = 9.84, MOTOR_R2 = 46.4;

  BATT_M_V_PROPORTION = BATT_M_R1 * 100 / (BATT_M_R1 + BATT_M_R2) / 1.083;
  BATT_A_V_PROPORTION = BATT_A_R1 * 100 / (BATT_A_R1 + BATT_A_R2) / 1.060;
  MOTOR_V_PROPORTION = MOTOR_R1 * 100 / (MOTOR_R1 + MOTOR_R2) / 1.045;
}

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void meausureVoltages() {
  // Yap: Mega'nin dahili 1v1 veya 2v56 referans uretecini kullan.
  float BATT_MAIN_KALMAN = kalman_filter(analogRead(BATT_MAIN_PIN), volts_old[0], volts_ss[0]);
  float BATT_ARDU_KALMAN = kalman_filter(analogRead(BATT_ARDU_PIN), volts_old[1], volts_ss[1]);
  float VOLT_MOTOR_KALMAN = kalman_filter(analogRead(VOLT_MOTOR_PIN), volts_old[2], volts_ss[2]);

  volts[0] = BATT_MAIN_KALMAN * MEGA_ADC_VREF / 1023 / BATT_M_V_PROPORTION * 100;
  volts[1] = BATT_ARDU_KALMAN * MEGA_ADC_VREF / 1023 / BATT_A_V_PROPORTION * 100;
  volts[2] = VOLT_MOTOR_KALMAN * MEGA_ADC_VREF / 1023 / MOTOR_V_PROPORTION * 100;

  if (volts[0] > (LION_CELL_EMPTY_V * 4))
    volts[0] = mapf(volts[0], (LION_CELL_EMPTY_V * 4), (LION_CELL_FULL_V * 4), 0, 100);
  else
    volts[0] = 0;

  if (volts[1] > (LION_CELL_EMPTY_V * 2))
    volts[1] = mapf(volts[1], (LION_CELL_EMPTY_V * 2), (LION_CELL_FULL_V * 2), 0, 100);
  else
    volts[1] = 0;
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

unsigned long readLine(bool oledActive = false, bool debugMode = false, bool kalmanEnable = false) {

  unsigned long line = 0, avg = 0, sum = 0;

  if (oledActive)
    oled.clearBuffer();

  for (byte i = 0; i <= 15; i++) {

    sensors[i] = readQRD(i, kalmanEnable);
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

  line = map(line, 0, 16000, 0, 255);

  if (oledActive)
    oled.sendBuffer();

  if (debugMode)
    Serial.println(" [" + String(line) + "] -> ["+String(targetPosition)+"]");

  return line;
}

void waitForButton() {
  while (analogRead(BUTTONS_PIN) < 1000);
}

void calibrateSensors(int swingCount, int &QRDMin, int &QRDMax)
{
  if (swingCount == 0)
    motorsSpeed(0, 0);
  else
    motorsSpeed(40, 40);

  for (int i = 0; i < swingCount; i++) {
    // Sola dogru tara
    botDirection(M_LEFT);

    // Saga dogru tara
    botDirection(M_RIGHT);

    // Sola dogru tara
    botDirection(M_LEFT);
  }

  // Motorlari durdur
  botDirection(M_STOP);
}

void minMaxCalibration(bool kalmanEnable = false) {

  // Calibrate
  oledBanner("", "", "Calibrating...");

  for (byte i = 0; i <= 15; i++) {
    sensorsMin[i] = 1023;
    sensorsMax[i] = 0;
  }

  // Pre-store data for kalman filter
  for (int j = 0; j < 100; j++)
    for (byte i = 0; i <= 15; i++)
      sensors[i] = readQRD(i, kalmanEnable);

  int j = 0;
  while ( analogRead(BUTTONS_PIN) < 1000) {

    if ((j % 100) == 0) {
      oledBanner("", "", "Calibrating...");

      // Pre-store data for kalman
      for (int j = 0; j < 100; j++)
        for (byte i = 0; i <= 15; i++) {
          sensors[i] = readQRD(i, kalmanEnable);
        }
    }

    for (byte i = 0; i <= 15; i++) {
      sensors[i] = readQRD(i, kalmanEnable);

      if (sensors[i] < sensorsMin[i])
        sensorsMin[i] = sensors[i];

      if (sensors[i] > sensorsMax[i])
        sensorsMax[i] = sensors[i];

      // Homojen olmayan desenli zeminler icin min degerini artir.
      sensorsMin[i] = (int) (sensorsMin[i] + sensorsMin[i] /100 * MIN_SENSOR_TOLERANCE);

      byte qrdHeight = map(sensors[i], 0, 1023, 0, 32);
      oled.drawBox(i * 8, 0, 8, qrdHeight); //x,y,w,h
    }
    j++;
    oled.sendBuffer();
  }  
}

void updateDisplay() {

  oled.clearBuffer();

  for (byte i = 0; i <= 15; i++) {
    int qrdAnalog = readQRD(i, false);
    byte qrdHeight = map(qrdAnalog, 120, 950, 0, 32 - 11);
    oled.drawBox(i * 8, 11, 8, qrdHeight); //x,y,w,h
  }

  char buf[30];
  String(String((int) volts[2]) + "v, CAP:%" + String((int)volts[0]) + " %" + String((int)volts[1])).toCharArray(buf, 30);
  oled.setFont(u8g2_font_5x8_tr);
  oled.drawStr(0, 10, buf);

  oled.sendBuffer();
}

void displayBatteryStatus() {

  oledBanner("Checking batteries...");
  calculateVoltageDividers();

  for (int i = 0; i < 2000; i++)
    meausureVoltages();

  oledBanner(String((int) volts[2]) + "v, CAP:%" + String((int)volts[0]) + " %" + String((int)volts[1]));
}
