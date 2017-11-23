
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
    unsigned int password = Serial.parseInt();
    if (password <= 9999) {
      Serial.print("Senha digitada: "); Serial.println(password);

      Serial.println("Gravando...");

      byte firstByte = password & mask;
      short firstAddress = writeToNextEmptyEEPROMAddress(firstByte);
      Serial.print("1)Endereco: "); Serial.println(firstAddress);

      byte secondByte = (password >> 8) & mask;
      short secondAddress = writeToNextEmptyEEPROMAddress(secondByte);
      Serial.print("2)Endereco: "); Serial.println(secondAddress);

      Serial.println("Lendo...");

      byte recoveredFirstByte = EEPROM.read(firstAddress);
      byte recoveredSecondByte = EEPROM.read(secondAddress);
      Serial.println(recoveredFirstByte);
      Serial.println(recoveredSecondByte);

      unsigned short recoveredPassword = (recoveredSecondByte << 8) | recoveredFirstByte;
      // Isso da problema pois a senha 0001 e diferente da 1, entao e preciso receber a senha
      // como String e guardar os bytes de cada parte da String
      Serial.print("Recuperado:");Serial.println(recoveredPassword);
      digitalWrite(13, HIGH);
    } else {
      Serial.println("Máximo permitido: 9999");
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
  Serial.println("Nenhum endereço vago!");
}
