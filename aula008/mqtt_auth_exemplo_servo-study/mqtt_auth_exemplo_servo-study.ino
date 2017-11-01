/*
  Basic MQTT example with Authentication

  - connects to an MQTT server, providing username
    and password
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <SPI.h>
#include <Ethernet.h>
//#include <utility/logging.h>
#include <PubSubClient.h>
#include <Servo.h>

//SERVER
byte mac[] = { 0xAA, 0xED, 0xFF, 0xFE, 0xAF, 0x15 };
char* server = "m10.cloudmqtt.com";
int port = 16813;

//SERVO
Servo myservo;
int pos = 0;    // variable to store the servo position

//LEDS
const int LED_PIN = 6;
const int YLED_PIN = 2;
const int RLED_PIN = 3;
long previousMillis = 0;
long interval = 300;

void whenMessageReceived(char* topic, byte* payload, unsigned int length) {
  // Converter pointer do tipo `byte` para typo `char`
  payload[length] = 0;
  char* payloadAsChar = payload;

  // Converter em tipo String para conveniência
  String msg = String(payloadAsChar);
  String topicString = String(topic);

  Serial.print("Topic received: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(msg);
  
  int msgCode = msg.toInt();

  if (msgCode) {
    digitalWrite(LED_PIN, HIGH);
    for (pos = 0; pos <= 180; pos += 1) {
      // in steps of 1 degree
      myservo.write(pos);
    }
  } else {
    for (pos; pos >= 0; pos -= 1) {
      myservo.write(pos);
    }
    digitalWrite(LED_PIN, LOW);
  }
  // Dentro do callback da biblioteca MQTT,
  // devemos usar Serial.flush() para garantir que as mensagens serão enviadas
  Serial.flush();

  int msgComoNumero = msg.toInt();

  Serial.print("Numero lido: "); Serial.println(msgComoNumero);
  Serial.flush();
}

EthernetClient ethClient;
PubSubClient client(server, port, whenMessageReceived, ethClient);

void setup()
{
  //LED
  pinMode(LED_PIN, OUTPUT);
  pinMode(YLED_PIN, OUTPUT);
  pinMode(RLED_PIN, OUTPUT);
  digitalWrite(YLED_PIN, HIGH);

  //SERVO
  myservo.attach(9);

  //SERVER
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("Attempting DHCP");
  if (!Ethernet.begin(mac)) {
    Serial.println("DHCP Failed");
  } else {
    Serial.println(Ethernet.localIP());
  }

  Serial.println("Connecting...");
  // Conectando com informações de cliente e senha criados através da interface web do serviço
  if (client.connect("arduinoClient", "admin", "admin")) {
    Serial.println("Connected");
    client.publish("outTopic", "Send me data");
    Serial.println("outTopic sent");
    client.subscribe("inTopic");
  } else {
    Serial.println("Failed to connect to MQTT server");
  }
}

void loop()
{
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

  client.loop();
}

void turnOffAllLEDs() {
  digitalWrite(RLED_PIN, LOW);
  digitalWrite(YLED_PIN, LOW);
}
