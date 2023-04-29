#ifndef LOCK_H
#define LOCK_H

#include <Arduino.h>
#include <Stepper.h>
#include <LCD.h>


#define STEPS 2048
#define UNLOCKTIME 5000

extern LCD lcd;

class Lock {
public:
  Lock(int stepsPerRevolultion, int IN1, int IN2, int IN3, int IN4);
  void unlock();
  void update();
  bool isLocked();
  void updateMax(int maxCustomers);
  int getMax();
private:
    Stepper _stepper;
    bool _locked;
    unsigned long _unlockTime;
    int _maxCustomers;
};
#endif // LOCK_H
