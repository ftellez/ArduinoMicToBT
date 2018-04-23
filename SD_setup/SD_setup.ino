#include <SD.h>
#include <SPI.h>

int cont = 0;
char readData[11]= {"xxxxxxxxxx"}; 
char nombreArch[] = "LOGGER00.CSV";

//-----SD stuff
const String path = "/CircuitTests";
const String file = "mdata.txt";
const int CS = 8; //Chip select
File datalogFile; //Datalog File

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  delay(2000);
  Serial.println("Pulsa cualquier tecla para iniciar.");
  while (!Serial.available()) {}
  while (!initSD()) { Serial.println("Starting SD again..."); }
  GenerateFilename(nombreArch);
  File openedFile = SD.open(nombreArch, FILE_WRITE); //prueba_0.txt
  openedFile.close();
  Serial.println(nombreArch);
  while (!writeDataToFile("Prueba,1", nombreArch)) { Serial.println("Sending data to SD again..."); }
  Serial.println("Datos guardados.");
  file = nombreArch;
  Serial.print("Nombre del archivo: ");
  Serial.println(file);
//  Serial.print("Numero leido: ");
//  Serial.println(numPrueba);
}

void loop() {
  cont++;
  while (!writeDataToFile((String)cont, file)) { Serial.println("Guardando datos de nuevo..."); }
  Serial.println((String) cont);
  blinkLed();
  if (cont == 10) {  
    while(true);
  }
}

bool initSD() {
  if (!SD.begin(CS)) {
    Serial.println("Error");  // Check if SD card is present
    return false;
  } else { Serial.println("Init successful"); }
  return true;
}

bool writeDataToFile(String datos, String filename) {
  datalogFile = SD.open(filename, FILE_WRITE); // Add header to file
  if (datalogFile) {
    datalogFile.println(datos);
    datalogFile.close();
  } else {
    Serial.println("Error opening .txt");
    return false;
  }

  return true;
}

void blinkLed() {
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(1000);
}

int buscarNumPrueba(File dirBase){
  File entry = dirBase;
  int num;
  while(true){
    entry = entry.openNextFile();
    Serial.println(entry.name());
    if(!entry.openNextFile()){ 
      String nombre = entry.name();
      num = nombre.substring(7).toInt();
      entry.close();
      break; 
    }
    entry.close();
  }
  return num;
}

void GenerateFilename(char* fileName){
  // Construct the filename to be the next incrementally indexed filename in the set [00-99].
  for (byte i = 1; i <= 99; i++)
  {
    // check before modifying target filename.
    if (SD.exists(fileName))
    {
      // the filename exists so increment the 2 digit filename index.
      fileName[6] = i/10 + '0';
      fileName[7] = i%10 + '0';
    } else {
      break;  // the filename doesn't exist so break out of the for loop.
    }
  }
}

