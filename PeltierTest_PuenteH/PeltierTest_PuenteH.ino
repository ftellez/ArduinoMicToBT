int millisAnterior = 0;
int millisActual = 0;
bool sw = true;
int percentage;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.println("Introduce potencia de peltier: ");
  while (!Serial.available()) {}
  percentage = Serial.parseInt();
  analogWrite(5, 0);
  analogWrite(6, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  millisActual = millis();
  int calc;
  if ((millisActual - millisAnterior) > 15000 && sw) {
    sw = false;
    millisAnterior = millisActual;
    Serial.println("Encendiendo 5 y apagando 6");
    calc = percentage * 255 / 100;
    analogWrite(5, calc);
    analogWrite(6, 0);
    Serial.println(calc);
  }
  if ((millisActual - millisAnterior) > 5000 && !sw) {
    sw = true;
    millisAnterior = millisActual;
    Serial.println("Encendiendo 6 y apagando 5");
    calc = percentage * 255 / 100;
    analogWrite(6, calc);
    analogWrite(5, 0);
    Serial.println(calc);
  }
}
