#include "wifi.h"
#include "progmem_wifi.h"
WIFI::WIFI() {}

WIFI::~WIFI() {}
ESP8266WiFiMulti wifiMulti;
DNSServer dns;

void WIFI::setup() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  WiFi.softAPdisconnect(true);
  WiFi.hostname(HOSTNAME);
  WiFi.begin(ST_SSID, ST_PASS);
  this->startServer();
}

void WIFI::update() {
  if (WiFi.status() == WL_CONNECTED && !this->connected){
    this->connected = true;
    Serial.println("WiFi connected...");
    MDNS.begin(HOSTNAME);
  }
  MDNS.update();
  dns.processNextRequest();
}

void WIFI::startServer() {
  server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){ request->send_P(200, str(W_HTML).c_str(), file_index_html); });
  server.on("/down", HTTP_GET, [this](AsyncWebServerRequest *request){ 
    motors.down(0, 20000, 6);
    request->send(200, str(W_JSON).c_str(), "down"); 
  });
  server.on("/up", HTTP_GET, [this](AsyncWebServerRequest *request){ 
    motors.up(0, 20000, 6);
    request->send(200, str(W_JSON).c_str(), "up"); 
  });

  server.on("/test", HTTP_GET, [this](AsyncWebServerRequest *request){ 
    if(request->getParam("d")->value().toInt() == 1){
      motors.down(
        request->getParam("s")->value().toInt(), 
        20000, 
        request->getParam("sp")->value().toInt()
      );
    }
    else
    {
      motors.up(
        request->getParam("s")->value().toInt(), 
        20000, 
        request->getParam("sp")->value().toInt()
      );
    }
    request->send(200, str(W_JSON).c_str(), "cust"); 
  });

  this->server.begin();
}
