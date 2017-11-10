#include <SPI.h>
#ifdef SENAI
#include <UIPEthernet.h>
#include <utility/logging.h>
#else
#include <Ethernet.h>
#endif
#include <PubSubClient.h>

//=============================================================//
//========================= LED ===============================//
//=============================================================//
const int GREEN_LED_PIN = 8;
const int YELLOW_LED_PIN = 7;
int ledArray[] = {GREEN_LED_PIN, YELLOW_LED_PIN};
unsigned long previousMillis = 0;
unsigned long blinkInterval = 100;
const int blinkMaxCount = 5;
int blinkCounter = 0;
bool blinkPublishLED = false;
bool blinkReceivedMsgLED = false;

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
const char device_id[] = {"#15"};
const char user[] = {"admin"};
const char password[] = {"admin"};
const char willTopicName[] = {"will-topic"};
const char willTopicMessage[] = {"Device #15 is offline"};
const unsigned int willTopicQos = 0;
const boolean willTopicRetain = false;
const char writeToTopic[] = "outTopic";
const char readFromTopic[] = "inTopic";

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
  blinkReceivedMsgLED = true;
}

PubSubClient mqttClient(server, port, whenMessageReceived, ethClient);

bool initializeEthernet() {
  Serial.print("Attempting DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("...but it failed miserably");
    Serial.flush();
    return false;
  } else {
    Serial.print(" with "); Serial.println(Ethernet.localIP());
    Serial.flush();
    return true;
  }
}
void establishMQTTConnection() {
  if (ethClient.connected()) {
    // MQTT Connection
    Serial.println("Connecting to MQTT...");
    //boolean PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage)
    if (mqttClient.connect(device_id, user, password, willTopicName, willTopicQos, willTopicRetain, willTopicMessage)) {
      publishMQTTMessage(writeToTopic);
      Serial.println("MQTT Connected!");
      Serial.flush();
    } else {
      Serial.println("Failed to connect to MQTT server");
      Serial.flush();
    }
  } else {
    initializeEthernet();
  }
}

void publishMQTTMessage(char* topic, char* message) {
  if (mqttClient.publish(topic, message)) {
    Serial.print(topic); Serial.println((" sent!"));
    Serial.flush();
    blinkPublishLED = true;
  } else {
    Serial.print(topic); Serial.println((" not completed!"));
    Serial.flush();
  }
}

void publishMQTTMessage(char* topic) {
  if (mqttClient.publish(topic, "ACG says \"Hello world\"")) {
    Serial.print(topic); Serial.println((" sent!"));
    Serial.flush();
  } else {
    Serial.print(topic); Serial.println((" not completed!"));
    Serial.flush();
  }
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
  Serial.print("Message:"); Serial.println(result);
  Serial.flush();
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
      Serial.print("Command \""); Serial.print(radix); Serial.println("\" does not exist!");
      Serial.println("Type \"p\" to publish or \"s\" to subscribe!");
    }
  } else {
    Serial.println("Malformed line!");
  }
  Serial.flush();
}

void turnOffAllLEDs() {
  for (int i = 0; i < sizeof(ledArray); i++) {
    digitalWrite(ledArray[i], LOW);
  }
}

bool blinkLED(int ledPin, unsigned long interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    if (blinkMaxCount >= blinkCounter) {
      previousMillis = currentMillis;
      swapPhase(ledPin);
      blinkCounter++;
      return true;
    } else {
      blinkCounter = 0;
      return false;
    }
  }
}

void swapPhase(int ledPin) {
  digitalWrite(ledPin, !digitalRead(ledPin));
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  if (initializeEthernet()) {
    establishMQTTConnection();
  } else {
    Serial.println("No internet connection!");
  }

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
}

void loop() {
  if (mqttClient.connected()) {
    if (Serial.available()) {
      String command = Serial.readString();
      executeCommand(command);
    }
    mqttClient.loop();
  } else {
    Serial.println("There's no connection, mate. Type \"r\" to retry and good luck!");
    Serial.flush();
    turnOffAllLEDs();
    String serialMessage = "";
    while (serialMessage != "r") {
      serialMessage = Serial.readString();
    }
    establishMQTTConnection();
  }
  int result = Ethernet.maintain();
  if (result == 3 || result == 1) {
    initializeEthernet();
  }
  if (blinkPublishLED) {
    blinkPublishLED = blinkLED(GREEN_LED_PIN, blinkInterval);
  }
  if (blinkReceivedMsgLED) {
    blinkReceivedMsgLED = blinkLED(YELLOW_LED_PIN, blinkInterval);
  }
}

