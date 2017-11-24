#include <eeprom.h>
const short passwordMaxDigits = 6;
const char defaultPasswordActivate[] = {'0', '0', '0', '0', '0', '0'};
const char defaultPasswordDeactivate[] = {'0', '0', '0', '0', '0', '0'};
const char defaultPasswordPanic[] = {'9', '9', '9', '9', '9', '9'};
const char newLineCharacter = 10;
const short eepromStartingIndex = 1;

void initializeEEPROM() {
  EEPROM.put(0,0);
  EEPROM.put(1, defaultPasswordActivate);
  EEPROM.put(passwordMaxDigits + 1, defaultPasswordActivate);
  EEPROM.put((passwordMaxDigits + 1) * 2, defaultPasswordActivate);

}
