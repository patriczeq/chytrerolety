#include "hwui.h"

HWUI::HWUI() {}
HWUI::~HWUI() {}

void HWUI::setup(){
  #ifdef USE_PCF8574
    Wire.begin();
    this->exp = new simplebutton::PCF8574(PCF8574_ADDR);
    this->i2cList();
  #endif

  for(byte s = 0; s < BLINDS; s++)
    {
      this->cycleDOWN[s]  = motors.CYCLE::SLOW_DOWN;
      this->cycleUP[s]    = motors.CYCLE::SLOW_UP;
        
      #ifdef USE_PCF8574
        this->down[s] = new simplebutton::ButtonPullupGPIOExpander(this->exp, BTN[s].down);
        this->up[s] = new simplebutton::ButtonPullupGPIOExpander(this->exp, BTN[s].up);
      #endif
      
      #ifndef USE_PCF8574
        this->down[s] = new simplebutton::ButtonPullup(BTN[s].down);
        this->up[s] = new simplebutton::ButtonPullup(BTN[s].up);
      #endif

      /**
       * Pomalý pohyb celého cyklu
       */
      this->down[s]->setOnClicked([this, s, motors](){
        motors.runCycle(s, this->cycleDOWN[s]);
      });
      this->up[s]->setOnClicked([this, s](){
        motors.runCycle(s, this->cycleUP[s]);
      });
      /**
       * Ruční ovládání
       */
      this->down[s]->setOnHolding(
        [this, s, motors](){

        }, 
        250
      );
      this->up[s]->setOnHolding(
        [this, s, motors](){
          
          
        }, 
        250
      );
    }
}

void HWUI::update(){
 for(byte s = 0; s < BLINDS; s++)
    {
      this->down[s]->update();
      this->up[s]->update();
    }
}


void HWUI::i2cList(){
    byte error, address;
    int nDevices;
    nDevices = 0;
    for (address = 1; address < 127; address++ )  {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0) {
        Serial.print(String(F("[I2C]: FOUND: 0x")));
        if (address < 16)
          Serial.print("0");
        Serial.println(String(address, HEX));
        nDevices++;
      } else if (error == 4) {
        Serial.println(String(F("[I2C]: ERROR: 0x")));
        if (address < 16)
          Serial.print("0");
        Serial.println(String(address, HEX));
      }
    } //for loop
    if (nDevices == 0) {
      Serial.println(String(F("[I2C]: NO DEVICES FOUND!")));
    }
}
