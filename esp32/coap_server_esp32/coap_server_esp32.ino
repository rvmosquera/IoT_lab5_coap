#include <WiFi.h>
#include <coap-simple.h>
#include <DHT.h>
#define DHT_PIN 4
#define DHTTYPE DHT11

const char *ssid = "HUAWEIP10lite";
const char *password = "9261566b39f3";
DHT dht(DHT_PIN, DHTTYPE);
// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port);

// UDP and CoAP class
// other initialize is "Coap coap(Udp, 512);"
// 2nd default parameter is COAP_BUF_MAX_SIZE(defaulit:128)
// For UDP fragmentation, it is good to set the maximum under
// 1280byte when using the internet connection.
WiFiUDP udp;
Coap coap(udp);

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");

  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  Serial.println(p);
}

void setup() {
  Serial.begin(115200);

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  dht.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // client response callback.
  // this endpoint is single callback.
  Serial.println("Setup Response Callback");
  coap.response(callback_response);

  // start coap server/client
  coap.start();
}

int value = 0;

void loop() {
  delay(5000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String data = "{\"temperature\":";
  data = data + t;
  data = data + ",\"humidity\":";
  data = data + h;
  data = data + "}";
  char dataChar[data.length() + 1];
  data.toCharArray(dataChar, data.length() + 1);

  Serial.println("Send Request");
  int msgid = coap.put(IPAddress(192,168,174,2), 5683, "temperature", dataChar, data.length());

  coap.loop();
}
