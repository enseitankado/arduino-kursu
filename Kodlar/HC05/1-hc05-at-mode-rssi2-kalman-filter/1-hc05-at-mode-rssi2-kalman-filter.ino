/*

	Bu program HC05 modulunun arastirma moduna alarak
	RSSI sonuclarini daha stabil sonuclar icin
	kalman filtresinden gecirir.

	Devre şeması şu şekildedir:

	Bluetooth   Arduino
	------------------
	RX          =>  11 (Tx)
	TX          =>  10 (Rx)
    Vcc         =>  6+7 (power over GPIO)
    Key (pin34) =>  2 (Modulun key pini yoksa Pin34'e [PIO11] bir kablo lehimleyin)


	Temmuz 2018
	Özgür Koca
*/
#include <SimpleKalmanFilter.h>
#include <SoftwareSerial.h>

/*
 * Kalman filter is an optimal estimation algorithm. Install from "Manage libraries...". 
 * https://github.com/denyssene/SimpleKalmanFilter
 * SimpleKalmanFilter(e_mea, e_est, q);     
 * e_mea: Measurement Uncertainty - How much do we expect to our measurement vary    
 * e_est: Estimation Uncertainty - Can be initilized with the same value as e_mea since the kalman filter will adjust its value.    
 * q: Process Variance - usually a small number between 0.001 and 1 - how fast your measurement moves. Recommended 0.01. Should be tunned to your needs.
 */
SimpleKalmanFilter kalmanFilter(5, 5, 0.2);
SoftwareSerial BTSerial(10, 11);

#define RSSI_HC05_Vcc1     6        // RSSI Meter (HC05) Vcc1; Supply ~40mA
#define RSSI_HC05_Vcc2     7        // RSSI Meter (HC05) Vcc2; Supply ~40mA
#define RSSI_HC05_KeyPin   2        // RSSI Meter (HC05) key pin; HC05's key pin to switch AT mode
#define RSSI_HC05_INQinterval   15  // INQuery timeout = INQinterval * 1.28 Secs
#define buzzer 4                    // Voice alert pin for RSSI threshold

/* Configure plooter mode at starer */
boolean startInPlotterMode = true, plotterMode = startInPlotterMode;

boolean YeniSatir = true;
unsigned long lastMillis = 0;
String mac, adminMac = "F460:E2:BA93F9", deviceType, plotterData;
byte rssi, kalmanInitCount = 5, initialized = 0;
float kalmanRSSI;

void initHC05();
void sendATCmd(String atCommand,  bool printOut = true);
void printToConsole(String str, bool serialConsole = true, bool bluetoothConsole = true);

void setup()
{
  initHC05();
  setupINQ();
}

void loop()
{  
  // Detect INQM timeout
  if ( (millis() - lastMillis) > RSSI_HC05_INQinterval * 1000 *1.28 ) {
    lastMillis = millis();
    sendATCmd("AT+INQ", !plotterMode);
  }
  
  if (BTSerial.available()) 
  {
    String BTReply = BTSerial.readStringUntil('\r\n');

    // Detect INQery results and print optimized RSSI
    if (BTReply.substring(0, 5) == "+INQ:") { 
      
      parseInquiredDevice(BTReply, mac, deviceType, rssi);
      if (mac.equals(adminMac)) {  
         
          /* Apply kalman filter to remove transient gain fluctuations.
           *  
           *  First initialize the filter with first of number of kalmanInitCount  
           *  before draw on plotter otherwise the kalmanFilter results 
           *  starts from "0" to adapt own completly. it should be fed with 
           *  a sufficient number (kalmanInitCount) of values for best prediction results.
           */
          if (initialized >= kalmanInitCount) {
            kalmanRSSI = kalmanFilter.updateEstimate(rssi);
            plotterData = String(rssi) + " " + String(kalmanRSSI);            
          } else {
            kalmanRSSI = kalmanFilter.updateEstimate(rssi);            
            plotterData = String(rssi);
            initialized++;
          }
          
          printToConsole(plotterData+ "\r\n", true, true);
      }      
    } 
    
    // Otherwise print bluetooth replies if plotter mode off.
    else 
      printToConsole(BTReply, !plotterMode, true);
  }

  if (Serial.available())
    processSerialConsoleInput();
}

void setupINQ() {
  sendATCmd("AT+NAME?", !startInPlotterMode);
  sendATCmd("AT+UART=38400,0,0", !startInPlotterMode);
  sendATCmd("AT+INIT", !startInPlotterMode);
  sendATCmd("AT+IAC=9E8B33", !startInPlotterMode);
  sendATCmd("AT+CLASS=0", !startInPlotterMode);
  sendATCmd("AT+CLASS=0", !startInPlotterMode);
  sendATCmd(String("AT+INQM=1,1000,") + String(RSSI_HC05_INQinterval), !startInPlotterMode);
}

void parseInquiredDevice(String INQReply, String &mac, String &deviceClass, byte &rssi) {    
    // Format => +INQ:8EF:3B:258C20,240428,FFCA
    int index1 = INQReply.indexOf(',');  
    mac = INQReply.substring(5, index1);
    int index2 = INQReply.indexOf(',', index1+1 );
    deviceClass = INQReply.substring(index1+1, index2);
    String deviceRSSI = INQReply.substring(index2+1);
    char bf[4];
    deviceRSSI.substring(2,4).toCharArray(bf, 4);
    rssi = strtol(bf,0,16);
}

void printToConsole(String str, bool serialConsole = true, bool bluetoothConsole = true) {
  if (serialConsole)
    Serial.print(str);
}

void sendATCmd(String atCommand,  bool printOut = true) {
  
  printToConsole( String(">" + atCommand + "\r\n"), printOut, true);  
    
  BTSerial.print( String(atCommand + "\r\n") );
  delay(300);
  if (printOut)
    printToConsole(BTSerial.readString(), printOut, true);
}

void processSerialConsoleInput() {
  
    String input = Serial.readStringUntil('\r\n');
    
    if (input.substring(0, 2).equals("AT"))
    BTSerial.println(input);
    
    printToConsole(String(">") + input + String("\r\n"), !plotterMode, true);

    // Switch plotter mode
    input.trim();
    if (input.equals("p")) {
      if (plotterMode == true) {
        printToConsole("Plotter mode: Off\r\n", !plotterMode, true);
        plotterMode = false;        
      } else {
        plotterMode = true;
        printToConsole("Plotter mode: On\r\n", !plotterMode, true);
      }
      delay(500);
    }
}

void initHC05() {
  pinMode(RSSI_HC05_Vcc1, OUTPUT);
  pinMode(RSSI_HC05_Vcc2, OUTPUT);
  pinMode(RSSI_HC05_KeyPin, OUTPUT);
  pinMode(buzzer, OUTPUT); 
  
  digitalWrite(RSSI_HC05_Vcc1, LOW);
  digitalWrite(RSSI_HC05_Vcc2, LOW);
  digitalWrite(RSSI_HC05_KeyPin, LOW);
  
  Serial.begin(9600);
  BTSerial.begin(38400);

  digitalWrite(RSSI_HC05_KeyPin, HIGH);
  digitalWrite(RSSI_HC05_Vcc1, HIGH);
  digitalWrite(RSSI_HC05_Vcc2, HIGH);

  printToConsole(__DATE__ + String(", ") + __TIME__ + String(", CPU:") + F_CPU + String("Hz. \r\n"), !plotterMode, true);
  printToConsole("INFO: Arduino restarted.\r\n", !plotterMode, true);
  printToConsole("INFO: Set line ending chars of this console to NL+CR.\r\n", !plotterMode, true);
  printToConsole("INFO: To switch plotter mode send <p> command.\r\n", !plotterMode, true);
  printToConsole("Stage1: HC05 switched to AT command mode.\r\n", !plotterMode, true);
  
  // Time to switch AT Mode
  delay(500);
}
