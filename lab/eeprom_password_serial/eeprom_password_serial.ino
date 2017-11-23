
#include <EEPROM.h>

const byte mask = 0xFF;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

}
//11110 1100010
void loop() {
  if (Serial.available()) {

    digitalWrite(13, LOW);
    String password = Serial.readString();
    int passwordSize = password.length();

    if (passwordSize == 4) {

      Serial.print("Senha digitada: "); Serial.println(password);

      Serial.println("Gravando...");
      short arraySize = passwordSize + 1;
      char passwordArray[arraySize];
      password.toCharArray(passwordArray, arraySize);
      //DEBUG
      Serial.print("[toCharArray]:"); Serial.println(passwordArray);
      char addresses[arraySize];
      //DEBUG
      Serial.print("[sizeof$addresses]:"); Serial.println(sizeof(addresses));
      for (short i = 0; i < sizeof(passwordArray); i++) {
        byte data = passwordArray[i];
        short address = writeToNextEmptyEEPROMAddress(data);
        addresses[i] = address;
        Serial.print("Endereco: "); Serial.println(address);
      }

      char recoveredPassword[passwordSize];
      Serial.println("Lendo...");
      for (short i = 0; i < sizeof(addresses); i++) {
        byte recoveredData = EEPROM.read(addresses[i]);
        recoveredPassword[i] = (char) recoveredData;
      }

      Serial.print("Recuperado:"); Serial.println(recoveredPassword);
      if (String(recoveredPassword) == "1234") {
        Serial.println("Senha correta!");
      }
      digitalWrite(13, HIGH);

    } else {
      Serial.println("Maximo permitido: 9999");
      Serial.println("Tente novamente");
    }

  }
}

short writeToNextEmptyEEPROMAddress(byte data) {
  for (short i = 0; i < EEPROM.length(); i++) {
    if (EEPROM.read(i) == 0) {
      EEPROM.write(i, data);
      return i;
    }
  }
  Serial.println("Nenhum endereco vago!");
}
