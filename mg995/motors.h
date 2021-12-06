#ifndef motors_h
#define motors_h

#include "A_config.h"
#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}
#include <Servo.h>
#include "src/SimpleButton/SimpleButton.h"

extern uint32_t currentTime;
extern servoCfg SRV[BLINDS];

#ifdef USE_ENDSTOPS
#include "endstops.h"
extern EndStops endstops;
#endif

/*

plán kalibrace:
pojede nahoru dokud nezastaví o TopEnd
Pojede dolu, nastaví se millis u TopEndTime
jakmile dojede na BottomEnd nastaví se millis u BottomEndTime

DutyTotalTime = BottomEndTime - TopEndTime;

Při použití stejný rychlosti motoru můžu dopočítat % polohy -> další VAR na přičítání/odečítání pojezdu

*/

class Motors {
    public:
      void setup();
      void update();
      void down(byte s, uint32_t duration = 1000, byte speed = 20);
      void up(byte s, uint32_t duration = 1000, byte speed = 20);
      void updown(byte s, byte sp, uint32_t duration = 1000, byte speed = 20);
      void stop(byte s, byte cause);
      void stop(byte s);
      void stop();
      bool isMoving();
      bool isMoving(byte s);
      byte getSpin(byte s);
      enum STOP_CAUSE {
        SYSTEM  = 1,
        TIMEOUT = 2,
        BOTTOM_END = 3,
        TOP_END = 4,
        POSITION_BOTTOM = 5,
        POSITION_TOP = 6
      };

      enum SPIN {
        NONE = 0,
        DOWN = 1,
        UP   = 2
      };

      enum POSITION {
        UNKNOWN = 0,
        BOTTOM = 1,
        TOP = 2
      };

      enum SPEED {
        ZERO  = 0,
        SLOW  = 4,
        FAST  = 80
      };

      enum CYCLE {
        SLOW_DOWN = 0,
        SLOW_UP   = 1,
        FAST_DOWN = 2,
        FAST_UP   = 3
      };

      void runCycle(byte s, byte c);

      Motors();
      ~Motors();
    private:
      Servo servo[BLINDS];
      bool servoAttached[BLINDS];
      uint32_t targetTime[BLINDS];
      byte StartTime[BLINDS];
      byte spin[BLINDS];
      uint16_t motorsTimeout                  = SERVO_TIMEOUT;
      void servoAttach(byte s);
      void servoDetach(byte s);
      void rotate(byte s, byte dir, byte speed);
      byte searchMidPoint(byte s);
      byte relativePosition[BLINDS]; // 0 - unknown, 1 - bottom, 2 - top
      byte absolutePosition[BLINDS]; // 0 - unknown, 1 - bottom, 2 - top
      byte runningCycle[BLINDS]; // -1 shift - 0 => none
      CycleProfile _Cycles[4] = {
          {SPIN::DOWN,  SPEED::SLOW,  2000},
          {SPIN::UP,    SPEED::SLOW,  2000},
          {SPIN::DOWN,  SPEED::FAST,  2000},
          {SPIN::UP,    SPEED::FAST,  2000}
        };
      CycleProfile TrainCycle[BLINDS];
      CycleProfile Cycles[BLINDS][4];   // Čas celého cyklu při směru a rychlosti



};

#endif
