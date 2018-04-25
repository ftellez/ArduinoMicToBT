
//Constantes Encoder
#define encoder0PinA 2
#define encoder0PinB 3
#define encoder1PinA 18
#define encoder1PinB 19
#define encoder2PinA 20
#define encoder2PinB 21


//Giro motores
int forward = 0;
int backward = 1;
int stopMotor = 2;

// Identificacion Motores
int motorA = 0;
int motorB = 1;
int motorC = 2;

//Constantes PuenteH
// motorA
int enA = 10;
int in1 = 9;
int in2 = 8;
// motorB
int enB = 5;
int in3 = 7;
int in4 = 6;
// motorC
int enC = 11;
int in5 = 13;
int in6 = 12;

volatile int encoder0Pos = 0;
volatile int encoder1Pos = 0;
volatile int encoder2Pos = 0;

// Posicion en grados a donde se quiere llegar
int posicionDeseadaMotA = 0;
int posicionDeseadaMotB = 0;
int posicionDeseadaMotC = 0;

//
bool motorAenPosicion = false;
bool motorBenPosicion = false;

bool yaDisparo = false;
bool yaDisparo2 = true;
byte messageIn[2];
byte messageOut[2];

unsigned long currentTime = 3000;
unsigned long lastShotTime = 0;

/* If pinA and pinB are both high or both low, it is spinning
   forward. If they're different, it's going backward.

   For more information on speeding up this process, see
   [Reference/PortManipulation], specifically the PIND register.
*/
void doEncoder0() {
  if (digitalRead(encoder0PinA) != digitalRead(encoder0PinB)) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }
}
void doEncoder1() {
  if (digitalRead(encoder1PinA) != digitalRead(encoder1PinB)) {
    encoder1Pos++;
  } else {
    encoder1Pos--;
  }
}
void doEncoder2() {
  if (digitalRead(encoder2PinA) != digitalRead(encoder2PinB)) {
    encoder2Pos++;
  } else {
    encoder2Pos--;
  }
}

// ******************************************
// ****************** SETUP *****************
// ******************************************
void setup() {

  //SetUp encoders
  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);       // turn on pull-up resistor
  attachInterrupt(0, doEncoder0, CHANGE);

  pinMode(encoder1PinA, INPUT);
  digitalWrite(encoder1PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder1PinB, INPUT);
  digitalWrite(encoder1PinB, HIGH);       // turn on pull-up resistor
  attachInterrupt(4, doEncoder1, CHANGE);

  pinMode(encoder2PinA, INPUT);
  digitalWrite(encoder2PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder2PinB, INPUT);
  digitalWrite(encoder2PinB, HIGH);       // turn on pull-up resistor
  attachInterrupt(3, doEncoder2, CHANGE);

  //SetUp Motores
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enC, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);

  Serial.begin (9600);

  reiniciarMotores();

  // Posiciones iniciales de los motores
  posicionDeseadaMotA = 0;
  posicionDeseadaMotB = 0;
  // 1080 grados son 3 vueltas necesarias para disparar
  posicionDeseadaMotC = 0;

}



// ******************************************
// ****************** LOOP ******************
// ******************************************
void loop() {
  if (Serial.available() > 1)
  {
    Serial.readBytes(messageIn, 2);
    posicionDeseadaMotA = (int)(messageIn[0]);
    posicionDeseadaMotB = (int)(messageIn[1]);
    currentTime = millis();
    if (currentTime > (lastShotTime + 7000)) {
      yaDisparo = true;
      lastShotTime = currentTime;
    }
  }

  // imprimeEncoder(); // debugging only
  posicionaMotor(motorA);
  posicionaMotor(motorB);
  if (motorAenPosicion && motorBenPosicion && yaDisparo) {
    posicionDeseadaMotC += 1080;
    messageOut[0] = (byte)(encoder0Pos);
    messageOut[1] = (byte)(encoder1Pos);
    yaDisparo = false;
    Serial.write(messageOut, 2);
  }
  controlaDisparador();

}

void reiniciarMotores() {
  motorACero();
  motorBCero();
  motorACero();
  motorBCero();
}
void motorACero() {
  int encoderActual = 0;
  int encoderNew = encoder0Pos;
  moveMotor(motorA, backward, 150);
  delay(50);
  moveMotor(motorA, backward, 100);
  do {
    encoderActual = encoderNew;
    delay(50);
    encoderNew = encoder0Pos;
  } while ( (encoderActual - encoderNew) > 2);
  moveMotor(motorA, backward, 50);
  delay(150);
  moveMotor(motorA, stopMotor, 100);
  delay(150);
  encoder0Pos = 0;
}

void motorBCero() {
  int encoderActual = 0;
  int encoderNew = encoder1Pos;
  moveMotor(motorB, backward, 100);
  delay(50);
  moveMotor(motorB, backward, 100);
  do {
    encoderActual = encoderNew;
    delay(50);
    encoderNew = encoder1Pos;
  } while ( (encoderActual - encoderNew) > 2);
  moveMotor(motorB, backward, 50);
  delay(150);
  moveMotor(motorB, stopMotor, 100);
  delay(150);
  encoder1Pos = 0;
}

void posicionaMotor(int iMotor) {
  int precision = 1; // Precision aceptada en grados

  // Obtener cual encoder vamos a utilizar
  int iGradosDeseados = 0;
  int iGradosActuales = 0;
  if (iMotor == motorA) {
    iGradosDeseados = posicionDeseadaMotA;
    iGradosActuales = encoder0Pos;
    motorAenPosicion = false;
  }
  else if (iMotor == motorB) {
    iGradosDeseados = posicionDeseadaMotB;
    iGradosActuales = encoder1Pos;
    motorBenPosicion = false;
  }

  int diferencia = iGradosDeseados - iGradosActuales;
  int pwm = abs(diferencia);
  pwm = pwm < 100 ? 100 : pwm;
  if (diferencia > precision) {
    moveMotor(iMotor, forward, pwm);
  }
  else if (diferencia < (-1 * precision)) {
    moveMotor(iMotor, backward, pwm);
  }
  else {
    moveMotor(iMotor, stopMotor, pwm);
    if (iMotor == motorA) {
      motorAenPosicion = true;
    }
    else if (iMotor == motorB) {
      motorBenPosicion = true;
    }
  }
}

void controlaDisparador() {
  // Diferencia en grados entre posicion deseada
  // y valor actual del encoder
  int diferencia = posicionDeseadaMotC - encoder2Pos;
  if (diferencia > 4) {
    moveMotor(motorC, forward, 230);
  }
  else if (diferencia < -4) {
    moveMotor(motorC, backward, 255);
  }
  else {
    moveMotor(motorC, stopMotor, 255);
  }
}


void moveMotor(int iMotor, int direccion, int pwm) {
  if (iMotor == motorA) {
    controlMotor(in1, in2, enA, direccion, pwm);
  }
  else if (iMotor == motorB) {
    controlMotor(in3, in4, enB, direccion, pwm);
  }
  else if (iMotor == motorC) {
    controlMotor(in5, in6, enC, direccion, pwm);
  }

}
void controlMotor (int pinMotA, int pinMotB,
                   int pinMotPwm, int direccion, int iPwm) {
  if (direccion == forward) {
    digitalWrite(pinMotA, HIGH);
    digitalWrite(pinMotB, LOW);
  }
  else if (direccion == backward) {
    digitalWrite(pinMotA, LOW);
    digitalWrite(pinMotB, HIGH);
  }
  else {
    digitalWrite(pinMotA, LOW);
    digitalWrite(pinMotB, LOW);
  }
  analogWrite(pinMotPwm, iPwm); // Set PWM value
}

void imprimeEncoder() {
  Serial.print (encoder0Pos, DEC);
  Serial.print ("\t");
  Serial.print (encoder1Pos, DEC);
  Serial.print ("\t");
  Serial.println (encoder2Pos, DEC);
}


