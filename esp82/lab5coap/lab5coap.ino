#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "coap_client.h"
#include <DHT.h>
//instance for coapclient
coapClient coap;
#define DHT_PIN 0
#define DHTTYPE DHT11

//WiFi connection info
const char* ssid = "DukeV2";
const char* password = "Duke201303";
String DEVICE_SECRET_KEY = "your-device_secret_key";

DHT dht(DHT_PIN, DHTTYPE);
//ip address and default port of coap server in which your interested in
IPAddress ip(192, 168, 1, 196);
int port = 5683;
char* path = "temperature";
float h;
float t;

StaticJsonDocument<200> root;

// coap client response callback
void callback_response(coapPacket& packet, IPAddress ip, int port) {
  Serial.println("ping ok");
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  //response from coap server
  if (packet.type == 3 && packet.code == 0) {
    Serial.println("ping ok");
  }

  Serial.println(p);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.println(" ");

  // Connection info to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    yield();
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address of client
  Serial.println(WiFi.localIP());

  // client response callback.
  // this endpoint is single callback.
  coap.response(callback_response);

  // start coap client
  coap.start();
}

void loop() {
  root["name"] = "temperature";
  root["data"] = 21.5;
  //root["accessToken"] =  DEVICE_SECRET_KEY;
  bool state;
  h = dht.readHumidity();
  t = dht.readTemperature();  
  h = 40;
  t = 17;
  //String data;
  //serializeJson(root,data);
  String data = "h:";
  data = data + h;
  data = data + "t:";
  data = data + t;
  char dataChar[data.length() + 1];
  data.toCharArray(dataChar, data.length() + 1);

  //post request
  //arguments server ip address,default port,resource name, payload,payloadlength
  int msgid = coap.put(ip, port, path, dataChar, data.length()+1);
  Serial.println(dataChar);
  Serial.println(data.length()+1);
  Serial.println(msgid);
  state = coap.loop();

  delay(5000);
}