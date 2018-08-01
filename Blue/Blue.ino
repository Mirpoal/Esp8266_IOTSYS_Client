#include <SerialESP8266wifi.h>
#include <SoftwareSerial.h>

const char* ssid     = "航模实验室";
const char* password = "hmsys@45678";
const char* host = "192.168.1.105";
const int Port = 1883;

uint8_t wifi_started = false;
#define esp8266_reset_pin 5

SoftwareSerial BT(8, 9);           //bluetooch tx rx
SoftwareSerial swSerial(6, 7);     //esp8266 tx rx

SerialESP8266wifi wifi(Serial, swSerial, esp8266_reset_pin);

char val; 

void setup() {
  Serial.begin(9600);  
  swSerial.begin(9600);
  BT.begin(9600);  
  Serial.println("Starting wifi");
  swSerial.println("Starting wifi");
  wifi.setTransportToTCP(); 
  wifi.endSendWithNewline(false);
  
  wifi_started  = wifi.begin();
  if(wifi_started) {
    wifi.connectToAP(ssid, password);
    //wifi.connectToServer(host, Port);
  }else{
    Serial.println("ESP8266 isn't working..");
  }
}

void loop() {
  static WifiConnection *connections;
  if (wifi_started)
    wifi.checkConnections(&connections);
  delay(5000);
  while(!(wifi.connectToServer(host, Port))) {
     Serial.println("connection failed");
     return;
  }
  String url = "/api/sensor";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  String data = "";
  if (BT.available()) {
    val = BT.read();
    Serial.println(val);
    data = "{\"JSONdata\":{\"beam\""+ (String)val +"}";
  }
  if(wifi.send(SERVER, data)) {
    Serial.println("Send to server");
  }
//  String putRequest = String("PUT ") + url + " HTTP/1.1\r\n" +
//                     "Content-Type: application/json;charset=utf-8\r\n" +
//                     "Host: " + host + ":" + httpPort + "\r\n" +  
//                     "Content-Length: " + length + "\r\n" +  
//                     "Connection: Keep Alive\r\n\r\n" +  
//                     data+"\r\n";
//  Serial.println(putRequest);                   
  
}
