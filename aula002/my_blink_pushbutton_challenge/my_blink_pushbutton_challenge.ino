const int pinLed13 = 13;
const int pinButton = 6;
boolean turnLedOn = false;
int currentState = LOW;

void setup() {
  pinMode(pinLed13, OUTPUT);
}

/*void loop() {
  int btPressed = digitalRead(pinButton);

  if (btPressed == HIGH) {
    if (turnLedOn) {
        delay(200);
        turnLedOn = false;
    } else {
        delay(200);
        turnLedOn = true;
    }
  }

  if (turnLedOn) {
    digitalWrite(pinLed13, HIGH);
  } else {
    digitalWrite(pinLed13, LOW);
  }
  }*/


void loop() {
  int btPressed = digitalRead(pinButton);

  digitalWrite(pinLed13, currentState);

  if (btPressed == HIGH) {
    if (currentState == LOW) {
      currentState = HIGH;
    } else {
      currentState = LOW;
    }
  }
}
