#include "RFID.h"
#include <SPI.h>
#include <MFRC522.h>
#include "Lock.h"


RFID::RFID(Lock& lock, int SS_PIN, int RST_PIN) : _rfid(SS_PIN, RST_PIN), _lock(lock), _custIns(0) {
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



void RFID::removeTag(byte *uid, int uidSize) {
    for (int i = 0; i < MAXTAGS; i++) {
        if (memcmp(_existingTags[i], uid, uidSize) == 0) {
            // Shift remaining tags up
            for (int j = i; j < MAXTAGS - 1; j++) {
                memcpy(_existingTags[j], _existingTags[j + 1], uidSize);
            }

            // Clear the last tag since it's been shifted
            memset(_existingTags[MAXTAGS - 1], 0, uidSize);

            _custIns--; // Decrement the visitor count
            return;
        }
    }
}




String RFID::getNUID(byte *uid, int uidSize) {
  String nuid = "";
  for (int i = 0; i < uidSize; i++) {
    if (uid[i] < 0x10) {
      nuid += "0";
    }
    nuid += String(uid[i], HEX);
    if (i < uidSize - 1) {
      nuid += ":";
    }
  }
  return nuid;
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
  Serial.println(getNUID(_rfid.uid.uidByte, _rfid.uid.size));
  lcd.write_TOP_LCD("UID tag: ");
  lcd.write_BOT_LCD(getNUID(_rfid.uid.uidByte, _rfid.uid.size));

  if (!tagAllowed(_rfid.uid.uidByte, _rfid.uid.size)) {
    Serial.println("Access denied. Tag not allowed.");
    lcd.writeLCD("Access denied.", "Tag not allowed.");
    _rfid.PICC_HaltA();
    _rfid.PCD_StopCrypto1();
    return;
  }

  if (findTag(_rfid.uid.uidByte, _rfid.uid.size)) {
    removeTag(_rfid.uid.uidByte, _rfid.uid.size);
    Serial.println("User left. Tag removed");
    lcd.writeLCD("User left.", "Tag removed");
    _lock.unlock();
    Serial.print("Customers inside: ");
    Serial.println(_custIns);
    lcd.write_TOP_LCD("Customers inside: ");
    lcd.write_BOT_LCD(String(_custIns));

  } else if (_custIns < _lock.getMax()) {
    addTag(_rfid.uid.uidByte, _rfid.uid.size);
    Serial.println("Access granted.");
    lcd.writeLCD("Access granted.", "Welcome!");
    _lock.unlock();
    Serial.print("Customers inside: ");
    Serial.println(_custIns);
    lcd.write_TOP_LCD("Customers inside: ");
    lcd.write_BOT_LCD(String(_custIns));

  } else {
    Serial.println("Access denied. Max reached!");
    Serial.println(_lock.getMax());
    lcd.writeLCD("Access denied.", "Max reached!.");
    lcd.write_TOP_LCD("Maximum number:");
    lcd.write_BOT_LCD(String(_lock.getMax()));

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
  lcd.writeLCD("Place a tag to", "update access.");
  _time = millis();
  while(millis()-_time < 3000) {
    if (_rfid.PICC_IsNewCardPresent() && _rfid.PICC_ReadCardSerial()){
      Serial.print("UID tag: ");
      Serial.println(getNUID(_rfid.uid.uidByte, _rfid.uid.size));
      lcd.write_TOP_LCD("UID tag: ");
      lcd.write_BOT_LCD(getNUID(_rfid.uid.uidByte, _rfid.uid.size));
      if (findTag(_rfid.uid.uidByte, _rfid.uid.size)) {
        Serial.println("Sign out in order to remove access.");
        lcd.writeLCD("Sign out in order", "to remove access.");
        _rfid.PICC_HaltA();
        _rfid.PCD_StopCrypto1();
        return;
      }
      if (tagAllowed(_rfid.uid.uidByte, _rfid.uid.size)) {
        removeAccess(_rfid.uid.uidByte, _rfid.uid.size);
        Serial.println("Key card removed.");
        lcd.writeLCD("Key card removed.", "");
      } else {
        grantAccess(_rfid.uid.uidByte, _rfid.uid.size);
        Serial.println("Key card added.");
        lcd.writeLCD("Key card added.", "");
      }
      _rfid.PICC_HaltA();
      _rfid.PCD_StopCrypto1();
      return;
    }
    
  }
  Serial.print("Time limit exceeded, press button again to add/remove access.");
  lcd.writeLCD("Time limit exceeded,", "press button again to add/remove access.");
}

void RFID::halt() {
  _rfid.PICC_HaltA();
  _rfid.PCD_StopCrypto1();
}

String RFID::allowedTagsJSON() {
  String json = "[";
  for (int i = 0; i < MAXTAGS; i++) {
    String tag = getNUID(_allowedTags[i], NUMBYTES);
    json += "\"" + tag + "\"";
    if (i < MAXTAGS - 1) {
      json += ",";
    }
  }
  json += "]";
  return json;
}

String RFID::existingTagsJSON() {
  String json = "[";
  for (int i = 0; i < MAXTAGS; i++) {
    String tag = getNUID(_existingTags[i], NUMBYTES);
    json += "\"" + tag + "\"";
    if (i < MAXTAGS - 1) {
      json += ",";
    }
  }
  json += "]";
  return json;
}

int RFID::getCount() {
  return _custIns;
}