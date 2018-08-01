#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN  2
DHT dht(DHTPIN, DHTTYPE, 11);
float humidity, temp_f;

unsigned long previousMillis = 0; 
const long interval = 2000;
String webString="";

const char *ssid_ap = "NodeMCU_AP";
const char *password_ap = "bishe@45678";

const char* ssid_sta = "航模实验室";
const char* password_sta = "hmsys@45678";

ESP8266WebServer server_ap(80);
ESP8266WebServer server_sta(3001);    

void gettemperature() {
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp_f = dht.readTemperature(true);     // Read temperature as Fahrenheit
    if (isnan(humidity) || isnan(temp_f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}

void handleRoot() {
  gettemperature(); 
  server_ap.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  
  Serial.println("Configuring access point...");
  WiFi.softAP(ssid_ap, password_ap);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  WiFi.begin(ssid_sta, password_sta);
  Serial.println("Configuring NodeMCU STA...");
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid_sta);
  Serial.print("STA IP address: ");
  Serial.println(WiFi.localIP());

  server_ap.on("/", handleRoot);
  server_ap.begin();
  Serial.println("HTTP AP server started");

  server_sta.on("/temp", [](){  
    gettemperature();      
    webString="{Temperature: "+String((int)temp_f)+" F}";   
    server_sta.send(200, "text/plain", webString);            
  });
  server_sta.on("/humidity", [](){ 
    gettemperature();           
    webString="{Humidity: "+String((int)humidity)+"%}";
    server_sta.send(200, "text/plain", webString);              
  });
  server_sta.begin();
  Serial.println("HTTP STA server started");
}

void loop() {
  server_ap.handleClient();
  server_sta.handleClient();
}
