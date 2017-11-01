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

//=============================================================//
//========================= MQTT ==============================//
//=============================================================//
byte mac[] = { 0xFE, 0xED, 0x4E, 0xBE, 0xEF, 0x15 };
char* server = "m10.cloudmqtt.com";
int port = 16813;
bool isLoggedIntoMQTTServer = false;
EthernetClient ethClient;

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
  servoAndLightActivation(msgCode);

  Serial.print("Numero lido: "); Serial.println(msgCode);
}

PubSubClient client(server, port, whenMessageReceived, ethClient);
//=============================================================//
//========================= SERVO =============================//
//=============================================================//
Servo myservo;
int pos = 0;    // variable to store the servo position
const int SERVO_DATA_PIN = 3;
const int SERVO_POWER_PIN = 7;
//=============================================================//
//========================= LEDS ==============================//
//=============================================================//
const int LED_PIN = 6;
const int YLED_PIN = 2;
const int RLED_PIN = 3;
long previousMillis = 0;
long interval = 300;

void setup()
{
  //LED
  pinMode(LED_PIN, OUTPUT);
  pinMode(YLED_PIN, OUTPUT);
  pinMode(RLED_PIN, OUTPUT);
  digitalWrite(YLED_PIN, HIGH);

  //SERVO
  pinMode(SERVO_POWER_PIN, OUTPUT);
  digitalWrite(SERVO_POWER_PIN, LOW);
  
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
  if (client.connect("arduinoClient", "dono-garagem", "dono-garagem")) {
    Serial.println("Connected");
    client.publish("controle-garagem", "Estou online!");
    Serial.println("Topic sent");
    client.subscribe("controle-garagem");
    isLoggedIntoMQTTServer = true;
  } else {
    Serial.println("Failed to connect to MQTT server");
    isLoggedIntoMQTTServer = false;
  }
}

void loop() {

  if (isLoggedIntoMQTTServer) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      turnOffAllLEDs();
      if (digitalRead(YLED_PIN) == LOW) {
        digitalWrite(YLED_PIN, HIGH);
      } else {
        digitalWrite(RLED_PIN, HIGH);
      }
    }
  }

  client.loop();
}

void turnOffAllLEDs() {
  digitalWrite(RLED_PIN, LOW);
  digitalWrite(YLED_PIN, LOW);
}

void servoAndLightActivation(int msgCode) {
  if (msgCode) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(SERVO_POWER_PIN, HIGH);
    myservo.attach(SERVO_DATA_PIN);
    for (pos = 0; pos <= 180; pos += 1) {
      // in steps of 1 degree
      myservo.write(pos);
    }
    myservo.detach();
    digitalWrite(SERVO_POWER_PIN, LOW);
  } else {
    digitalWrite(SERVO_POWER_PIN, HIGH);
    myservo.attach(SERVO_DATA_PIN);
    for (pos; pos >= 0; pos -= 1) {
      myservo.write(pos);
    }
    digitalWrite(LED_PIN, LOW);
    myservo.detach();
    digitalWrite(SERVO_POWER_PIN, LOW);
  }

  Serial.flush();
}
