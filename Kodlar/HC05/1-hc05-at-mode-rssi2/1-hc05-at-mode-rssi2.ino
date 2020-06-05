/*

	Bu program HC05 modulunun arastirma modun calistirarak,
  adminMAC'in (sahip telefonun mac adresi) sinyal 
  seviyesini ekrana basar.

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

SimpleKalmanFilter kalmanFilter(1, 1, 0.01);
SoftwareSerial BTSerial(10, 11);

boolean startInPlotterMode = true, plotterMode = startInPlotterMode;

#define vcc1  6         // Supply ~40mA
#define vcc2  7         // Supply ~40mA
#define pin34  2        // HC05's key pin to switch AT mode
#define INQinterval 15  // INQuery timeout
#define buzzer 4        // Voice alert pin for RSSI threshold

boolean YeniSatir = true;
unsigned long lastMillis = 0;
String mac, adminMac = "F460:E2:BA93F9", deviceType;
byte rssi;

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
  if ( (millis() - lastMillis) > INQinterval * 1000 *1.28 ) {
    lastMillis = millis();
    sendATCmd("AT+INQ", !plotterMode);
  }
  
  if (BTSerial.available()) 
  {
    String BTReply = BTSerial.readStringUntil('\r\n');
    
    if (BTReply.substring(0, 5) == "+INQ:") { 
      parseInquiredDevice(BTReply, mac, deviceType, rssi);
      if (mac.equals(adminMac)) {
          
          printToConsole(String(rssi) + " ", true, true);

          float kalmanRSSI = kalmanFilter.updateEstimate(rssi);
          if (kalmanRSSI > 190)
            printToConsole(String((int) kalmanRSSI), true, true);

          printToConsole(String("\r\n"), true, true);
      }      
    } 
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
  sendATCmd("AT+ROLE=1", !startInPlotterMode);
  sendATCmd(String("AT+INQM=1,1000,") + String(INQinterval), !startInPlotterMode);
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
  pinMode(vcc1, OUTPUT);
  pinMode(vcc2, OUTPUT);
  pinMode(pin34, OUTPUT);
  pinMode(buzzer, OUTPUT); 
  
  digitalWrite(vcc1, LOW);
  digitalWrite(vcc2, LOW);
  digitalWrite(pin34, LOW);
  
  Serial.begin(9600);
  BTSerial.begin(38400);

  digitalWrite(pin34, HIGH);
  digitalWrite(vcc1, HIGH);
  digitalWrite(vcc2, HIGH);

  printToConsole(__DATE__ + String(", ") + __TIME__ + String(", CPU:") + F_CPU + String("Hz. \r\n"), !plotterMode, true);
  printToConsole("INFO: Arduino restarted.", !plotterMode, true);
  printToConsole("INFO: Set line ending chars of this console to NL+CR.", !plotterMode, true);
  printToConsole("INFO: To switch plotter mode send <p> command.\r\n", !plotterMode, true);
  printToConsole("Stage1: HC05 switched to AT command mode.\r\n", !plotterMode, true);
  
  // Time to switch AT Mode
  delay(500);
}
