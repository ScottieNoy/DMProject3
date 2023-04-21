#ifndef LOCK_H
#define LOCK_H

#include <Arduino.h>
#include <Stepper.h>


class Lock {
public:
  Lock(int steptPerRevolution, int p1, int p2, int p3, int p4);
  void unlock();
  
  
  
private:
    Stepper _stepper;
    int _currentTime;
};

#endif // LOCK_H