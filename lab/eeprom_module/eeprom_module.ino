
#include <EEPROM.h>
#include <Keypad.h>

/******************************************
                  EEPROM
*******************************************/
// 7 por causa do NULL no final da String
const short passwordMaxDigits = 7;
const char* defaultPasswordActivate = "000000";
const char* defaultPasswordDeactivate = "000000";
const char* defaultPasswordPanic = "999999";

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
}

void initializeEEPROM() {
  Serial.println("####### EEPROM Initialization START #######");
  Serial.println("Clearing EEPROM...");
  for (short i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  Serial.println("Writing default passwords...");
  EEPROM.put(1, defaultPasswordActivate);
  EEPROM.put(passwordMaxDigits + 1, defaultPasswordDeactivate);
  EEPROM.put((passwordMaxDigits + 1) * 2, defaultPasswordPanic);
  Serial.println("####### EEPROM Initialization END #######");
}

boolean setPassword(char* password, int passwordTypeCode) {
  boolean result;
  char* realPassword[passwordMaxDigits];
  memcpy(password, realPassword, passwordMaxDigits);
  switch (passwordTypeCode) {
    case 1:
      EEPROM.put(1, realPassword);
      result = true;
      break;
    case 2:
      EEPROM.put(passwordMaxDigits + 1, realPassword);
      result = true;
      break;
    case 3:
      EEPROM.put((passwordMaxDigits + 1) * 2, realPassword);
      result = true;
      break;
    default:
      result = false;
  }
  return result;
}

char* getPassword(int passwordTypeCode) {
  String result = "";
  short endAddress = passwordTypeCode * passwordMaxDigits;
  short startAddress = endAddress - passwordMaxDigits;

  for (short i = startAddress; i <= endAddress; i++) {
    result += (char) EEPROM.read(i);
  }

  return result;
}




