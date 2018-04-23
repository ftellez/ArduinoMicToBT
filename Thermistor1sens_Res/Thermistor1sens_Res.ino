#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3); // TX-O(Pin D2), RX-I (Pin D3)

int ledPin = 13;       // outside leads to ground and +5V
long startTime = 0, currentTime = 0;
long cont = 0;

//----Thermistor-------------
int ThermistorPin0 = 0;
int Vo;
float R1 = 10000;
float logR, R, R2, T; 
float c1 = 3.354016e-03, c2 = 2.569850e-04, c3 = 2.620131e-07, c4 = 6.383091e-08;
//----------------------------

void setup() {
  Serial.begin(57600);
  digitalWrite(ledPin, HIGH);
  bluetooth.begin(9600);  // Start bluetooth serial at 9600, 115200 baud rate 
                          // can be too fast to relay data reliability.
  Serial.println("Enter key to start.");
  while(!Serial.available()){}
  pinMode(A3,OUTPUT);           
  startTime = millis();
}

void loop(){
  String datos = "";
  Vo = analogRead(ThermistorPin0);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  R = R2 / 10000.0;
  logR = log(R);
  T = (1.0 / (c1 + c2*logR + c3*logR*logR + c4*logR*logR*logR));
  T = T - 273.15;
  cont += 1;
  analogWrite(A3,cont);
  Serial.println(cont);
  if(cont == 255){
    cont = 0;
  }

  //Serial.println((String)T);
  currentTime = millis() - startTime;
  datos += String(T) + "," + String(currentTime/1000.0) + "\r\n";
  sendingSensLog(datos);

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
