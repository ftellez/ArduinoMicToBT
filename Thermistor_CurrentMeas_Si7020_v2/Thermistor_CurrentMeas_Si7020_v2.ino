#include "Wire.h"
#include "Si7020.h"

String inString = "";

//----Thermistor-------------
int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR, R, R2, T;
float c1 = 3.354016e-03, c2 = 2.569850e-04, c3 = 2.620131e-07, c4 = 6.383091e-08;
//----------------------------

//----Control Mosfet---------
int potpin = 2;
//---------------------------

//----Mosfet------------------
int mosfetpin = 9;
int dutycycle = 0;
double dutywrite = 0;
long setpoint = 0;
double millisActual = 0, millisAnterior = 0;
bool onTemp = true;
bool desactVarTemp = false;
double error = 0;
//----------------------------

//----Current Measurement-----
int ledPin = 13;       // outside leads to ground and +5V
//----------------------------

//----Si7020------------------
Si7020 sensor;
//----------------------------

void setup() {
  Serial.begin(57600);
  pinMode(ledPin, OUTPUT);
  pinMode(mosfetpin,OUTPUT);
  pinMode(potpin,INPUT);
  digitalWrite(ledPin, HIGH);
  sensor.begin();
  Serial.println("Please, enter setpoint:");
  while(!Serial.available()){}
  setpoint = Serial.parseInt();
}

void loop(){
  String datos = "";
  millisActual = millis();
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  R = R2 / 10000.0;
  logR = log(R);
  T = (1.0 / (c1 + c2*logR + c3*logR*logR + c4*logR*logR*logR));
  T = T - 273.15;
  //T = (T * 9.0)/ 5.0 + 32.0; 

  datos += String(T) + ", ";
  //---------------------------------------
  
  //----------------------------------------
  // Measure RH
  double rh = sensor.getRH();
  // Measure Temperature
  double t = sensor.getTemp();
  
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement instead with readTemp()
  // float t = sensor.readTemp();
  
  datos += String(rh) + ", " + String(t) + ", ";
  //----------------------------------------
  
  //-----Potentiometer----------------------
  dutycycle = map(analogRead(potpin), 0, 345, 0, 100);
  datos += String(dutycycle) + ", "; 
  //----------------------------------------
  
  //-----Mosfet-----------------------------
//  dutywrite = (dutycycle * 255.0) / 100.0;
//  analogWrite(mosfetpin, dutywrite);
  error = setpoint - t;
  control(error);
  datos += String(setpoint) + ", " + String(t) + ", " + String(error);
  Serial.println(datos);
  //----------------------------------------

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
}

void control(double e) {
  //basic comparison in error.
  if (e > -2) {
    digitalWrite(mosfetpin, LOW);
    desactVarTemp = true;
  }
  if (e > 2) {
    if (!desactVarTemp) {
      if ((millisActual - millisAnterior) >= 10000 && onTemp) {
        onTemp = false;
        millisAnterior = millisActual;
        digitalWrite(mosfetpin, LOW);
      }
      if ((millisActual - millisAnterior) >= 5000 && !onTemp) {
        onTemp = true;
        millisAnterior = millisActual;
        digitalWrite(mosfetpin, HIGH);
      }
    } else {
      digitalWrite(mosfetpin, HIGH);
    }
  }
}
