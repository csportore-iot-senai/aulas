unsigned int i = 0;
int ledRedPin = 13;
int ledYellowPin = 12;
int ledGreenPin = 11;
int buzzPin = 6;
int potentPin = A1;

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
  pinMode(potentPin, INPUT);
}

void loop() {
  int value = analogRead(potentPin);
  float milliVolts = map(value, 0, 1023, 0, 5000);
  float temperatura = milliVolts / 10.0;
  if (temperatura < 25) {
    turnOn(ledRedPin);
    noTone(6);
  } else if ( temperatura < 27) {
    turnOn(ledYellowPin);
    noTone(6);
  } else if (temperatura < 30) {
    turnOn(ledGreenPin);
    noTone(6);
  } else {
    turnAllOff();
    tone(buzzPin, value);
  }
  Serial.println(temperatura);
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
