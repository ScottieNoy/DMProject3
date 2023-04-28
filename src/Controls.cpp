#include <Controls.h>
#include <SPI.h>
#include <arduino.h>

Controls::Controls(int potPin,int butPin, Lock& lock, RFID& rfid) : _rfid(rfid), _lock(lock){
    _potPin = potPin;
    _butPin = butPin;
    pinMode(_butPin, INPUT);
}

void Controls::adjustMax(){
    int potValue = map(analogRead(_potPin), 0, 4095, 0, 3);
    _previousMax = _lock.getMax();
    if (_previousMax != potValue){
        _lock.updateMax(potValue);
        Serial.println("Max changed to: " + String(potValue));
    }
    _interrupt_time = millis();
    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (_interrupt_time - _last_interrupt_time > _debounceDelay) {
        if (digitalRead(_butPin) == HIGH) {
            _last_interrupt_time = _interrupt_time;
            _rfid.updateAccess();
        }
    }
}