#include <TimeLib.h>

//------Thermistors-------------------
String values = "";
double segundosEjec;
int ThermPin1 = 0;
int Vo1;
float R1 = 10000;
float logRA,RA,R2A,TA;
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float c1 = 3.354016e-03, c2 = 2.569850e-04, c3 = 2.620131e-07, c4 = 6.383091e-08;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print(0);  // To freeze the lower limit
  Serial.print(" ");
  Serial.print(50);  // To freeze the upper limit
  Serial.print(" ");
}

void loop() {
  // put your main code here, to run repeatedly:
  segundosEjec = hour() * 3600.0 + minute() * 60.0 + second();
  Vo1 = analogRead(ThermPin1);
  R2A = R1 * (1023.0 / (float)Vo1 - 1.0);
  RA = R2A / 10000.0;
  logRA = log(RA);
  TA = (1.0 / (c1 + c2*logRA + c3*logRA*logRA + c4*logRA*logRA*logRA));
  TA = TA - 273.15;

  /*Serial.print("Segundos: ");
  Serial.println(segundosEjec);*/

  //Serial.print("Temperature_A: "); 
  Serial.println(TA);
  //Serial.println(" C"); 

  delay(100);
}
