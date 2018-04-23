// defines pins numbers
const int stepPin = 5;
const int dirPin = 6;
int stepVelocity = 0;
char command;

void setup() {
  // Sets the two pins as Outputs
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.print("Teclea algo para iniciar.");
  while (!Serial.available()) {}
  command = Serial.read();
}
void loop() {
  if (Serial.available()) {
    command = Serial.read();
  }

  if(
  

  if (command != 's') {
    //ATRAS
    //  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
    //  // Makes 200 pulses for making one full cycle rotation
    //  for(int x = 0; x < 200; x++) {
    //    digitalWrite(stepPin,HIGH);
    //    delayMicroseconds(5000);
    //    digitalWrite(stepPin,LOW);
    //    delayMicroseconds(100);
    //  }
    //  delay(1000); // One second delay
    // ADELANTE
    digitalWrite(dirPin, LOW); //Changes the rotations direction
    // Makes 400 pulses for making two full cycle rotation
    for (int x = 0; x < 400; x++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(stepVelocity);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(stepVelocity);
    }
    //  delay(100);
  }
}
