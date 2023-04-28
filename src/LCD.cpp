 #include "LCD.h"

LCD::LCD(int address, int columns, int rows) : lcd(address, columns, rows)
{
  
}

//===========================================================
void LCD::setup()
{

  lcd.init();         // Initiating LCD with I2C
  lcd.backlight();    // Initiating LCDs backlight with I2C
  lcd.clear();        // clear display

}
//===========================================================

void LCD::write(String text1, String text2)
{
  lcd.clear();
  lcd.setCursor(0,0);    lcd.print(text1); 
  lcd.setCursor(0,1);    lcd.print(text2);
  
}
//===========================================================

void LCD::write_TOP(String text1)
{
  lcd.clear();
  lcd.setCursor(0,0);    lcd.print(text1);
}
void LCD::write_BOT(String text2)
{
  lcd.setCursor(0,1);    lcd.print(text2);
}
//===========================================================