
const int GREEN_LED_PIN = 8;
const int YELLOW_LED_PIN = 7;
unsigned long previousMillis = 0;
unsigned long blinkInterval = 100;

void setup() {
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

}

void loop() {
  blinkLED(GREEN_LED_PIN, blinkInterval);
  blinkLED(YELLOW_LED_PIN, blinkInterval);
}

void blinkLED(int ledPin, unsigned long interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    digitalWrite(ledPin, swapPhase(ledPin));
  }
}

int swapPhase(int ledPin) {
  if (digitalRead(ledPin)) {
    return LOW;
  } else {
    return HIGH;
  }
}
