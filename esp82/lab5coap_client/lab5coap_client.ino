#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>
#include <DHT.h>
#define DHT_PIN 4
#define DHTTYPE DHT11

const char* ssid     = "DukeV2";
const char* password = "Duke201303";

IPAddress ip(192, 168, 1, 2);
int port = 5683;

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port);

// CoAP server endpoint url callback
void callback_light(CoapPacket &packet, IPAddress ip, int port);

// UDP and CoAP class
// other initialize is "Coap coap(Udp, 512);"
// 2nd default parameter is COAP_BUF_MAX_SIZE(defaulit:128)
// For UDP fragmentation, it is good to set the maximum under
// 1280byte when using the internet connection.
WiFiUDP udp;
Coap coap(udp);

// LED STATE
bool LEDSTATE;
char* path = "/temperature";

// CoAP server endpoint URL
void callback_light(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Light] ON/OFF");
  
  // send response
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  String message(p);

  if (message.equals("0"))
    LEDSTATE = false;
  else if(message.equals("1"))
    LEDSTATE = true;
      
  if (LEDSTATE) {
    //digitalWrite(9, HIGH) ; 
    Serial.println("Light High");
    Serial.println(ip);
    Serial.println(port);    
    coap.sendResponse(ip, port, packet.messageid);
    //coap.sendResponse(ip, port, packet.messageid, packet.payloadlen + 1, 1, COAP_CREATED, COAP_TEXT_PLAIN, p, packet.payloadlen + 1);
  } else { 
    //digitalWrite(9, LOW) ; 
    Serial.println("Light Low");
    Serial.println(ip);
    Serial.println(port);
    coap.sendResponse(ip, port, packet.messageid);              
    //coap.sendResponse(ip, port, packet.messageid, packet.payloadlen + 1, 1, COAP_CREATED, COAP_TEXT_PLAIN, p, packet.payloadlen + 1);
  }
}

void callback_temperature(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("Temperature");
  
  // send response
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  String message(p);

  if (message.equals("0"))
    LEDSTATE = false;
  else if(message.equals("1"))
    LEDSTATE = true;
      
  if (LEDSTATE) {
    Serial.println("temperature high");  
    Serial.println(ip);
    Serial.println(port);       
    //coap.sendResponse(ip, port, packet.messageid, "1");
    coap.sendResponse(ip, port, packet.messageid, "1", 1, COAP_CREATED, COAP_TEXT_PLAIN);      
  } else { 
    Serial.println("temperature Low");
    Serial.println(ip);
    Serial.println(port);                 
    //coap.sendResponse(ip, port, packet.messageid, "0");
    coap.sendResponse(ip, port, packet.messageid, "0", 1, COAP_CREATED, COAP_TEXT_PLAIN, (uint8_t*)1234, 4);       
  }
}

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // LED State
  //pinMode(9, OUTPUT);
  //digitalWrite(9, HIGH);
  LEDSTATE = true;
  
  // add server url endpoints.
  // can add multiple endpoint urls.
  // exp) coap.server(callback_switch, "switch");
  //      coap.server(callback_env, "env/temp");
  //      coap.server(callback_env, "env/humidity");
  Serial.println("Setup Callback Light");
  coap.server(callback_light, "light");
  coap.server(callback_temperature, "temperature");

  // client response callback.
  // this endpoint is single callback.
  Serial.println("Setup Response Callback");
  coap.response(callback_response);

  // start coap server/client
  coap.start();
}

void loop() {
  delay(1000);

  String data = "Hola";
  char dataChar[data.length() + 1];
  data.toCharArray(dataChar, data.length() + 1);

  //int msgid = coap.put(IPAddress(192,168,174,2), 5683, path, dataChar, data.length());
  //Serial.println("Message Id: "); 
  //Serial.println(msgid);
  coap.loop();
}