const int pinLed13 = 13;
const int pinLed12 = 12;
void setup() {
  //LED_BUILTIN is the built in led or the 13th pin
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinLed13, OUTPUT);
  pinMode(pinLed12, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 4; i++) {
    digitalWrite(pinLed13, HIGH);
    delay(100 * i);
    digitalWrite(pinLed13, LOW);
    delay(100 * i);
    digitalWrite(pinLed12, HIGH);
    delay(50 * i);
    digitalWrite(pinLed12, LOW);
    delay(50 * i);
  }
}
