#include <SoftwareSerial.h>
#include "Udp.h"
#define ESP_baudrate   115200
#define rxpin  4
#define txpin  5
SoftwareSerial ESP(rxpin,txpin);
String mode_STA = "AT+CWMODE=1";
String connectwifi = "AT+CWJAP=\"航模实验室\",\"hmsys@45678\"";
String connectserver = "AT+CIPSTART = \"UDP\",\"192.168.1.105\",5683";
String sendData = "AT+CIPSEND=";

String mac = "81:4C:AA:3A:C6:AB";
bool LEDSTATE;

void get_response() {
  String str="";
  char c;
  while(!ESP.available());
  while(ESP.available()) {
    c=ESP.read(); 
    str.concat(c); 
    delay(10);
  }
  str.trim();
  Serial.println(str);
}

void esp8266_send_cmd(String cmd) {
  ESP.println(cmd);
  get_response();
}

void esp8266_send_data(String data) {
  String senddata = sendData + String(data.length());
  Serial.println("Send data: ");
  ESP.println(senddata);
  get_response();
}

void setup() {
  Serial.begin(115200);
  ESP.begin(ESP_baudrate);
  Serial.println("set STA Mode......");
  esp8266_send_cmd(mode_STA);
  Serial.println("connect WIFI......");
  esp8266_send_cmd(connectwifi);
  esp8266_send_cmd(connectserver);
  Serial.println("connected to server");
}

void loop() {
  String payload = "{\"state\":"+ (String)LEDSTATE +",\"mac\":\""+ mac + "\"}";
  esp8266_send_data(payload);
}
