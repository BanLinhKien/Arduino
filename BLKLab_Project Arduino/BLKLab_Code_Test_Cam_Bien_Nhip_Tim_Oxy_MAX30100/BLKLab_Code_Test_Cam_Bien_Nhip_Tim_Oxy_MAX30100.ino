#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);         // Khởi tạo đối tượng LCD với địa chỉ I2C
PulseOximeter pox;                          // Khai báo đối tượng PulseOximeter
uint32_t tsLastReport = 0;                  // Thời gian để đọc dữ liệu

void onBeatDetected() {                     // Hàm callback khi phát hiện nhịp tim
  Serial.println("Nhịp tim phát hiện!");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Khởi động MAX30100...");
  lcd.begin(16, 2);                         // Khởi động LCD
  lcd.backlight();                          // Bật đèn nền LCD
  if (!pox.begin()) {                       // Khởi tạo module MAX30100
    Serial.println("Không thể khởi tạo MAX30100. Kiểm tra kết nối!");
    while (1);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);// Cấu hình chế độ và callback
  pox.setOnBeatDetectedCallback(onBeatDetected);

  Serial.println("Bắt đầu đo!");
}

void loop() {
  pox.update();                             // Cập nhật dữ liệu từ MAX30100
  if (millis() - tsLastReport > 1000) {     // Hiển thị dữ liệu sau mỗi 1 giây
    tsLastReport = millis();
    Serial.print("Nhịp tim: ");
    Serial.print(pox.getHeartRate());
    Serial.print(" bpm / Nồng độ SpO2: ");
    Serial.print(pox.getSpO2());
    Serial.println(" %");
    
    lcd.setCursor(0, 0);
    lcd.print("HR: ");
    lcd.print(pox.getHeartRate());
    lcd.setCursor(9, 0);
    lcd.print(" bpm");

    lcd.setCursor(0, 1);
    lcd.print("SpO2: ");
    lcd.print(pox.getSpO2());
    lcd.setCursor(9, 1);
    lcd.print(" %");

  }
}
