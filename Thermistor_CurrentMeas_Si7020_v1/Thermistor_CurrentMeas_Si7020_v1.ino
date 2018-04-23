#include "Wire.h"
#include "Si7020.h"
//#include "string.h"

//----Thermistor-------------
int ThermistorPin = 0, ThermistorPin1 = 1;
int Vo,Vo1;
float R1 = 10000, RA1 = 10000;
float logR, R, R2, T, logRA, RA, R2A, TA;
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float c1 = 3.354016e-03, c2 = 2.569850e-04, c3 = 2.620131e-07, c4 = 6.383091e-08;
//----------------------------

//----Control Mosfet---------
int potpin = 2;
//---------------------------

//----Mosfet------------------
int mosfetpin = 9;
int dutycycle = 0;
double dutywrite = 0;
//----------------------------

//----Current Measurement-----
int analogPin = 3;     // potentiometer wiper (middle terminal) connected to analog pin 3
int ledPin = 13;       // outside leads to ground and +5V
int val = 0;           // variable to store the value read
float current = 0;
//----------------------------

//----Si7020------------------
Si7020 sensor;
//----------------------------

void setup() {
  Serial.begin(57600);
  pinMode(analogPin,INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(mosfetpin,OUTPUT);
  pinMode(potpin,INPUT);
  digitalWrite(ledPin, HIGH);
  sensor.begin();
}

void loop(){
  String data = "";
  Vo = analogRead(ThermistorPin);
  Vo1 = analogRead(ThermistorPin1);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  R2A = RA1 * (1023.0 / (float)Vo1 - 1.0);
  R = R2 / 10000.0;
  RA = R2A / 10000.0;
  logR = log(R);
  logRA = log(RA);
  T = (1.0 / (c1 + c2*logR + c3*logR*logR + c4*logR*logR*logR));
  TA = (1.0 / (c1 + c2*logRA + c3*logRA*logRA + c4*logRA*logRA*logRA));
  T = T - 273.15;
  TA = TA - 273.15;
  //T = (T * 9.0)/ 5.0 + 32.0; 

  //Serial.print("Temperature Thermistor1: "); 
  //Serial.print(T);
  //Serial.println(" C"); 

  //Serial.print("Temperature Thermistor2: "); 
  //Serial.print(TA);
  //Serial.println(" C"); 
  data += String(T) + ", " + String(TA) + ", ";

  //---------------------------------------
  val = analogRead(analogPin);    // read the input pin
//  Serial.print("ADC value: ");
//  Serial.println(val);
//  Serial.print("Converted val: ");
//  Serial.println((5.0*(val/1024.0)));
  current = 1000.0*(5.0*(val/1024.0));
  //current = 0.1*(5.0*(val/1024.0))/(.001*0.1*1000.0);
  //Serial.print("Corriente (mA): ");
  //Serial.println(current,3);             // debug value
//  Serial.println();
  data += String(current) + ", ";

  //----------------------------------------
  // Measure RH
  float rh = sensor.getRH();
  // Measure Temperature
  float t = sensor.getTemp();
  
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement instead with readTemp()
  // float t = sensor.readTemp();
  
  // Print the result in the JSON format
  //Serial.print("{\"rh\":\"");
  //Serial.print(rh,1);
  //Serial.print("\",\"temp\":\"");
  //Serial.print(t,2);
  //Serial.println("\"}"); 
  //Serial.println();
  data += String(rh) + ", " + String(t);
  Serial.println(data);
  //----------------------------------------

  //-----Potentiometer----------------------
  dutycycle = map(analogRead(potpin), 0, 900, 0, 99); 
  //----------------------------------------

  //-----Mosfet-----------------------------
  dutywrite = (dutycycle * 255.0) / 100.0;
  analogWrite(mosfetpin, dutywrite);
  //----------------------------------------
  //delay(1000);  

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
}
