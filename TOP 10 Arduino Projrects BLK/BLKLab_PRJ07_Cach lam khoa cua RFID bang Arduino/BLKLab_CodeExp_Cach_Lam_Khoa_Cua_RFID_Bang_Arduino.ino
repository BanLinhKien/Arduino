/*
 * ===================================================================================================================*
 *                                                  BANLINHKIEN.COM
 * ===================================================================================================================*                           
 *                                           Khóa cửa RFID sử dụng Arduino
 * Đấu nối: 
 + LCD 1602 I2C:
  -SDA: nối với chân SDA của LCD  
  -SCL: nối với chân SCL của LCD  
 + Động cơ Servo:
  -D8: nối với dây cam của động cơ servo    
 + Còi buzzer:
  -D5: nối với chân dương của còi buzzer
 + Led:
  -D6: nối với chân dương của led red
  -D7: nối với chân dương của led green
 + Keypad 4x4:
  -A0: nối với chân hàng 1 của keypad 4x4
  -A1: nối với chân hàng 2 của keypad 4x4
  -A2: nối với chân hàng 3 của keypad 4x4
  -A3: nối với chân hàng 4 của keypad 4x4
  -D2: nối với chân cột 1 của keypad 4x4
  -D1: nối với chân cột 2 của keypad 4x4  
  -D0: nối với chân cột 2 của keypad 4x4    
 */

#include <MFRC522.h>                                                                        // Sử dụng thư viện MFRC522 để đọc thẻ RFID
#include <LiquidCrystal_I2C.h>                                                              // Sử dụng thư viện LiquidCrystal_I2C để điều khiển màn hình LCD qua giao tiếp I2C
#include <Keypad.h>                                                                         // Sử dụng thư viện Keypad để đọc dữ liệu từ bàn phím số
#include <Servo.h>                                                                          // Sử dụng thư viện Servo để điều khiển Servo motor
#include <SPI.h>                                                                            // Sử dụng thư viện SPI cho việc giao tiếp với MFRC522

LiquidCrystal_I2C lcd(0x27, 16, 2);                                                         // Khởi tạo đối tượng LCD với địa chỉ I2C 0x27 và kích thước 16x2
MFRC522 mfrc522(10, 9);                                                                     // Khởi tạo đối tượng MFRC522 với chân chốt kết nối 10 và 9
Servo sg90;                                                                                 // Khởi tạo đối tượng Servo để điều khiển Servo motor
constexpr uint8_t greenLed = 7;                                                             // Chân kết nối LED màu xanh
constexpr uint8_t redLed = 6;                                                               // Chân kết nối LED màu đỏ
constexpr uint8_t servoPin = 8;                                                             // Chân kết nối Servo motor
constexpr uint8_t buzzerPin = 5;                                                            // Chân kết nối buzzer

char initial_password[4] = {'1', '2', '3', '4'};                                            // Mật khẩu khởi tạo
String tagUID = "70 08 F7 55";                                                              // UID của thẻ RFID được chấp nhận
char password[4];                                                                           // Mật khẩu nhập từ bàn phím
boolean RFIDMode = true;                                                                    // Chế độ đọc thẻ RFID ban đầu
char key_pressed = 0;                                                                       // Biến lưu giữ phím được nhấn trên bàn phím
uint8_t i = 0;                                                                              // Biến đếm số ký tự trong mật khẩu nhập vào

const byte rows = 4;                                                                        // Số hàng trên bàn phím
const byte columns = 4;                                                                     // Số cột trên bàn phím

char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};                                                                                          // Mảng chứa các ký tự tương ứng trên bàn phím

byte row_pins[rows] = {A0, A1, A2, A3};                                                     // Chân kết nối hàng trên bàn phím
byte column_pins[columns] = {2, 1, 0};                                                      // Chân kết nối cột trên bàn phím

Keypad keypad_key = Keypad(makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);     // Khởi tạo đối tượng Keypad

void setup() {
  pinMode(buzzerPin, OUTPUT);                                                               // Đặt chân buzzer là chế độ OUTPUT
  pinMode(redLed, OUTPUT);                                                                  // Đặt chân LED đỏ là chế độ OUTPUT
  pinMode(greenLed, OUTPUT);                                                                // Đặt chân LED xanh là chế độ OUTPUT
  sg90.attach(servoPin);                                                                    // Kết nối Servo motor với chân điều khiển
  sg90.write(0);                                                                            // Đưa Servo motor về góc quay 0 độ ở vị trí ban đầu
  lcd.init();                                                                               // Khởi tạo màn hình LCD
  lcd.backlight();                                                                          // Bật đèn nền của màn hình LCD
  SPI.begin();                                                                              // Khởi tạo giao tiếp SPI
  mfrc522.PCD_Init();                                                                       // Khởi tạo MFRC522
  lcd.clear();                                                                              // Xóa nội dung trên màn hình LCD
}

void beep() {
  digitalWrite(buzzerPin, HIGH);                                                            // Kích hoạt buzzer
  delay(100);                                                                               // Delay 100ms
  digitalWrite(buzzerPin, LOW);                                                             // Tắt buzzer
}

void loop() {
  if (RFIDMode == true) {
    lcd.setCursor(1, 0);
    lcd.print("Cua Dang Khoa");                                                             // Hiển thị thông báo trên màn hình LCD
    lcd.setCursor(1, 1);
    lcd.print("Quet The RFID");
    if (!mfrc522.PICC_IsNewCardPresent()) {
      return;                                                                               // Nếu không có thẻ mới được đặt gần, thoát khỏi hàm loop()
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;                                                                               // Nếu không đọc được thông tin từ thẻ, thoát khỏi hàm loop()
    }
    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++) {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
    }
    tag.toUpperCase();
    if (tag.substring(1) == tagUID) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("The Da Khop");
      digitalWrite(greenLed, HIGH);
      beep();                                                                               // Kêu tít tít khi thẻ RFID đúng
      delay(500);
      digitalWrite(greenLed, LOW);
      lcd.clear();
      lcd.print(" Nhap Mat Khau:");
      lcd.setCursor(0, 1);
      RFIDMode = false;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" The Khong Dung");
      lcd.setCursor(1, 1);
      lcd.print(" Quet Lai The");
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(redLed, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(redLed, LOW);
      lcd.clear();
    }
  }
  if (RFIDMode == false) {
    key_pressed = keypad_key.getKey();
    if (key_pressed) {
      password[i++] = key_pressed;
      lcd.print("*");                                                                       // Hiển thị dấu '*' trên màn hình LCD khi nhập mật khẩu
    }
    if (i == 4) {
      delay(200);
      if (!(strncmp(password, initial_password, 4))) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Mat Khau Dung");
        lcd.setCursor(4, 1);
        lcd.print("Mo Cua");
        sg90.write(180);                                                                      // Mở khóa bằng cách đưa Servo motor về góc quay 180 độ
        delay(1000);
        digitalWrite(greenLed, HIGH);
        beep();  
        delay(500);
        digitalWrite(greenLed, LOW);
        sg90.write(0);                                                                        // Đóng khóa bằng cách đưa Servo motor về vị trí ban đầu
        lcd.clear();
        i = 0;
        RFIDMode = true;
      } else {
        lcd.clear();
        lcd.print("  Sai Mat Khau");
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(redLed, HIGH);
        delay(3000);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(redLed, LOW);
        lcd.clear();
        i = 0;
        RFIDMode = true;
      }
    }
  }
}
