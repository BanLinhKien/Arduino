/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 *              
 *              Hiển Thị Sóng Âm Thanh Lên LCD1602 với I2C
 */
#include <Wire.h>                     // Thư viện I2C
#include <LiquidCrystal_I2C.h>        // Thư viện cho LCD I2C

LiquidCrystal_I2C lcd(0x27, 16, 2);   // Địa chỉ I2C, 16 cột, 2 hàng

// Khởi tạo các biến
int i = 0;
int j = 0;
int randomnum;
int k = 0;

// Khởi tạo các độ cao trong một ô 5x8
byte wave1[] = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};  // Tất cả các hàng sáng
byte wave2[] = {0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};  // 2 hàng trên tắt
byte wave3[] = {0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F};  // 4 hàng trên tắt
byte wave4[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F};  // 6 hàng tắt
byte wave5[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // 8 hàng tắt

void setup() {
  Serial.begin(9600);           // Tạo serial với tốc độ 9600 baud
  lcd.init();                   // Khởi tạo LCD I2C
  lcd.backlight();              // Bật đèn nền (bỏ dòng này nếu không cần đèn nền)
  lcd.createChar(4, wave1);     // Hiển thị wave1 ở vị trí thứ 4
  lcd.createChar(3, wave2);     // Hiển thị wave2 ở vị trí thứ 3
  lcd.createChar(2, wave3);     // Hiển thị wave3 ở vị trí thứ 2
  lcd.createChar(1, wave4);     // Hiển thị wave4 ở vị trí thứ 1
  lcd.createChar(0, wave5);     // Hiển thị wave5 ở vị trí thứ 0
  lcd.clear();                  // Xóa màn hình
  lcd.setCursor(1, 0);          // Đưa con trỏ về vị trí 1,0
  lcd.print("Banlinhkien.com"); // Hiển thị dòng chữ
  delay(2000);                  // Hiển thị trong 2s
  lcd.clear();                  // Xóa màn hình
}

void loop() {
  randomnum = random(0, 8);     // Gán biến randomnum bằng số ngẫu nhiên từ 0-7
  Serial.print("Giá trị randomnum= ");
  Serial.println(randomnum);

  for (i = randomnum; i > 0; i--) {
    int z = i;                  // Lưu giá trị của i
    Serial.print("Giá trị i= ");
    Serial.println(i);

    // Hiển thị ở cột thứ 7
    test();
    lcd.setCursor(7, 0);
    lcd.write(j);
    lcd.setCursor(7, 1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 6 và 8
    test();
    lcd.setCursor(8, 0);
    lcd.write(j);
    lcd.setCursor(8, 1);
    lcd.write(k);
    lcd.setCursor(6, 0);
    lcd.write(j);
    lcd.setCursor(6, 1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 5 và 9
    test();
    lcd.setCursor(9, 0);
    lcd.write(j);
    lcd.setCursor(9, 1);
    lcd.write(k);
    lcd.setCursor(5, 0);
    lcd.write(j);
    lcd.setCursor(5, 1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 4 và 10
    test();
    lcd.setCursor(10, 0);
    lcd.write(j);
    lcd.setCursor(10, 1);
    lcd.write(k);
    lcd.setCursor(4, 0);
    lcd.write(j);
    lcd.setCursor(4, 1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 3 và 11
    test();
    lcd.setCursor(11, 0);
    lcd.write(j);
    lcd.setCursor(11, 1);
    lcd.write(k);
    lcd.setCursor(3, 0);
    lcd.write(j);
    lcd.setCursor(3, 1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 2 và 12
    test();
    lcd.setCursor(12, 0);
    lcd.write(j);
    lcd.setCursor(12, 1);
    lcd.write(k);
    lcd.setCursor(2, 0);
    lcd.write(j);
    lcd.setCursor(2, 1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 1 và 13
    test();
    lcd.setCursor(13, 0);
    lcd.write(j);
    lcd.setCursor(13, 1);
    lcd.write(k);
    lcd.setCursor(1, 0);
    lcd.write(j);
    lcd.setCursor(1, 1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 0 và 14
    test();
    lcd.setCursor(14, 0);
    lcd.write(j);
    lcd.setCursor(14, 1);
    lcd.write(k);
    lcd.setCursor(0, 0);
    lcd.write(j);
    lcd.setCursor(0, 1);
    lcd.write(k);

    delay(100); // Hiển thị 0.1s
    lcd.clear(); // Xóa màn hình
    i = z; // Đặt lại i=z
  }
}

void test() {
  if (i == 8) {
    j = 4; k = 4;
  } else if (i == 7) {
    j = 3; k = 4;
  } else if (i == 6) {
    j = 2; k = 4;
  } else if (i == 5) {
    j = 1; k = 4;
  } else if (i == 4) {
    j = 0; k = 4;
  } else if (i == 3) {
    j = 0; k = 3;
  } else if (i == 2) {
    j = 0; k = 2;
  } else if (i == 1) {
    j = 0; k = 1;
  } else if (i == 0) {
    j = 0; k = 0;
  }
}
