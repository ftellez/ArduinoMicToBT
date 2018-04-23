int ThermistorPin = 0;
int Vo;
float R1 = 10000;
float logR, R, R2, T;
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
float c1 = 3.354016e-03, c2 = 2.569850e-04, c3 = 2.620131e-07, c4 = 6.383091e-08;

void setup() {
  Serial.begin(57600);
  pinMode(13, OUTPUT);
}

void loop() {
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  R = R2 / 10000.0;
  logR = log(R);
  T = (1.0 / (c1 + c2*logR + c3*logR*logR + c4*logR*logR*logR));
  T = T - 273.15;
  //T = (T * 9.0)/ 5.0 + 32.0; 

  Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.println(" C"); 

//  delay(500);

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
