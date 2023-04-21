#include "Lock.h"

Lock::Lock(int stepsPerRevolultion, int IN1, int IN2, int IN3, int IN4)
    : _stepper(stepsPerRevolultion, IN1, IN2, IN3, IN4) {
        _stepper.setSpeed(15);
    }

void Lock::unlock() {
    _stepper.step(2000);
    // timer
    delay(5000);
    _stepper.step(-2000);
}