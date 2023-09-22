/*
 * ===========================================================================================================================*
 *                                                        BANLINHKIEN.COM
 * ===========================================================================================================================*                           
 *                                                  Đo tần số NE555 bằng Arduino
 * Đấu nối: 
 + Modul NE555:
  -D2: nối với chân OUT của NE555 
  -GND: nối với chân GND của NE555 
  -VCC: nối với chân VCC của NE555
 + LCD I2C 1602:
  -GND: nối với chân GND của LCD I2C 1602 
  -VCC: nối với chân VCC của LCD I2C 1602 
  -SDA: nối với chân SDA của LCD I2C 1602 
  -SCL: nối với chân SCL của LCD I2C 1602 
 */

#include <Wire.h>                                                           // Import thư viện Wire cho giao tiếp I2C
#include <LiquidCrystal_I2C.h>                                              // Import thư viện LiquidCrystal_I2C cho LCD

LiquidCrystal_I2C lcd(0x27, 16, 2);                                         // Khai báo một đối tượng LiquidCrystal_I2C với địa chỉ I2C 0x27, màn hình 16x2

const int inputPin = 2;                                                     // Khai báo biến inputPin để đọc tín hiệu đầu vào từ chân số 2
unsigned long previousMillis = 0;                                           // Biến lưu trữ thời gian của lần lặp trước đó
unsigned long interval = 1000;                                              // Thời gian cách nhau giữa các lần đo tần số (1 giây)
int frequency = 0;                                                          // Biến lưu trữ tần số đo được

void setup() {
  lcd.init();                                                               // Khởi tạo màn hình LCD
  lcd.backlight();                                                          // Bật đèn nền của LCD
  Serial.begin(9600);                                                       // Khởi tạo giao tiếp Serial với tốc độ baud 9600
  pinMode(inputPin, INPUT);                                                 // Cài đặt chân inputPin là chế độ đầu vào

  lcd.setCursor(0, 0);                                                      // Đặt con trỏ ở hàng 1, cột 1 trên màn hình LCD
  lcd.print("Banlinhkien.com");                                             // In chuỗi "Banlinhkien.com" lên màn hình LCD
  lcd.setCursor(0, 1);                                                      // Đặt con trỏ ở hàng 2, cột 1 trên màn hình LCD
  lcd.print("Do Tan So NE555");                                             // In chuỗi "Do Tan So NE555" lên màn hình LCD
  delay(2000);
}

void loop() {
  unsigned long currentMillis = millis();                                 // Lấy thời gian hiện tại tính từ khi Arduino khởi động

  if (currentMillis - previousMillis >= interval) {                       // Kiểm tra nếu đã đủ thời gian giữa các lần đo
    previousMillis = currentMillis;                                       // Lưu thời điểm hiện tại là thời điểm trước đó

    int pulseCount = 0;  // Biến đếm số xung
    while (currentMillis - previousMillis < interval) {                   // Trong khoảng thời gian interval
      if (digitalRead(inputPin) == HIGH) {                                // Kiểm tra nếu chân inputPin đang ở mức cao
        pulseCount++;                                                     // Tăng biến đếm số xung lên 1
        while (digitalRead(inputPin) == HIGH) {                           // Đợi đến khi chân inputPin thay đổi về mức thấp
        }
      }
      currentMillis = millis();                                           // Cập nhật thời gian hiện tại
    }

    frequency = pulseCount / (interval / 1000);                           // Tính toán tần số dựa trên số xung và thời gian

    lcd.clear();                                                          // Xóa màn hình LCD
    lcd.setCursor(0, 0);                                                  // Đặt con trỏ ở hàng 1, cột 1 trên màn hình LCD
    lcd.print("Frequency: ");                                             // In chuỗi "Frequency: " lên màn hình LCD
    lcd.setCursor(0, 1);                                                  // Đặt con trỏ ở hàng 2, cột 1 trên màn hình LCD
    lcd.print(frequency);                                                 // In giá trị tần số lên màn hình LCD
    lcd.print(" Hz");                                                     // In đơn vị Hz lên màn hình LCD

    Serial.print("Frequency: ");                                          // Gửi chuỗi "Frequency: " qua giao tiếp Serial
    Serial.print(frequency);                                              // Gửi giá trị tần số qua giao tiếp Serial
    Serial.println(" Hz");                                                // Gửi chuỗi " Hz" qua giao tiếp Serial
  }
}

/*
    Chương trình này sử dụng một màn hình LCD I2C để hiển thị tần số (frequency) của xung đầu vào từ chân số 2 (inputPin) trong một khoảng thời gian cố định (interval). 
    Tần số được tính bằng cách đếm số lượng xung trong interval và chia cho interval (đơn vị là mili giây) và sau đó nhân với 1000 để chuyển đổi thành Hz. 
    Kết quả cả tần số và dữ liệu được hiển thị trên màn hình LCD và gửi qua giao tiếp Serial.

    Các dải tần số lựa chọn:	+ 1Hz-50Hz
                              + 50Hz-1Khz
                              + 1Khz-10Khz
                              + 10Khz-200Khz
 */

