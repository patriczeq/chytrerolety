#include "endstops.h"

EndStops::EndStops() {}
EndStops::~EndStops() {}

void EndStops::setup(){
  for(byte s = 0; s < BLINDS; s++)
    {
      /**
       * Namontování EndStopů
       */
      this->TopEndStop[s] = new simplebutton::ButtonPullup(END[s].top);
      this->BottomEndStop[s] = new simplebutton::ButtonPullup(END[s].bottom);
      
      this->TopEndStop[s]->setOnPushed([this, s](){this->TopEnd[s] = true;});
      this->BottomEndStop[s]->setOnPushed([this, s](){this->BottomEnd[s] = true;});
      this->TopEndStop[s]->setOnReleased([this, s](){this->TopEnd[s] = false;});
      this->BottomEndStop[s]->setOnReleased([this, s](){this->BottomEnd[s] = false;});
    }
    
}

void EndStops::update(){
  for(byte s = 0; s < BLINDS; s++)
    {
      this->TopEndStop[s]->update();
      this->BottomEndStop[s]->update();
    }
}

bool EndStops::top(byte s){
  return this->TopEnd[s];
}
bool EndStops::bottom(byte s){
  return this->BottomEnd[s];
}
