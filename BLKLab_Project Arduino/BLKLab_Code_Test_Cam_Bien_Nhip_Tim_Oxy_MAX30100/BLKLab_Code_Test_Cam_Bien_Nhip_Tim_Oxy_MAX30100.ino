#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal_I2C.h>  // Thư viện LCD I2C

PulseOximeter pox;                          // Khai báo đối tượng PulseOximeter
uint32_t tsLastReport = 0;                  // Thời gian để đọc dữ liệu

// Khai báo màn hình LCD (Địa chỉ I2C, chiều rộng và chiều cao của màn hình)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Địa chỉ 0x27 cho LCD 1602

void onBeatDetected() {                     // Hàm callback khi phát hiện nhịp tim
  Serial.println("Nhịp tim phát hiện!");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Khởi động MAX30100...");
  
  if (!pox.begin()) {                       // Khởi tạo module MAX30100
    Serial.println("Không thể khởi tạo MAX30100. Kiểm tra kết nối!");
    while (1);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA); // Cấu hình chế độ và callback
  pox.setOnBeatDetectedCallback(onBeatDetected);

  // Khởi tạo màn hình LCD
  lcd.begin(16, 2);                        // Cài đặt độ rộng và chiều cao của màn hình LCD
  lcd.backlight();                         // Bật đèn nền
  lcd.clear();                             // Xóa màn hình LCD
  lcd.setCursor(0, 0);                     // Đặt con trỏ tại dòng 0, cột 0
  lcd.print("Khởi động...");               // In dòng thông báo trên màn hình LCD
  delay(2000);                             // Đợi 2 giây
}

void loop() {
  pox.update();                             // Cập nhật dữ liệu từ MAX30100
  if (millis() - tsLastReport > 1000) {     // Hiển thị dữ liệu sau mỗi 1 giây
    tsLastReport = millis();
    
    // Lấy nhịp tim và nồng độ SpO2 từ MAX30100
    float heartRate = pox.getHeartRate();
    float spO2 = pox.getSpO2();
    
    // Hiển thị dữ liệu lên Serial Monitor
    Serial.print("Nhịp tim: ");
    Serial.print(heartRate);
    Serial.print(" bpm / Nồng độ SpO2: ");
    Serial.print(spO2);
    Serial.println(" %");

    // Hiển thị dữ liệu lên LCD
    lcd.clear();                         // Xóa màn hình LCD
    lcd.setCursor(0, 0);                 // Đặt con trỏ tại dòng 0, cột 0
    lcd.print("HR: ");
    lcd.print(heartRate, 0);             // In nhịp tim (làm tròn tới số nguyên)
    lcd.print(" bpm");

    lcd.setCursor(0, 1);                 // Đặt con trỏ tại dòng 1, cột 0
    lcd.print("SpO2: ");
    lcd.print(spO2, 1);                  // In nồng độ SpO2 (1 chữ số thập phân)
    lcd.print(" %");
  }
}
