/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
const int YLED_PIN = 2;
const int RLED_PIN = 4;
long previousMillis = 0;
long interval = 300;

void setup() {
  // set the digital pin as output:
  pinMode(YLED_PIN, OUTPUT);
  pinMode(RLED_PIN, OUTPUT);
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (digitalRead(YLED_PIN) == LOW) {
      turnOffAllLEDs();
      digitalWrite(YLED_PIN, HIGH);
    } else {
      turnOffAllLEDs();
      digitalWrite(RLED_PIN, HIGH);
    }
  }
}

void turnOffAllLEDs() {
  digitalWrite(RLED_PIN, LOW);
  digitalWrite(YLED_PIN, LOW);
}
