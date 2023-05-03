#include "RFID.h"
#include <SPI.h>
#include <MFRC522.h>
#include "Lock.h"


RFID::RFID(Lock& lock, int SS_PIN, int RST_PIN) : _rfid(SS_PIN, RST_PIN), _lock(lock) {
    _custIns = 0;
    SPI.begin();
    _rfid.PCD_Init();

    EEPROM.begin(EEPROM_SIZE);

    int addr = EEPROM_ADDR;
    for (int i = 0; i < MAXTAGS; i++) {
        for (int j = 0; j < NUMBYTES; j++) {
            _allowedTags[i][j] = EEPROM.readByte(addr++);
        }
    }
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
  Serial.print(getNUID(_rfid.uid.uidByte, _rfid.uid.size));
  lcd.write_TOP_LCD("UID tag: ");
  lcd.write_BOT_LCD(getNUID(_rfid.uid.uidByte, _rfid.uid.size));
  delay(2000);

  if (!tagAllowed(_rfid.uid.uidByte, _rfid.uid.size)) {
    Serial.println("Access denied. Tag not allowed.");
    lcd.writeLCD("Access denied.", "Tag not allowed.");
    _rfid.PICC_HaltA();
    _rfid.PCD_StopCrypto1();
    delay(2000);
    return;
  }

  if (findTag(_rfid.uid.uidByte, _rfid.uid.size)) {
    removeTag(_rfid.uid.uidByte, _rfid.uid.size);
    Serial.println("User left. Tag removed from list.");
    lcd.writeLCD("User left.", "Tag removed from list.");
    _lock.unlock();
    delay(1000);
    Serial.print("Customers inside: ");
    Serial.println(_custIns);
    lcd.write_TOP_LCD("Customers inside: ");
    lcd.write_BOT_LCD(String(_custIns));
    delay(2000);

  } else if (_custIns < _lock.getMax()) {
    addTag(_rfid.uid.uidByte, _rfid.uid.size);
    Serial.println("Access granted.");
    lcd.writeLCD("Access granted.", "Welcome!");
    _lock.unlock();
    delay(1000);
    Serial.print("Customers inside: ");
    Serial.println(_custIns);
    lcd.write_TOP_LCD("Customers inside: ");
    lcd.write_BOT_LCD(String(_custIns));
    delay(2000);

  } else {
    Serial.println("Access denied. Maximum number of customers reached.");
    Serial.println(_lock.getMax());
    lcd.writeLCD("Access denied.", "Maximum number of customers reached.");
    delay(2000);
    lcd.write_TOP_LCD("Maximum number:");
    lcd.write_BOT_LCD(String(_lock.getMax()));
    delay(2000);

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

  // Write _allowedTags array to EEPROM
  int addr = EEPROM_ADDR;
  for (int i = 0; i < MAXTAGS; i++) {
      for (int j = 0; j < NUMBYTES; j++) {
          EEPROM.writeByte(addr++, _allowedTags[i][j]);
      }
  }
  EEPROM.commit();  // Save changes to EEPROM
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
  // Write _allowedTags array to EEPROM
  int addr = EEPROM_ADDR;
  for (int i = 0; i < MAXTAGS; i++) {
      for (int j = 0; j < NUMBYTES; j++) {
          EEPROM.writeByte(addr++, _allowedTags[i][j]);
      }
  }
  EEPROM.commit();  // Save changes to EEPROM
}

void RFID::updateAccess() {
  Serial.println("Place a tag to update access.");
  lcd.writeLCD("Place a tag to", "update access.");
  _time = millis();
  while(millis()-_time < 5000) {
    if (_rfid.PICC_IsNewCardPresent() && _rfid.PICC_ReadCardSerial()){
      Serial.print("UID tag: ");
      Serial.print(getNUID(_rfid.uid.uidByte, _rfid.uid.size));
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
