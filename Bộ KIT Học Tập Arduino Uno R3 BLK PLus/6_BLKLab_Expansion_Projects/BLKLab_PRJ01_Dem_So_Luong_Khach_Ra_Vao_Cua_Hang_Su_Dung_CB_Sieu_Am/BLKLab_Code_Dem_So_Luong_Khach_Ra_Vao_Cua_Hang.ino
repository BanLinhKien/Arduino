/*
 * =========================================================================================================================*
 *                                                     BANLINHKIEN.COM
 * =========================================================================================================================*                           
 *                              Đếm số lượng khách ra, vào cửa hàng sử dụng cảm biến siêu âm
 * Đấu nối: 
 + Cảm biến siêu âm (SRF05):
  -D2: nối với chân echo của CB SRF05 1   
  -D3: nối với chân trig của CB SRF05 1
  
  -D4: nối với chân echo của CB SRF05 2    
  -D5: nối với chân trig của CB SRF05 1

 + Loa:
  -D7: nối với chân Anot của loa   

 + Đèn led:
  -D6: nối với chân Anot của đèn led  

 + Màn hình LCD I2C 1602:
  -SDA: nối với chân SDA của LCD I2C 1602
  -SCL: nối với chân SCL của LCD I2C 1602
*/

#include <Wire.h>                                                   // Thư viện Wire sử dụng để giao tiếp I2C.
#include <LiquidCrystal_I2C.h>                                      // Thư viện LiquidCrystal_I2C điều khiển màn hình LCD qua giao tiếp I2C.

LiquidCrystal_I2C lcd(0x27, 16, 2);                                 // Khởi tạo đối tượng LiquidCrystal_I2C với địa chỉ I2C 0x27 và kích thước màn hình 16x2.

#define trigPinIn 3                                                 // Định nghĩa chân trig cho CB bên trong cửa hàng.
#define echoPinIn 2                                                 // Định nghĩa chân echo cho CB bên trong cửa hàng.
#define trigPinOut 5                                                // Định nghĩa chân trig cho CB bên ngoài cửa hàng.
#define echoPinOut 4                                                // Định nghĩa chân echo cho CB bên ngoài cửa hàng.
#define ledPin 6                                                    // Định nghĩa chân cho đèn LED.
#define buzzerPin 7                                                 // Định nghĩa chân cho còi buzzer.

int count = 0;                                                      // Biến lưu trữ số lượng người trong cửa hàng.
bool isInside = false;                                              // Biến kiểm tra nếu có người vào trong cửa hàng.
bool isOutside = false;                                             // Biến kiểm tra nếu có người ra khỏi cửa hàng.
bool updateLCD = true;                                              // Biến kiểm tra cập nhật màn hình LCD.

void setup() {
  lcd.init();                                                       // Khởi tạo màn hình LCD.
  lcd.backlight();                                                  // Bật đèn nền cho màn hình.
  Serial.begin(9600);                                               // Khởi tạo giao tiếp serial với tốc độ 9600 bps.
  
  pinMode(trigPinIn, OUTPUT);                                       // Chân trig bên trong là đầu ra.
  pinMode(echoPinIn, INPUT);                                        // Chân echo bên trong là đầu vào.
  pinMode(trigPinOut, OUTPUT);                                      // Chân trig bên ngoài là đầu ra.
  pinMode(echoPinOut, INPUT);                                       // Chân echo bên ngoài là đầu vào.
  pinMode(ledPin, OUTPUT);                                          // Chân cho đèn LED là đầu ra.
  pinMode(buzzerPin, OUTPUT);                                       // Chân cho loa còi là đầu ra.
  
  lcd.clear();                                                      // Xóa màn hình LCD.
  lcd.setCursor(1, 0);                                              
  lcd.print("DEM SL NGUOI:");  
  lcd.setCursor(0, 1);  
  lcd.print("TRONG CUA HANG");  
  delay(2000); 
}

void beep(int duration) {
  digitalWrite(buzzerPin, HIGH);                                    // Bật loa còi.
  delay(duration);                                                  // Delay theo thời gian được truyền vào.
  digitalWrite(buzzerPin, LOW);                                     // Tắt loa còi.
}

void loop() {
  long durationIn, distanceIn, durationOut, distanceOut;
  
  digitalWrite(trigPinIn, LOW);                                     // Đặt chân trig bên trong xuống mức thấp.
  delayMicroseconds(2);                                             // Delay 2 microseconds.
  digitalWrite(trigPinIn, HIGH);                                    // Đặt chân trig bên trong lên mức cao.
  delayMicroseconds(10);                                            // Delay 10 microseconds.
  digitalWrite(trigPinIn, LOW);                                     // Đặt chân trig bên trong xuống mức thấp.
  durationIn = pulseIn(echoPinIn, HIGH);                            // Đo thời gian phản hồi từ cảm biến echo bên trong.
  distanceIn = (durationIn / 2) / 29.1;                             // Tính khoảng cách từ thời gian phản hồi.
  
  digitalWrite(trigPinOut, LOW);                                    // Đặt chân trig bên ngoài xuống mức thấp.
  delayMicroseconds(2);                                             // Delay 2 microseconds.
  digitalWrite(trigPinOut, HIGH);                                   // Đặt chân trig bên ngoài lên mức cao.
  delayMicroseconds(10);                                            // Delay 10 microseconds.
  digitalWrite(trigPinOut, LOW);                                    // Đặt chân trig bên ngoài xuống mức thấp.
  durationOut = pulseIn(echoPinOut, HIGH);                          // Đo thời gian phản hồi từ cảm biến echo bên ngoài.
  distanceOut = (durationOut / 2) / 29.1;                           // Tính khoảng cách từ thời gian phản hồi.
  
  Serial.print("KC Vao: ");                                         // In ra Serial thông tin về khoảng cách bên trong.
  Serial.print(distanceIn);
  Serial.print(" cm, KC Ra: ");                                     // In ra Serial thông tin về khoảng cách bên ngoài.
  Serial.print(distanceOut);
  Serial.println(" cm");
  
  if (distanceIn < 20) {
    if (!isInside) {
      isInside = true;
      count++;
      updateLCD = true;                                             // Đặt biến updateLCD để cập nhật màn hình LCD.
      beep(500);                                                    // Kích hoạt âm thanh còi trong 500ms.
      delay(500);                                                   // Delay 500ms.
    }
  } else {
    isInside = false;
  }

  if (distanceOut < 20) {
    if (!isOutside) {
      isOutside = true;
      count--;
      if (count < 0) {
        count = 0;
      }
      updateLCD = true;                                               // Đặt biến updateLCD để cập nhật màn hình LCD.
      beep(500);                                                      // Kích hoạt âm thanh còi trong 500ms.
      delay(500);                                                     // Delay 500ms.
    }
  } else {
    isOutside = false;
  }
  
  Serial.print("SL Nguoi Trong Cua Hang: ");                          // In ra Serial số lượng người trong cửa hàng.
  Serial.println(count);
  
  if (count <= 0) {
    digitalWrite(ledPin, LOW);                                        // Tắt đèn LED.
    if (updateLCD) { 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("KHONG CO NGUOI:");                                   // Hiển thị thông báo không có người trên màn hình LCD.
      lcd.setCursor(4, 1);
      lcd.print("DEN TAT");                                           // Hiển thị thông báo tắt đèn trên màn hình LCD.
      updateLCD = false;                                              // Đặt biến updateLCD để ngăn cập nhật màn hình LCD liên tục.
    }
    delay(200);                                                       // Delay 200ms.
  } else {
    digitalWrite(ledPin, HIGH);                                       // Bật đèn LED.
    if (updateLCD) { 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SO NGUOI:");                                         // Hiển thị số lượng người trên màn hình LCD.
      lcd.setCursor(10, 0);
      lcd.print(count);
      lcd.setCursor(0, 1);
      lcd.print("XINCHAOQUYKHACH");                                   // Hiển thị thông báo chào khách hàng trên màn hình LCD.
      updateLCD = false;                                              // Đặt biến updateLCD để ngăn cập nhật màn hình LCD liên tục.
    }
    delay(200);                                                       // Delay 200ms.
  }
}
