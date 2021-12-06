#include "A_config.h"
#include "motors.h"
#include "hwui.h"
#include "wifi.h"

Motors    motors;
HWUI      hwui;
WIFI      wifi;

uint32_t currentTime  = 0;

#ifdef USE_ENDSTOPS
  #include "endstops.h"
  EndStops  endstops;
#endif

servoCfg  SRV[BLINDS] = { SRV_0, SRV_1 };
buttonCfg BTN[BLINDS] = { BTN_0, BTN_1 };
#ifdef USE_ENDSTOPS
endstopCfg END[BLINDS] = { END_0, END_1 };
#endif


String str(const char* ptr) {
  char keyword[strlen_P(ptr)];

  strcpy_P(keyword, ptr);
  return String(keyword);
}

void setup() {
  Serial.begin(DEBUG_BAUD);
  delay(10);
  Serial.println("boot..");
  #ifdef USE_ENDSTOPS
    endstops.setup();
  #endif
  motors.setup();
  hwui.setup();
  wifi.setup();
}


void loop() {
  currentTime  = millis();
  #ifdef USE_ENDSTOPS
    endstops.update();
  #endif
  motors.update();
  hwui.update();
  wifi.update();
}
