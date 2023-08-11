#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2); 

  byte stepA1[8] = {0x0F,  0x0F,  0x06,  0x0A,  0x12,  0x12,  0x09,  0x00};
  byte stepA2[8] = {0x18,  0x1e,  0x18,  0x10,  0x10,  0x08,  0x04,  0x00};
  byte stepA3[8] = {0x00,  0x09,  0x12,  0x12,  0x0A,  0x07,  0x0F,  0x0F};
  byte stepA4[8] = {0x00,  0x04,  0x08,  0x10,  0x10,  0x18,  0x1e,  0x18};
  byte stepA5[8] = {0x0F,  0x0F,  0x07,  0x0A,  0x12,  0x12,  0x12,  0x00};
  byte stepA6[8] = {0x18,  0x1E,  0x18,  0x10,  0x10,  0x08,  0x08,  0x00};
  byte stepA7[8] = {0x00,  0x12,  0x12,  0x12,  0x0A,  0x07,  0x0F,  0x0F};
  byte stepA8[8] = { 0x00,  0x08,  0x08,  0x10,  0x10,  0x18,  0x1E,  0x18};
  
void setup() {
 lcd.begin();
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Banlinhkien.com");
 lcd.setCursor(0,1);
 lcd.print("Welcome");  
 delay(3000);
 lcd.createChar(1, stepA1);
 lcd.createChar(2, stepA2);
 lcd.createChar(3, stepA3);
 lcd.createChar(4, stepA4);
 lcd.createChar(5, stepA5);
 lcd.createChar(6, stepA6);
 lcd.createChar(7, stepA7);
 lcd.createChar(8, stepA8);
 lcd.clear();
        }
void loop() {
  for (int i =-1; i<16 ; i=i+2)
  {
    int j = i+1;
    int k =j+1;
 
   lcd.setCursor(i,1);
   lcd.write(1);
   lcd.setCursor(j,1);
   lcd.write(2);
   lcd.setCursor(i,0);
   lcd.write(3);
   lcd.setCursor(j,0);
   lcd.write(4);
   delay(500);
 
   lcd.clear ();
   lcd.setCursor(j,1);
   lcd.write(5);
   lcd.setCursor(k,1);
   lcd.write(6);
   lcd.setCursor(j,0);
   lcd.write(7);
   lcd.setCursor(k,0);
   lcd.write(8);
   delay(500);
   lcd.clear ();
  }
}
