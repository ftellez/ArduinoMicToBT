#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <math.h>

SoftwareSerial bluetooth(2, 3); // TX-O(Pin D2), RX-I (Pin D3)

String values = "";
double segundosEjec;
int ThermPin1 = 0,ThermPin2 = 1,ThermPin3 = 2,ThermPin4 = 3,PotPin =4;
int Vo1,Vo2,Vo3,Vo4,VoPot;
float R1 = 10000;
float logRA,logRB,logRC,logRD,RA,RB,RC,RD,R2A,R2B,R2C,R2D,TA,TB,TC,TD;
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float c1 = 3.354016e-03, c2 = 2.569850e-04, c3 = 2.620131e-07, c4 = 6.383091e-08;

void setup() {
  Serial.begin(57600);
  pinMode(13, OUTPUT);
  bluetooth.begin(9600);                       // Start bluetooth serial at 9600, 115200 baud rate can be too fast to relay data reliability.
  delay(10000);
}

void loop() {
  segundosEjec = hour() * 3600.0 + minute() * 60.0 + second();
  VoPot = analogRead(PotPin);
  VoPot = map(VoPot, 0, 910, 0, 100);
  
  Vo1 = analogRead(ThermPin1);
  Vo2 = analogRead(ThermPin2);
  Vo3 = analogRead(ThermPin3);
  Vo4 = analogRead(ThermPin4);
  R2A = R1 * (1023.0 / (float)Vo1 - 1.0);
  R2B = R1 * (1023.0 / (float)Vo2 - 1.0);
  R2C = R1 * (1023.0 / (float)Vo3 - 1.0);
  R2D = R1 * (1023.0 / (float)Vo4 - 1.0);
  RA = R2A / 10000.0;
  RB = R2B / 10000.0;
  RC = R2C / 10000.0;
  RD = R2D / 10000.0;
  logRA = log(RA);
  logRB = log(RB);
  logRC = log(RC);
  logRD = log(RD);
  TA = (1.0 / (c1 + c2*logRA + c3*logRA*logRA + c4*logRA*logRA*logRA));
  TB = (1.0 / (c1 + c2*logRB + c3*logRB*logRB + c4*logRB*logRB*logRB));
  TC = (1.0 / (c1 + c2*logRC + c3*logRC*logRC + c4*logRC*logRC*logRC));
  TD = (1.0 / (c1 + c2*logRD + c3*logRD*logRD + c4*logRD*logRD*logRD));
  TA = TA - 273.15;
  TB = TB - 273.15;
  TC = TC - 273.15;
  TD = TD - 273.15;
  //T = (T * 9.0)/ 5.0 + 32.0; 

  values = (String)segundosEjec + "," + (String)VoPot + "," + (String)TA + "," + (String)TB + "," + (String)TC + "," + (String)TD + ",";

  Serial.print("Segundos: ");
  Serial.println(segundosEjec);

  Serial.print("Temperature_A: "); 
  Serial.print(TA);
  Serial.println(" C"); 

  Serial.print("Temperature_B: "); 
  Serial.print(TB);
  Serial.println(" C"); 

  Serial.print("Temperature_C: "); 
  Serial.print(TC);
  Serial.println(" C"); 

  Serial.print("Temperature_D: "); 
  Serial.print(TD);
  Serial.println(" C"); 

  Serial.print("Potentiometer: ");
  Serial.println(VoPot);

  sendingSensLog(values);
//  delay(500);

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1500);              // wait for a second
}

void sendingSensLog(String sensLog) {
  const char *Log = sensLog.c_str();
  Serial.print("Bluetooth: ");
  bluetooth.write(Log);
  Serial.println(Log);
  delay(50);
}
