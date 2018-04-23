#include "Wire.h"
#include "Si7020.h"

String inString = "";

//----Thermistor-------------
int ThermistorPin = A0;
int Vo;
float R1 = 10000;
float logR, R, R2, T;
float c1 = 3.354016e-03, c2 = 2.569850e-04, c3 = 2.620131e-07, c4 = 6.383091e-08;
//----------------------------

//----Control Mosfet---------
int potpin = 2;
//---------------------------

//----Mosfet Resistencia-------
int mosfetpin = 9;
long setpoint = 0;
double millisActual = 0, millisAnterior = 0;
double error = 0;
int power = 7;
//----------------------------

//----Mosfet Ventiladores------
int mosfetVpin = 10;
//-----------------------------

//----Current Measurement-----
int ledPin = 13;       // outside leads to ground and +5V
//----------------------------

//----Si7020------------------
Si7020 sensor;
//----------------------------

void setup() {
  Serial.begin(57600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(mosfetpin, OUTPUT);
  pinMode(mosfetVpin, OUTPUT);
  pinMode(potpin, INPUT);
  digitalWrite(ledPin, HIGH);
  sensor.begin();
  Serial.println("Please, enter setpoint:");
  while (!Serial.available()) {}
  setpoint = Serial.parseInt();
  analogWrite(mosfetpin,(power*255/100)); //30% approx
}

void loop() {
  String datos = "";
  millisActual = millis();

  if ((millisActual - millisAnterior) > 1200000){
    float temp = chkTemp();
    millisAnterior = millisActual;
    if (temp > 33.51) {
      power = power + ((33.5 - temp)*0.75);
      analogWrite(mosfetpin,(power*255/100));
    } else if (temp < 33.50){
      power = power + ((33.5 - temp)*0.75);
      analogWrite(mosfetpin,(power*255/100));
    } else {
      Serial.print("...el circuito encontro que el voltaje necesario es: ");
      Serial.println((power*5.0/100.0));
    }
  }

  Serial.print("Thermistor :");
  Serial.println(chkTemp());
  //datos += String(temp) + ", ";
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

  datos += String(t) + ", ";
  //----------------------------------------

  //-----Potentiometer----------------------
  //  dutycycle = map(analogRead(potpin), 0, 345, 0, 100);
  //  datos += String(dutycycle) + ", ";
  //----------------------------------------

  //-----Puente H / Peltier-----------------
  //  dutywrite = (dutycycle * 255.0) / 100.0;
  //  analogWrite(mosfetpin, dutywrite);
  error = setpoint - t;
  //basic comparison in error.
  if (error < -2) {
    Serial.println("Prende 6");
    analogWrite(5, 0);
    analogWrite(6, 255);
  }
  if (error > 2) {
    Serial.println("Prende 5");
    analogWrite(5, 125);
    analogWrite(6, 0);
  }
  datos += String(setpoint) + ", " + String(error);
  Serial.println(datos);
  //----------------------------------------

  //-----Mosfet Ventiladores----------------
  int percentage = (50 * 255) / 100;
  analogWrite(mosfetVpin, percentage);
  //----------------------------------------

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
}

float chkTemp(){
  Vo = analogRead(ThermistorPin);
  //Serial.println(Vo);
  float alpha = Vo / 1023.0;
  return (50.00 / 503.00 * (sqrt(10.00) * sqrt(3133827.00 - 1006000.00 * alpha) - 4950.00) + 24.00);
}

