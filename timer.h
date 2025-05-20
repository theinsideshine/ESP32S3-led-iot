#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"

class CTimer
{
  public:
    CTimer();
    void start();
    bool expired(uint32_t);

  private:
      uint32_t timer;
};

#endif // TIMER_H
