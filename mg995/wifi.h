#ifndef wifi_h
#define wifi_h

#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}

#include "A_config.h"
#include "motors.h"
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <DNSServer.h>          // DNS server
#include <ESP8266mDNS.h>        // mDNS

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

extern uint32_t currentTime;
extern String str(const char* ptr);

extern Motors    motors;

class WIFI {
    public:
      void setup();
      void update();
      bool connected = false;
      WIFI();
      ~WIFI();
    private:
      AsyncWebServer server = AsyncWebServer(80);
      void startServer();
      void sendToMotor(byte s, byte spin, uint32_t duration, byte speed);
    
 };
 #endif
