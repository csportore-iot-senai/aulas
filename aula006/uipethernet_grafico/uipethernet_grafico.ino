/*
  Web client

  This sketch connects to a website (http://www.google.com)
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13

  created 18 Dec 2009
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe, based on work by Adrian McEwen

*/

#include <SPI.h>
#include <UIPEthernet.h>
#include <utility/logging.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
IPAddress server(192, 168, 3, 140); // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 3, 17);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

unsigned long start;
unsigned long finish;

const int tempPin = A0;

String createMessage(float temperature) {
  String first = "{\"temp\":";
  String second = ",\"id\":15}";
  String temp = String(temperature);
  String resultArray;
  resultArray = first + temp + second;
  return resultArray;
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(tempPin, INPUT);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  start = millis();
}

void loop() {

  // if there are incoming bytes available
  // from the server, read them and print them:
  finish = millis();
  // if you get a connection, report back via serial:
  //if (finish > start + 5000) {
    if (client.connect(server, 8080)) {
      int value = analogRead(tempPin);
      float milliVolts = map(value, 0, 1023, 0, 5000);
      float temperatura = milliVolts / 10.0;
      String msg = (String)"TEMPERATURA:" + temperatura;
      Serial.println(msg);
      Serial.println("connected");
      // Make a HTTP request:
      client.println("POST / HTTP/1.1");
      client.println("Host: 192.168.3.140:8080");
      client.println("Content-Type: application/json");
      client.println("Content-Length: 23");
      client.println("Connection: close");
      client.println();
      client.println(createMessage(temperatura));
      client.flush();
      Serial.println(createMessage(temperatura));
    } else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
    }
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    //start = finish;
    delay(5000);
  //}

}
