#include <Arduino.h>
#include <U8g2lib.h>
#include "SimpleKalmanFilter.h"
#include <SPI.h>
#include <Wire.h>
#include "PID_v1.h"

#define DEBUG_MODE true

// SB = Sensor Board Pins. PORTB.4 .. PORTB.7
#define SB_S0_PIN   10
#define SB_S1_PIN   11
#define SB_S2_PIN   12
#define SB_S3_PIN   13
#define SB_SIG_PIN  A0
#define BUTTONS_PIN A1

#define BATT_MAIN_PIN A15
#define BATT_ARDU_PIN A13
#define VOLT_MOTOR_PIN A14
#define BUTTONS_PIN A1
#define BUZZER_PIN 8
#define MOTORA_EN 2
#define MOTORB_EN 7
#define L298_IN1 3
#define L298_IN2 4
#define L298_IN3 5
#define L298_IN4 6

#define M_STOP 0
#define M_FORWARD 1
#define M_BACKWARD 2
#define M_LEFT 3
#define M_RIGHT 4
#define M_HALF_RIGHT 5
#define M_HALF_LEFT 6
#define MIN_PWM 30
#define MAX_PWM 50
#define MIN_SENSOR_TOLERANCE 20

bool kalman;
int sensors[16], sensorsMin[16], sensorsMax[16];
float volts[3], volts_old[3], volts_ss[3];
double currentPosition;
double targetPosition = 0;

// Define 128x32 OLED Display
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C oled(U8G2_R0);

#include "lib.h"

double aPWM = MAX_PWM, bPWM = MAX_PWM;

double Kp = 0.5, Ki = 0.001, Kd = 0;

double pidOutput;

PID botPID(&currentPosition, &pidOutput, &targetPosition, Kp, Ki, Kd, DIRECT);


void setup() {
  oled.begin();
  Serial.begin(115200);

  pinMode(SB_S0_PIN, OUTPUT);
  pinMode(SB_S1_PIN, OUTPUT);
  pinMode(SB_S2_PIN, OUTPUT);
  pinMode(SB_S3_PIN, OUTPUT);
  pinMode(SB_SIG_PIN, INPUT);
  pinMode(BUTTONS_PIN, INPUT);

  pinMode(BATT_MAIN_PIN, INPUT);
  pinMode(BATT_ARDU_PIN, INPUT);
  pinMode(VOLT_MOTOR_PIN, INPUT);

  pinMode(L298_IN1, OUTPUT);
  pinMode(L298_IN2, OUTPUT);
  pinMode(L298_IN3, OUTPUT);
  pinMode(L298_IN4, OUTPUT);

  /*
    botDirection(M_FORWARD);
    motorsSpeed(MIN_PWM, MIN_PWM);
    while (1 == 1);
  */

  //displayBatteryStatus();
  delay(2000);
  minMaxCalibration(kalman = true);
  oledBanner("", "Calibrated...");
  multiBeep(1, 1);

  // Line centering
  while (analogRead(BUTTONS_PIN) < 1000) {
    targetPosition = readLine(true, false, kalman = true);
  }
  oledBanner("Bot Ready to start!");
  multiBeep(3, 1);

  botPID.SetMode(AUTOMATIC);
  botPID.SetOutputLimits(-(MAX_PWM - MIN_PWM), MAX_PWM - MIN_PWM);
  botPID.SetSampleTime(10);
}

void loop() {
  currentPosition = readLine(true, false, kalman = true);
  Serial.print("Pos:" + String(currentPosition));

  if (currentPosition == 0) {
    botDirection(M_STOP);
  } else {

    botPID.Compute();
    Serial.print(" Ham:" + String(pidOutput));

    if (pidOutput < 0) {
      aPWM = (byte) MAX_PWM + pidOutput;
      bPWM = MAX_PWM;
    }
    else if (pidOutput > 0) {
      aPWM = MAX_PWM;
      bPWM = (byte) MAX_PWM - pidOutput;
    }
    // No-error
    else
    {
      aPWM = MAX_PWM;
      bPWM = MAX_PWM;
    }

    //bPWM = bPWM * 520/470;

    botDirection(M_FORWARD);
    motorsSpeed(aPWM, bPWM);

    
    byte _a = aPWM, _b = bPWM;
    
    oled.setFont(u8g2_font_6x10_tr);
    String line1 = String(_b) + "   " + String(_a);
    char buf1[30];
    String(line1).toCharArray(buf1, 30);
    oled.drawStr(0, 10, buf1);
    oled.sendBuffer();


    //Serial.println(String(aPWM) + "," + String(bPWM));
    //Serial.println(", aPWM:" + String((byte)aPWM) + ", bPWM:" + String((byte)bPWM));
  }
}
