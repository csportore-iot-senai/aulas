
#include <LiquidCrystal.h>

int lastMessageRow = 0;
String lastMessageSent = "";

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // Inicializa a porta Serial
  Serial.begin(9600);

  lcd.begin(16, 2);

}

void loop() {
  if (Serial.available() > 0) {
    String msg = Serial.readString();
    Serial.print("Mensagem recebida: ");
    Serial.print(msg);
    Serial.println();
    if (lastMessageRow == 1) {
      printMessage(lastMessageSent, 0);
      printMessage(msg, 1);
      lastMessageRow = 0;
    } else {
      printMessage(msg, lastMessageRow);
    }
    lastMessageRow++;
  }
}

void printMessage(String msg, int lastMessageRow) {
  lcd.setCursor(0, lastMessageRow);
  lcd.print("                ");
  lcd.setCursor(0, lastMessageRow);
  lcd.print(msg);
  lastMessageSent = msg;
}

