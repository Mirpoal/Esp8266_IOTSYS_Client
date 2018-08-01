#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

const char* ssid     = "ESP_AP"; 
const char* password = "hmsys@45678"; 
const char* host   = "192.168.0.1"; 
String path          = "/api/sensor";
WiFiClient client;

const int pin        = 2;

void setup() {
  pinMode(pin, OUTPUT);
  Serial.begin(9600);
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");  
  Serial.println(WiFi.localIP()); 
}

void loop() {
  const int httpPort = 3001;
  if(!client.connect(host, httpPort)) {  
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: keep-alive\r\n\r\n");
  while(client.available()){
    
  }
}
