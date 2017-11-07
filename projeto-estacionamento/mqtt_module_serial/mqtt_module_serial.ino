
#ifdef ONLINE
#include <PubSubClient.h>
#else
#include <SerialPubSubClient.h>
#endif
#include <SPI.h>
#include <Ethernet.h>


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

//=============================================================//
//========================= MQTT ==============================//
//=============================================================//

char* server = "m10.cloudmqtt.com";
int port = 16813;
const char device_id[] = {"ACG-IoT"};
const char user[] = {"admin"};
const char password[] = {"admin"};
const char willTopicName[] = {"will-topic"};
const char willTopicMessage[] = {" device is offline"};
const unsigned int willTopicQos = 0;
const boolean willTopicRetain = false;
const char writeToTopic[] = "outTopic";
const char readFromTopic[] = "inTopic";
bool isMQTTConnected = false;
bool isReceivingMessage = false;

void whenMessageReceived(char* topic, byte* payload, unsigned int length) {
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

  Serial.print("Numero lido: "); Serial.println(msgCode);
  Serial.flush();
}

PubSubClient mqttClient(server, port, whenMessageReceived, ethClient);

void establishConnection() {
  #ifdef ONLINE
  Serial.print("Attempting DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("...but it failed miserably");
  } else {
    Serial.print(" with "); Serial.println(Ethernet.localIP());
    // MQTT Connection
    Serial.println("Connecting...");
    //boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage)
    if (mqttClient.connect(device_id, user, password, willTopicName, willTopicQos, willTopicRetain, willTopicMessage)) {
      publishMQTTMessage(writeToTopic);
      Serial.println("MQTT Connected!");
    } else {
      Serial.println("Failed to connect to MQTT server");
    }
  }
  #endif
}

void piscarEnvio(int bytes) {
  
}

void publishMQTTMessage(char* topic, char* message) {
  piscarEnvio(strlen(message));
  if (mqttClient.publish(topic, message)) {
    Serial.print(topic); Serial.println((" sent!"));
  } else {
    Serial.print(topic); Serial.println((" not completed!"));
  }
  Serial.flush();
}

void publishMQTTMessage(char* topic) {
  if (mqttClient.publish(topic, "ACG says \"Hello world\"")) {
    Serial.print(topic); Serial.println((" sent!"));
  } else {
    Serial.print(topic); Serial.println((" not completed!"));
  }
  Serial.flush();
}

void subscribeToMQTTTopic(char* topic) {
  if (mqttClient.subscribe(topic)) {
    Serial.print(topic); Serial.println((" subscribed!"));
  } else {
    Serial.print(topic); Serial.println((" not completed!"));
  }
  Serial.flush();
}

char* findParameter(String command) {
  int firstQuote = command.indexOf("\"");
  int lastQuote = command.lastIndexOf("\"");
  String result = command.substring(firstQuote + 1, lastQuote);
  Serial.print("Message:");Serial.println(result);
  char msg[result.length()];
  result.toCharArray(msg, result.length() + 1);
  return msg;
}

bool executeCommand(String command) {
  if (command.length() >= 4) {
    char radix = command[0];
    if (radix == 'p') {
      publishMQTTMessage(writeToTopic, findParameter(command));
    } else if (radix == 's') {
      subscribeToMQTTTopic(findParameter(command));
    } else {
      Serial.print("Command \"");Serial.print(radix);Serial.println("\" does not exist!");
      Serial.println("Type \"p\" to publish or \"s\" to subscribe!");
    }
  } else {
    Serial.println("Malformed line!");
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  #ifdef ONLINE
  establishConnection();
  #endif
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
}

void loop() {
  #ifdef ONLINE
  if (mqttClient.connected()) {
    turnOnLED(GREEN_LED_PIN);
    if (Serial.available()) {
      String command = Serial.readString();
      executeCommand(command);
    }
    mqttClient.loop();
  } else {
    Serial.println("There's no connection, mate. Type \"r\" to retry and good luck!");
    turnOffAllLEDs();
    String serialMessage = "";
    while (serialMessage != "r") {
      serialMessage = Serial.readString();
    }
    establishConnection();
  }
  #endif
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

