#include <SPI.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include <PubSubClient.h>

//=============================================================//
//========================= LED ===============================//
//=============================================================//
const int GREEN_LED_PIN = 8;
const int YELLOW_LED_PIN = 7;
int lastStateGreenPin = LOW;
int lastStateYellowPin = LOW;
int ledArray[] = {GREEN_LED_PIN, YELLOW_LED_PIN};
unsigned long previousMillis = 0;
unsigned long blinkInterval = 20;

//=============================================================//
//========================= ETHERNET ==========================//
//=============================================================//
EthernetClient ethClient;
byte mac[] = { 0xAA, 0xED, 0xFF, 0xFE, 0xAF, 0x15 };

void establishEthernetConnection() {
  Serial.println("Attempting DHCP");
  if (!Ethernet.begin(mac)) {
    Serial.println("DHCP Failed");
    return false;
  } else {
    Serial.println(Ethernet.localIP());
  }
}

//=============================================================//
//========================= MQTT ==============================//
//=============================================================//

char* server = "m10.cloudmqtt.com";
int port = 16813;
const char device_id[] = {"ACG-IoT"};
const char user[] = {"admin"};
const char password[] = {"admin"};
const char willTopicName[] = {"will-topic"};
const char willTopicMessage[] = {String(device_id).concat(" is offline")};
const unsigned int willTopicQos = 0;
const boolean willTopicRetain = false;
const char writeToTopic[] = "outTopic";
const char readFromTopic[] = "inTopic";
bool isMQTTConnected = false;
bool isReceivingMessage = false;

void whenMessageReceived(char* topic, byte* payload, unsigned int length) {
  isReceivingMessage = true;
  payload[length] = 0;
  char* payloadAsChar = payload;
  // Converter em tipo String para conveniência
  String msg = String(payloadAsChar);
  String topicString = String(topic);
  Serial.print("Topic received: "); Serial.println(topic);
  Serial.print("Message: "); Serial.println(msg);
  int msgComoNumero = msg.toInt();
  Serial.flush();
}

PubSubClient mqttClient(server, port, whenMessageReceived, ethClient);

bool establishMQTTConnection() {
  Serial.println("Connecting...");
  //boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage)
  if (mqttClient.connect(device_id, user, password, willTopicName, willTopicQos, willTopicRetain, willTopicMessage)) {
    Serial.println("Connected");
    return true;
  } else {
    Serial.println("Failed to connect to MQTT server");
    return false;
  }
}

void publishMQTTMessage(char* topic) {
  mqttClient.publish(topic, String(device_id).concat(" says \"Hello world\""));
  Serial.print(topic); Serial.println((" sent!"));
  Serial.flush();
}

void subscribeToMQTTTopic(char* topic) {
  mqttClient.subscribe(topic);
  Serial.print(topic); Serial.println((" subscribed!"));
  Serial.flush();
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  establishEthernetConnection();
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
}

void loop() {
  if (ethClient.available()) {
    if (ethClient.connected()) {
      char c = ethClient.read();
      Serial.println(c);
      turnOnLED(GREEN_LED_PIN);
      if (mqttClient.connected()) {
        turnOnLED(YELLOW_LED_PIN);
        publishMQTTMessage(writeToTopic);
        subscribeToMQTTTopic(readFromTopic);
        mqttClient.loop();
      } else {
        establishMQTTConnection();
      }
    }
  } else {
    turnOffAllLEDs();
    Serial.println("There's no internet connection, mate. Type \"retry\" and good luck!");
    String serialMessage = "";
    while (serialMessage != "retry") {
      serialMessage = Serial.readString();
    };
    establishEthernetConnection();
  }

  if (isReceivingMessage) {
    blinkLED(YELLOW_LED_PIN);
  } else {
    resumeLEDState(YELLOW_LED_PIN, lastStateYellowPin);
  }
}

void turnOnLED(int pin) {
  digitalWrite(pin, HIGH);
}

void turnOffLED(int pin) {
  digitalWrite(pin, LOW);
}

void turnOnAllLEDs() {
  for (int i = 0; i < sizeof(ledArray); i++) {
    turnOnLED(ledArray[i]);
  }
  // Pensar numa forma melhor de fazer isso
  lastStateGreenPin = HIGH;
  lastStateYellowPin = HIGH;
}

void turnOffAllLEDs() {
  for (int i = 0; i < sizeof(ledArray); i++) {
    turnOffLED(ledArray[i]);
  }
  lastStateGreenPin = LOW;
  lastStateYellowPin = LOW;
}

void resumeLEDState(int ledPin, int lastState) {
  digitalWrite(ledPin, lastState);
}

void blinkLED(int ledPin) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > blinkInterval) {
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

