#ifndef endstops_h
#define endstops_h
#include "A_config.h"
#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}
#include "src/SimpleButton/SimpleButton.h"
extern uint32_t currentTime;
extern servoCfg SRV[BLINDS];
extern endstopCfg END[BLINDS];


class EndStops {
    public:
      void setup();
      void update();
      bool top(byte s);
      bool bottom(byte s);
      EndStops();
      ~EndStops();
    private:
      simplebutton::Button* TopEndStop[BLINDS];
      simplebutton::Button* BottomEndStop[BLINDS];
      bool TopEnd[BLINDS];
      bool BottomEnd[BLINDS];
      uint32_t TopEndTime[BLINDS];
      uint32_t BottomEndTime[BLINDS];

};

#endif
