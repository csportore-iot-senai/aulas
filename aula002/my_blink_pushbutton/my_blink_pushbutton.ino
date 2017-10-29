const int pinLed13 = 13;
const int pinButton = 6;

void setup() {
  pinMode(pinLed13, OUTPUT);
}

void loop() {
  int btPressed = digitalRead(pinButton);
  if (btPressed == HIGH) {
    digitalWrite(pinLed13, HIGH);
  } else {
    digitalWrite(pinLed13, LOW);
  }
}
