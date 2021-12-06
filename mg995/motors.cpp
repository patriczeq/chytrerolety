#include "motors.h"

Motors::Motors() {}
Motors::~Motors() {}

void Motors::setup(){
  #ifdef SEARCH_CENTER
    this->searchMidPoint(0);
    this->searchMidPoint(1);
  #endif
  for(byte s = 0; s < BLINDS; s++)
    {
      for(byte c = 0; c < 4; c++)
        {
          this->Cycles[s][c] = this->_Cycles[c];
        }
      this->relativePosition[s] = POSITION::UNKNOWN;
      this->runningCycle[s] = 0;
    }
 
}

void Motors::update(){
  for(byte s = 0; s < BLINDS; s++)
    {
      /**
       *  Logika zastavení motoru
       *  - vypršel čas
       *  - pozice dosáhla maxima
       *  - pozice dosáhla minima
       */
      if(this->targetTime[s] < currentTime && this->spin[s] != SPIN::NONE)
        {
          this->stop(s, STOP_CAUSE::TIMEOUT); // Timeout
        }
      #ifdef USE_ENDSTOPS
      else if(this->spin[s] == SPIN::DOWN && endstops.bottom(s))
        {
          this->stop(s, STOP_CAUSE::BOTTOM_END); // Blokuje spodní EndStop
        }
      else if(this->spin[s] == SPIN::UP && endstops.top(s))
        {
          this->stop(s, STOP_CAUSE::TOP_END); // Blokuje horní EndStop
        }
      #endif
        
      if(this->targetTime[s] < currentTime && (currentTime - this->targetTime[s]) >= this->motorsTimeout) // Vypnout motor po neaktivitě
        {
          this->servoDetach(s);
        }
    }
}

bool Motors::isMoving(){
  for(byte s = 0; s < BLINDS; s++)
    {
      if(this->spin[s])
        {
          return true;
        }
    }
  return false;
}

bool Motors::isMoving(byte s){
  return this->spin[s] != SPIN::NONE;
}

byte Motors::getSpin(byte s){
  return this->spin[s];
}
      
void Motors::rotate(byte s, byte dir, byte speed){
  this->servoAttach(s);
  this->spin[s] = dir;
  int calcdeg = SRV[s].center + (this->spin[s] == 1 ? (speed * -1) : speed);
  
  if(this->spin[s] == SPIN::DOWN && SRV[s].center > 90)
    {
      calcdeg += SRV[s].center - 90;
    }
    
  if(this->spin[s] == SPIN::UP && SRV[s].center < 90)
    {
      calcdeg -= 90 - SRV[s].center;
    }
  this->servo[s].write(calcdeg > 180 ? 180 : (calcdeg >= 0 ? calcdeg : 0));
}
void Motors::stop(byte s, byte cause){
   if(this->spin[s] == SPIN::DOWN)
     {
      this->servo[s].write(SRV[s].center - 1);
     }
   else if(this->spin[s] == SPIN::UP)
     {
      this->servo[s].write(SRV[s].center + 1);
     }
  this->servo[s].write(SRV[s].center);
  this->spin[s] = SPIN::NONE;
   Serial.print("S" + String(s) + "->STOP_CAUSE: ");
   switch(cause){
     case STOP_CAUSE::SYSTEM:
        Serial.println("SYSTEM");
        break;
     case STOP_CAUSE::TIMEOUT:
        // finished cycle
        if(this->runningCycle[s] > 0)
          {
            if(this->runningCycle[s] - 1 == CYCLE::SLOW_DOWN || this->runningCycle[s] - 1 == CYCLE::FAST_DOWN)
              {
                this->relativePosition[s] = POSITION::BOTTOM;
              }
             else if(this->runningCycle[s] - 1 == CYCLE::SLOW_UP || this->runningCycle[s] - 1 == CYCLE::FAST_UP)
              {
                this->relativePosition[s] = POSITION::TOP;
              }
          }
        Serial.println("TIMEOUT");
        break;
     case STOP_CAUSE::BOTTOM_END:
        this->relativePosition[s] = POSITION::BOTTOM;
        this->absolutePosition[s] = POSITION::BOTTOM;
        Serial.println("BOTTOM_END");
        break;
     case STOP_CAUSE::TOP_END:
       this->relativePosition[s] = POSITION::TOP;
       this->absolutePosition[s] = POSITION::TOP;
        Serial.println("TOP_END");
        break;
     case STOP_CAUSE::POSITION_BOTTOM:
        Serial.println("RLTV: POSITION_BOTTOM");
        break;
     case STOP_CAUSE::POSITION_TOP:
        Serial.println("RLTV: POSITION_TOP");
        break;
   }
   this->targetTime[s] = 0;
   this->runningCycle[s] = 0;
}

void Motors::stop(byte s){
  this->stop(s, STOP_CAUSE::SYSTEM);
}
void Motors::stop(){
  for(byte s = 0; s < BLINDS; s++)
    {
      this->stop(s);
    }
}

void Motors::servoAttach(byte s){
  if(!this->servoAttached[s]){
    this->servo[s].attach(SRV[s].pwm);
    this->servoAttached[s] = true;
  }
}
void Motors::servoDetach(byte s){
  if(this->servoAttached[s]){
    this->servo[s].detach();
    this->servoAttached[s] = false;
    digitalWrite(SRV[s].pwm, HIGH);
  }
}

void Motors::down(byte s, uint32_t duration, byte speed){
  if(this->spin[s] == SPIN::UP){
      this->stop(s, STOP_CAUSE::SYSTEM);
      return;
    }
  #ifdef USE_ENDSTOPS
   else if(endstops.bottom(s)){
      this->stop(s, STOP_CAUSE::BOTTOM_END);
      return;
    }
  #endif
  else if(this->relativePosition[s] == POSITION::BOTTOM){
      this->stop(s, STOP_CAUSE::POSITION_BOTTOM);
      return;
    }
  this->targetTime[s] = currentTime + duration;
  this->rotate(s, SPIN::DOWN, speed);
  Serial.println("down " + String(s) + ", timeout: " + String(duration));
}

void Motors::up(byte s, uint32_t duration, byte speed){
  if(this->spin[s] == SPIN::DOWN)
    {
      this->stop(s, STOP_CAUSE::SYSTEM);
      return;
    }
  #ifdef USE_ENDSTOPS
   else if(endstops.top(s)){
      this->stop(s, STOP_CAUSE::TOP_END);
      return;
    }
  #endif
  else if(this->relativePosition[s] == POSITION::TOP){
      this->stop(s, STOP_CAUSE::POSITION_TOP);
      return;
    }
  this->targetTime[s] = currentTime + duration;
  this->rotate(s, SPIN::UP, speed);
  Serial.println("up " + String(s) + ", timeout: " + String(duration));
}

void Motors::updown(byte s, byte sp, uint32_t duration, byte speed){
  this->runningCycle[s] = 0;
  if(sp == SPIN::DOWN)
    {
      this->down(s, duration, speed);
    }
  else if(sp == SPIN::UP)
    {
      this->up(s, duration, speed);
    }
}

void Motors::runCycle(byte s, byte c){
  this->runningCycle[s] = c + 1;
  if(this->Cycles[s][c].spin == SPIN::DOWN)
    {
      this->down(s, this->Cycles[s][c].duration, this->Cycles[s][c].speed);
    }
  else if(this->Cycles[s][c].spin == SPIN::UP)
    {
      this->up(s, this->Cycles[s][c].duration, this->Cycles[s][c].speed);
    }

}



byte Motors::searchMidPoint(byte s){
  Servo ser;
  byte pos = 85;
  ser.attach(SRV[s].pwm);
  pinMode(0, INPUT_PULLUP);
  digitalWrite(0, HIGH);
  Serial.println("Roztacim servo " + String(s) + " na pinu " + String(SRV[s].pwm) + ". Jakmile se servo uplne zastavi, stiskni tlacitko FLASH.");
  delay(1000);
  while(pos < 110){
    Serial.println("pozice: " + String(pos));
    ser.write(pos);
    delay(1000);
    if(!digitalRead(0)){
      Serial.println("Stredova pozice je: " + String(pos));
      break;
    }
    pos++;
  }
  ser.detach();
  SRV[s].center = pos;
  return pos;
}
