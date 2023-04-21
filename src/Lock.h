#ifndef LOCK_H
#define LOCK_H

#include <Arduino.h>
#include <Stepper.h>

#define STEPS 2048
#define UNLOCKTIME 5000

class Lock {
public:
  Lock(int stepsPerRevolultion, int IN1, int IN2, int IN3, int IN4);
  void unlock();
  void update();
  bool isLocked();
  
private:
    Stepper _stepper;
    bool _locked;
    unsigned long _unlockTime;
};
#endif // LOCK_H
