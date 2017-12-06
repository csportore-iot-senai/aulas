#include <Keypad.h>

boolean digitandoSenha = false;
const String senha = "1234";
String senhaDigitada = "";
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {5, A0, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '*') {
      Serial.println("Iniciando digitação da senha...");
      senhaDigitada = "";
      digitandoSenha = true;
      return;
    } else if (key == '#') {
      Serial.println("Terminando digitação da senha...");
      digitandoSenha = false;
      Serial.print("Senha: "); Serial.println(senhaDigitada);
      if (senhaDigitada == senha) {
        Serial.println("Alarme desarmado!");
      } else {
        Serial.println("ERROU!");
        //toca piezzo buzzer
      }
      senhaDigitada = "";
    }

    if (digitandoSenha) {
      senhaDigitada += key;
    }

    Serial.println(key);
  }

}
