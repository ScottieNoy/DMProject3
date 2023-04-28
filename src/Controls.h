#include <SPI.h>
#include <RFID.h>
#include <Lock.h>
class Controls{
public:
    Controls(int inp,int butPin, Lock& lock, RFID& rfid);
    void adjustMax();

private:
    int _potPin;
    int _butPin;
    RFID& _rfid;
    Lock& _lock;
    int _previousMax;
    unsigned long _last_interrupt_time = 0;
    unsigned long _interrupt_time;
    const unsigned long _debounceDelay = 200;

};