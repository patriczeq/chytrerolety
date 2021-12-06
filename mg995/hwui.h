#ifndef hwui_h
#define hwui_h
#include "A_config.h"
#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}
#include <Wire.h>
#include "src/SimpleButton/SimpleButton.h"
#include "motors.h"
extern uint32_t currentTime;
extern buttonCfg BTN[BLINDS];
extern Motors    motors;


class HWUI {
    public:
      void setup();
      void update();
      void i2cList();
      void setCycleDown(byte s, byte c);
      void setCycleUp(byte s, byte c);
      HWUI();
      ~HWUI();
    private:
        #ifdef USE_PCF8574
        simplebutton::GPIOExpander* exp;
        #endif
        simplebutton::Button* down[BLINDS];
        simplebutton::Button* up[BLINDS];
        byte cycleDOWN[BLINDS];
        byte cycleUP[BLINDS];
};

#endif
