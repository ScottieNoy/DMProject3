#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class LCD {
public:
  LCD(int address, int columns, int rows);
  void setup();
  void write(String text1, String text2);
  void write_TOP(String text1);
  void write_BOT(String text2);
private:
  LiquidCrystal_I2C lcd;
};
#endif //LCD_H