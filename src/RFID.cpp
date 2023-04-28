#include "RFID.h"
#include <SPI.h>
#include <MFRC522.h>
#include "Lock.h"



RFID::RFID(Lock& lock, int SS_PIN, int RST_PIN) : _rfid(SS_PIN, RST_PIN), _lock(lock) {
    _custIns = 0;
    SPI.begin();
    _rfid.PCD_Init();
}


bool RFID::findTag(byte *uid, int uidSize) {
  for (int i = 0; i < _custIns; i++) {
    bool found = true;
    for (int j = 0; j < uidSize; j++) {
      if (_existingTags[i][j] != uid[j]) {
        found = false;
        break;
      }
    }
    if (found) {
      return true;
    }
  }
  return false;
}

// removes tag when the same chip is read again
void RFID::removeTag(byte *uid, int uidSize) {
  for (int i = 0; i < _custIns; i++) {
    bool found = true;
    for (int j = 0; j < uidSize; j++) {
      if (_existingTags[i][j] != uid[j]) {
        found = false;
        break;
      }
    }
    if (found) {
      for (int k = i; k < _custIns - 1; k++) {
        memcpy(_existingTags[k], _existingTags[k + 1], uidSize);
      }
      _custIns--;
      break;
    }
  }
}

void RFID::printNUID(byte *uid, int uidSize) {
  for (int i = 0; i < uidSize; i++) {
    if (uid[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(uid[i], HEX);
    if (i < uidSize - 1) {
      Serial.print(":");
    }
  }
  Serial.println();
}

// Add a new tag to the existingTags array
void RFID::addTag(byte *uid, int uidSize) {
  memcpy(_existingTags[_custIns], uid, uidSize);
  _custIns++;
}

// Grant access and control the stepper motor


void RFID::RMain() {
  if (!_rfid.PICC_IsNewCardPresent()) return;
  if (!_rfid.PICC_ReadCardSerial()) return;

  Serial.print("UID tag: ");
  printNUID(_rfid.uid.uidByte, _rfid.uid.size);

  if (!tagAllowed(_rfid.uid.uidByte, _rfid.uid.size)) {
    Serial.println("Access denied. Tag not allowed.");
    _rfid.PICC_HaltA();
    _rfid.PCD_StopCrypto1();
    return;
  }

  if (findTag(_rfid.uid.uidByte, _rfid.uid.size)) {
    removeTag(_rfid.uid.uidByte, _rfid.uid.size);
    Serial.println("User left. Tag removed from list.");
    Serial.print("Customers inside: ");
    Serial.println(_custIns);
    _lock.unlock();

  } else if (_custIns < _lock.getMax()) {
    addTag(_rfid.uid.uidByte, _rfid.uid.size);
    Serial.println("Access granted.");
    Serial.print("Customers inside: ");
    Serial.println(_custIns);
    _lock.unlock();

  } else {
    Serial.println("Access denied. Maximum number of customers reached.");
    Serial.println(_lock.getMax());

  }

  _rfid.PICC_HaltA();
  _rfid.PCD_StopCrypto1();
}

bool RFID::tagAllowed(byte *uid, int uidSize) {
  for (int i = 0; i < MAXTAGS; i++) {
    bool found = true;
    for (int j = 0; j < uidSize; j++) {
      if (_allowedTags[i][j] != uid[j]) {
        found = false;
        break;
      }
    }
    if (found) {
      return true;
    }
  }
  return false;
}

void RFID::grantAccess(byte *uid, int uidSize) {
  memcpy(_allowedTags[_custIns], uid, uidSize);
}

void RFID::removeAccess(byte *uid, int uidSize) {
  for (int i = 0; i < MAXTAGS; i++) {
    bool found = true;
    for (int j = 0; j < uidSize; j++) {
      if (_allowedTags[i][j] != uid[j]) {
        found = false;
        break;
      }
    }
    if (found) {
      for (int k = i; k < MAXTAGS - 1; k++) {
        memcpy(_allowedTags[k], _allowedTags[k + 1], uidSize);
      }
      break;
    }
  }
}

void RFID::updateAccess() {
  Serial.println("Place a tag to update access.");
  _time = millis();
  while(millis()-_time < 5000) {
    if (_rfid.PICC_IsNewCardPresent() && _rfid.PICC_ReadCardSerial()){
      Serial.print("UID tag: ");
      printNUID(_rfid.uid.uidByte, _rfid.uid.size);
      if (findTag(_rfid.uid.uidByte, _rfid.uid.size)) {
        Serial.println("Sign out in order to remove access.");
        _rfid.PICC_HaltA();
        _rfid.PCD_StopCrypto1();
        return;
      }
      if (tagAllowed(_rfid.uid.uidByte, _rfid.uid.size)) {
        removeAccess(_rfid.uid.uidByte, _rfid.uid.size);
        Serial.println("Key card removed.");
      } else {
        grantAccess(_rfid.uid.uidByte, _rfid.uid.size);
        Serial.println("Key card added.");
      }
      _rfid.PICC_HaltA();
      _rfid.PCD_StopCrypto1();
      return;
    }
    
  }
  Serial.print("Time limit exceeded, press button again to add/remove access.");
}