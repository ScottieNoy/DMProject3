#include "Lock.h"

Lock::Lock(int stepsPerRevolultion, int IN1, int IN2, int IN3, int IN4)
    : _stepper(stepsPerRevolultion, IN1, IN2, IN3, IN4), _locked(true) {
        _stepper.setSpeed(15);
    }

void Lock::unlock() {
    if (_locked) {
        _locked = false;
        _stepper.step(STEPS);
        _unlockTime = millis();
    }
}

void Lock::update() {
    if (!_locked && (millis() - _unlockTime) >= UNLOCKTIME) {
        _stepper.step(-STEPS);
        _locked = true;
    } else {

    }
}

bool Lock::isLocked() {
    return _locked;
}