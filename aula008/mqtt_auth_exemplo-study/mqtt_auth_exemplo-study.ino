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

// Atualizar ultimo valor para ID do seu Kit para evitar duplicatas
byte mac[] = { 0xAA, 0xED, 0xFF, 0xFE, 0xAF, 0x15 };
IPAddress ip(192, 168, 3, 176);
// Endereço do Cloud MQTT
char* server = "m10.cloudmqtt.com";

const int LED_PIN = 7;

// Valor da porta do servidor MQTT
int port = 16813;

void whenMessageReceived(char* topic, byte* payload, unsigned int length) {
  // Converter pointer do tipo `byte` para typo `char`
  payload[length] = 0;
  char* payloadAsChar = payload;

  // Converter em tipo String para conveniência
  String msg = String(payloadAsChar);
  String topicString = String(topic);
  Serial.print("Topic received: "); Serial.println(topic);
  Serial.print("Message: "); Serial.println(msg);
  if (msg == "ON") {
    digitalWrite(LED_PIN, HIGH);
  } else {
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
  pinMode(LED_PIN, OUTPUT);
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
    client.publish("outTopic", "hello world");
    Serial.println("outTopic sent");
    client.subscribe("inTopic");
  } else {
    Serial.println("Failed to connect to MQTT server");
  }
}

void loop()
{
  // A biblioteca PubSubClient precisa que este método seja chamado em cada iteração de `loop()`
  // para manter a conexão MQTT e processar mensagens recebidas (via a função callback)
  client.loop();
}

