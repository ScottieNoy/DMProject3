#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class LCD {
public:
  LCD(int address, int columns, int rows);
  void setupLCD();
  void writeLCD(String text1, String text2);
  void write_TOP_LCD(String text1);
  void write_BOT_LCD(String text2);
private:
  LiquidCrystal_I2C lcd;
};
#endif //LCD_H