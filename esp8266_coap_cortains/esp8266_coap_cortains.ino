#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <coap.h>

const char* ssid     = "ESP_AP";
const char* password = "hmsys@45678";

String mac= "";
byte esp_mac[6]; 
String pwd="bishe456789";

#define LEDPin 0

void callback_response(CoapPacket &packet, IPAddress ip, int port);

WiFiUDP udp;
Coap coap(udp);

bool LEDSTATE;

void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);

  String message(p);
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);
  String state = root["state"];
  String jsonmac = root["mac"];
  if(jsonmac == mac) {
    if(state == "true") {
      LEDSTATE = 1;
      digitalWrite(LEDPin, LOW); 
      Serial.println("ON");
    }else{
      LEDSTATE = 0;
      digitalWrite(LEDPin, HIGH); 
      Serial.println("OFF");
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  WiFi.macAddress(esp_mac);
  mac = (String)esp_mac[0]+","+esp_mac[1]+","+esp_mac[2]+","+esp_mac[3]+","+esp_mac[4]+","+esp_mac[5];
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, HIGH);
  LEDSTATE = true;

  Serial.println("Setup Response Callback");
  coap.response(callback_response);
  
  coap.start();
}

void loop() {
  delay(1000);
  String payload = "{\"s\":"+ (String)LEDSTATE +",\"mac\":\""+ mac + "\"}";
  int msgid = coap.put(IPAddress(192,168,0,1), 5683, "light", (char*)payload.c_str());
  coap.loop();
}
