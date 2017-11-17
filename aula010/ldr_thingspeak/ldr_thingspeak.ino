//Sensor de luz com LDR

#include <ThingSpeak.h>
//#include <SPI.h>
#include <UIPEthernet.h>
#include <utility/logging.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xEE, 0x15};
EthernetClient client;

char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long myChannelNumber = 367038;
const char * myWriteAPIKey = "RI2Q2C5NQ7LSE0B1";

int ldrPin = A0; //LDR no pino analígico 8
int ldrValor = 0; //Valor lido do LDR

void setup() {
  Ethernet.begin(mac);
  ThingSpeak.begin(client);
  /*Serial.begin(9600); //Inicia a comunicação serial
  while (!Serial) {
    ;
  }
  Serial.println(Ethernet.localIP());*/
}

void loop() {
  ///ler o valor do LDR
  ldrValor = analogRead(ldrPin); //O valor lido será entre 0 e 1023
  //imprime o valor lido do LDR no monitor serial
  //Serial.println(ldrValor);
  ThingSpeak.writeField(myChannelNumber, 1, ldrValor, myWriteAPIKey);
  delay(1000);
}
