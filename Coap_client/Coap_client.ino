#include <SoftwareSerial.h>
#include "Udp.h"

#define LED_Pin 7

#define ESP_rate 115200
#define ESP_RX 4
#define ESP_TX 5

SoftwareSerial ESP( ESP_RX, ESP_TX );

String connectwifi = "AT+CWJ=\"航模实验室\",\"hmsys@45678\"";
String connectserver = "AT+CIPSTART=\"UDP\",\"192.168.1.102\",5683";
String sendData = "AT+CIPSEND=64";
uint8_t *p;

void sendcmd(String cmd) {
  Serial.println("Send:  ");
  Serial.println(cmd);
  ESP.println(cmd);
}

String get_ESP8266_response() {
  String str = "";
  char c;
  while(ESP.available()) {
    c = ESP.read();
    str.concat(c);
    delay(10);
  }
  str.trim();
  return str;
}

String get_return() {
  while(!ESP.available());
  return get_ESP8266_response();
}

void connectServer() {
  sendcmd(connectserver);
  delay(300);
  String ret = get_return();
  Serial.println("ret:  "+ret);
}

void SendData(String Data) {
  String com = sendData+String(Data.length());
  sendcmd(com);
  delay(100);
  String ret = get_return();
  Serial.println("ret:  "+ret);
  sendcmd(Data);
  delay(1000);
  ret = get_return();
  Serial.println("ret:  "+ret);
}

void setup() {
  Serial.begin(115200);
  ESP.begin(ESP_rate);
  sendcmd(connectwifi);
  delay(3000);
  String ret = get_return();
  Serial.println("ret:  "+ret);
  connectServer();
  
  pinMode(LED_Pin,OUTPUT);
}

void loop() {
  Serial.write("LED:1");
  ESP.write("LED:1");
  delay(500);
  String ret = get_return();
  Serial.println("ret:  "+ret);
}
