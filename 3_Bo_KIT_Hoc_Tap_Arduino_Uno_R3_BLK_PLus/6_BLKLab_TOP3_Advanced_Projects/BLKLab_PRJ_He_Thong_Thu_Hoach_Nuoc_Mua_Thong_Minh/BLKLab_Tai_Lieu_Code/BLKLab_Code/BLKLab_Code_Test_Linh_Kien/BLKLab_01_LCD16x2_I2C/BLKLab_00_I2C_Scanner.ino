#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Doi 0x27 thanh 0x3F neu I2C Scanner tim thay dia chi 0x3F.
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("HE THONG NUOC"));
  lcd.setCursor(0, 1);
  lcd.print(F("LCD HOAT DONG"));
}

void loop() {
  static unsigned long soGiay = 0;
  lcd.setCursor(0, 1);
  lcd.print(F("GIAY: "));
  lcd.print(soGiay++);
  lcd.print(F("      "));
  delay(1000);
}

