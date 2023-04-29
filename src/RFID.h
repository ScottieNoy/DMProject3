#ifndef RFID_H
#define RFID_H

#define MAXTAGS 5
#define NUMBYTES 4
#include <Lock.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LCD.h>

extern LCD lcd;

class RFID {
public:

    RFID(Lock& lock, int SS_pin, int RST_pin);
    bool findTag(byte *uid, int uidSize);
    void removeTag(byte *uid, int uidSize);
    String getNUID(byte *uid, int uidSize);
    void grantAccess(byte *uid, int uidSize);
    void removeAccess(byte *uid, int uidSize);
    void RMain();
    void addTag(byte *uid, int uidSize);
    bool tagAllowed(byte *uid, int uidSize);
    void updateAccess();
private:
    int _custIns;
    byte _allowedTags[MAXTAGS][NUMBYTES];
    byte _existingTags[MAXTAGS][NUMBYTES];
    byte _lastScanned[NUMBYTES];
    Lock& _lock;
    MFRC522 _rfid;
    int _time;
};

#endif // RFID_H