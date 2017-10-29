void setup() {
  // put your setup code here, to run once:
  pinMode(6,OUTPUT);
}

void loop() {
  tone(6, 1000, 300);
  noTone(6);
}
