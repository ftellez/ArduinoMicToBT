#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3); // TX-O(Pin D2), RX-I (Pin D3)

int ledPin = 13;       // outside leads to ground and +5V

//----Thermistor-------------
int ThermistorPin0 = 0;
int ThermistorPin1 = 1;
int ThermistorPin2 = 2;
int ThermistorPin3 = 3;
int ThermistorPin4 = 4;
int ThermistorPin5 = 5;
int Vo,Vo1,Vo2,Vo3,Vo4,Vo5;
float R1 = 10000;
float logR, R, R2, T; 
float logRA, logRB, logRC, logRD, logRE;
float RA, RB, RC, RD, RE;
float R2A, R2B, R2C, R2D, R2E; 
float TA, TB, TC, TD, TE;
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float c1 = 3.354016e-03, c2 = 2.569850e-04, c3 = 2.620131e-07, c4 = 6.383091e-08;
//----------------------------

void setup() {
  Serial.begin(57600);
  digitalWrite(ledPin, HIGH);
  bluetooth.begin(9600);  // Start bluetooth serial at 9600, 115200 baud rate 
                          // can be too fast to relay data reliability.
}

void loop(){
  String data = "";
  Vo = analogRead(ThermistorPin0);
  Vo1 = analogRead(ThermistorPin1);
  Vo2 = analogRead(ThermistorPin2);
  Vo3 = analogRead(ThermistorPin3);
  Vo4 = analogRead(ThermistorPin4);
  Vo5 = analogRead(ThermistorPin5);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  R2A = R1 * (1023.0 / (float)Vo1 - 1.0);
  R2B = R1 * (1023.0 / (float)Vo2 - 1.0);
  R2C = R1 * (1023.0 / (float)Vo3 - 1.0);
  R2D = R1 * (1023.0 / (float)Vo4 - 1.0);
  R2E = R1 * (1023.0 / (float)Vo5 - 1.0);
  R = R2 / 10000.0;
  RA = R2A / 10000.0;
  RB = R2B / 10000.0;
  RC = R2C / 10000.0;
  RD = R2D / 10000.0;
  RE = R2E / 10000.0;
  logR = log(R);
  logRA = log(RA);
  logRB = log(RB);
  logRC = log(RC);
  logRD = log(RD);
  logRE = log(RE);
  T = (1.0 / (c1 + c2*logR + c3*logR*logR + c4*logR*logR*logR));
  TA = (1.0 / (c1 + c2*logRA + c3*logRA*logRA + c4*logRA*logRA*logRA));
  TB = (1.0 / (c1 + c2*logRB + c3*logRB*logRB + c4*logRB*logRB*logRB));
  TC = (1.0 / (c1 + c2*logRC + c3*logRC*logRC + c4*logRC*logRC*logRC));
  TD = (1.0 / (c1 + c2*logRD + c3*logRD*logRD + c4*logRD*logRD*logRD));
  TE = (1.0 / (c1 + c2*logRE + c3*logRE*logRE + c4*logRE*logRE*logRE));
  T = T - 273.15;
  TA = TA - 273.15;
  TB = TB - 273.15;
  TC = TC - 273.15;
  TD = TD - 273.15;
  TE = TE - 273.15;
  //T = (T * 9.0)/ 5.0 + 32.0; 

  data += String(millis()/1000.0) + "," + String(T) + "," + String(TA) + "," + String(TB) + "," + String(TC) + "," + String(TD) + "," + String(TE) + "," + "\r\n";
  sendingSensLog(data);

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(250);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(250);              // wait for a second
}

void sendingSensLog(String sensLog) {
  const char *Log = sensLog.c_str();
  Serial.print("Bluetooth: ");
  bluetooth.write(Log);
  Serial.print(Log);
  delay(50);
}
