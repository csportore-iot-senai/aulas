#include <SPI.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include <PubSubClient.h>

//MQTT
byte mac[] = { 0xAA, 0xED, 0xFF, 0xFE, 0xAF, 0x15 };
char* server = "m10.cloudmqtt.com";
int port = 16813;
bool isConnected = false;
void whenMessageReceived(char* topic, byte* payload, unsigned int length) {
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

EthernetClient ethClient;
PubSubClient client(server, port, whenMessageReceived, ethClient);
const int LED_PIN = 8;

bool attemptConnection() {
  Serial.println("Attempting DHCP");
  if (!Ethernet.begin(mac)) {
    Serial.println("DHCP Failed");
    return false;
  } else {
    Serial.println(Ethernet.localIP());
    Serial.println("Connecting...");
    if (client.connect("arduinoClient", "admin", "admin")) {
      Serial.println("Connected");
      client.publish("outTopic", "hello world");
      Serial.println("outTopic sent");
      client.subscribe("inTopic");
      digitalWrite(LED_PIN, HIGH);
      return true;
    } else {
      Serial.println("Failed to connect to MQTT server");
      return false;
    }
  }
}
void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  isConnected = attemptConnection();
  pinMode(LED_PIN, OUTPUT);
}



void loop() {
  while (!isConnected) {
    Serial.println("...trying to reconnect");
    isConnected = attemptConnection();
  }
  client.loop();
}
