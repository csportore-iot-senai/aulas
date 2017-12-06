
#include <EEPROM.h>
#include <Keypad.h>

/******************************************
                  EEPROM
*******************************************/
// 7 por causa do NULL no final da String
const short passwordMaxDigits = 7;
const char defaultPasswordActivate[] = "000000";
const char defaultPasswordDeactivate[] = "000000";
const char defaultPasswordPanic[] = "999999";

int passCode = 0;
boolean digitandoSenha = false;
const char senha[passwordMaxDigits] = {"123456"};
char senhaDigitada[6] = {};
int digitosSenha = 0;
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

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  initializeEEPROM();
}

void loop() {

  digitalWrite(13, LOW);
  char key = keypad.getKey();
  if (Serial.available()) {
    passCode = Serial.parseInt();
    Serial.println(passCode);
    if (passCode > 3) {
      int multiplier = passCode - 3;
      char* result = getPassword(multiplier);
      for (int i = 0; i < passwordMaxDigits; i++) {
        Serial.print(result[i]);
      }
      Serial.println();
    }
  }

  if (key) {
    if (key == '*') {
      Serial.println("Iniciando digitação da senha...");
      digitandoSenha = true;
      return;
    } else if (key == '#' && passCode <= 3) {
      Serial.println("Terminando digitação da senha...");
      digitandoSenha = false;
      Serial.print("Chamando [setPassword]: ");
      for (int i = 0; i < sizeof(senhaDigitada); i++) {
        Serial.print(senhaDigitada[i]);
      }
      Serial.println();
      setPassword(senhaDigitada, passCode);
      Serial.print("Senha: "); Serial.println(senhaDigitada);
      if (strcmp(senhaDigitada, senha)) {
        Serial.println("Alarme desarmado!");
      } else {
        Serial.println("ERROU!");
      }
      memset(senhaDigitada, 0, sizeof(senhaDigitada));
    }

    if (digitandoSenha) {
      senhaDigitada[digitosSenha] = key;
      digitosSenha++;
    }

    Serial.println(key);
  }

}

void initializeEEPROM() {
  Serial.println("####### EEPROM Initialization START #######");
  Serial.println("Clearing EEPROM...");
  char initializer[1024] = {};
  EEPROM.put(0, initializer);
  
  Serial.println("Writing default passwords...");
  char pw[7 * 3] = {};
  setPassword(defaultPasswordActivate, 1);
  setPassword(defaultPasswordDeactivate, 2);
  setPassword(defaultPasswordPanic, 3);
  Serial.println("####### EEPROM Initialization END #######");
  EEPROM.get(0, pw);
  for (int i = 0; i < 7 * 3; i++) {
    Serial.print(pw[i]);
  }
  Serial.println();
}

boolean setPassword(char* password, int passwordTypeCode) {
  short multiplier = passwordTypeCode - 1;
  int minI = passwordMaxDigits * multiplier;
  int maxI = passwordMaxDigits * multiplier + passwordMaxDigits;
  
  for (int i = minI; i < maxI; i++) {
    EEPROM.write(i, password[i - minI]);
  }

  return true;
}

char* getPassword(int passwordTypeCode) {
  char result[passwordMaxDigits] = {};
  short resultIndex = 0;
  short multiplier = passwordTypeCode - 1;
  int minI = passwordMaxDigits * multiplier;
  int maxI = passwordMaxDigits * multiplier + passwordMaxDigits;
  
  for (int i = minI; i < maxI; i++) {
    result[resultIndex] = EEPROM.read(i);
    Serial.println(EEPROM.read(i));
    resultIndex++;
  }

  return result;
}




