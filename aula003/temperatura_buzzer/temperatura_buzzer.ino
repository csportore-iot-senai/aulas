unsigned int i = 0;
int ledRedPin = 13;
int ledYellowPin = 12;
int ledGreenPin = 11;
int ledPiezo = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
  }
  // sends info as byte or byte arrays
  // Serial.write("Olá, mundo!");
  // sends info as propper character array
  Serial.println("Serial pronto!");
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledYellowPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0 ) {
    int value = Serial.parseInt();
    if (value > 50) {
      turnOn(ledRedPin);
      tone(6, 1000, 500);
      //noTone(6);
    } else if (value <= 50 && value >= 30) {
      turnOn(ledYellowPin);
    } else if (value < 30 && value >= 20) {
      turnOn(ledGreenPin);
    } else {
      turnAllOff();
      Serial.println(value + " nao existe!");
    }
  }
}

void turnOn(int pin) {
  turnAllOff();
  digitalWrite(pin, HIGH);
}

void turnAllOff() {
  digitalWrite(ledRedPin, LOW);
  digitalWrite(ledYellowPin, LOW);
  digitalWrite(ledGreenPin, LOW);
}
