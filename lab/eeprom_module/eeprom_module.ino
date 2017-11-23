
#include <EEPROM.h>
#include <Keypad.h>

/******************************************
                  EEPROM
*******************************************/
const short passwordMaxDigits = 6;
const char newLineCharacter = 10;
const short eepromStartingIndex = 1;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  digitalWrite(13, LOW);

  if (typedPassword.length() == passwordMaxDigits) {




    for (short i = 0; i < arraySize; i++) {
      byte data = passwordArray[i];
      short address = writeToNextEmptyEEPROMAddress(data);
      addresses[i] = address;
      Serial.print("Endereco: "); Serial.println(address);
    }

    char recoveredPassword[arraySize];
    Serial.println("Lendo...");
    for (short i = 0; i < arraySize; i++) {
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

short getNumberOfPasswordsInMemory() {
  return EEPROM[0];
}

String[] fetchAllPasswords() {
  if (getNumberOfPasswordsInMemory > 0) {

  }
}
void savePasswordToMemory(String typedPassword, int passwordTypeID) {
  Serial.print("Senha digitada: "); Serial.println(typedPassword);
  Serial.println("Gravando...");

  for (short i = eepromStartingIndex; i < arraySize * passwordTypeID; i++) {
    EEPROM[i] = (byte) typedPassword[i];
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

void checkPassword() {
}
