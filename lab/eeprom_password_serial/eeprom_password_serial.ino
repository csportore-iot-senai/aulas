
#include <EEPROM.h>
#include <Keypad.h>

/******************************************
                  EEPROM
*******************************************/
const short passwordMaxDigits = 6;

/******************************************
                  KEYPAD
*******************************************/
boolean isTypingPassword = false;
boolean isPasswordComplete = false;
String typedPassword = "";
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  pinMode(2, INPUT);
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    if (key == '*') {
      Serial.println("Iniciando digitação da senha...");
      typedPassword = "";
      isTypingPassword = true;
      return;
    } else if (key == '#') {
      Serial.println("Terminando digitação da senha...");
      isTypingPassword = false;
      Serial.print("Senha digitada: "); Serial.println(typedPassword);
      typedPassword = "";
      isPasswordComplete = true;
    }
    if (isTypingPassword) {
      typedPassword += key;
    }
    Serial.println(key);
  }

  if (isPasswordComplete) {
    digitalWrite(13, LOW);

    if (typedPassword.length() == passwordMaxDigits) {

      Serial.print("Senha digitada: "); Serial.println(password);

      Serial.println("Gravando...");
      short arraySize = passwordMaxDigits + 1;
      char passwordArray[arraySize];
      password.toCharArray(passwordArray, arraySize);

      char addresses[arraySize];
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
