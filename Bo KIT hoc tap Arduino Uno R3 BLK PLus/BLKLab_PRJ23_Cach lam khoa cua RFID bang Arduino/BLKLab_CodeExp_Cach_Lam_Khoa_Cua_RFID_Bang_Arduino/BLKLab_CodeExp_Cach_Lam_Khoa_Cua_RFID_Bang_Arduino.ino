/*
 * =================================================================================================================*
 *                                                  BANLINHKIEN.COM
 * =================================================================================================================*                           
 *                                           Khóa cửa RFID sử dụng Arduino
 * Đấu nối: 
 + LCD 1602 I2C:
  -SDA: nối với chân SDA của LCD  
  -SCL: nối với chân SCL của LCD  
 + Động cơ Servo:
  -D8: nối với dây cam của động cơ servo    
 + Keypad 4x4:
  -A0: nối với chân hàng 1 của keypad 4x4
  -A1: nối với chân hàng 2 của keypad 4x4
  -A2: nối với chân hàng 3 của keypad 4x4
  -A3: nối với chân hàng 4 của keypad 4x4
  -D2: nối với chân cột 1 của keypad 4x4
  -D1: nối với chân cột 2 của keypad 4x4  
  -D0: nối với chân cột 2 của keypad 4x4    
 */

#include <MFRC522.h>                                                                          // Thư viện cho RFID
#include <LiquidCrystal_I2C.h>                                                                // Thư viện cho LCD I2C
#include <Keypad.h>                                                                           // Thư viện cho keypad
#include <Servo.h>                                                                            // Thư viện cho Servo motor
#include <SPI.h>                                                                              // Thư viện cho SPI communication

LiquidCrystal_I2C lcd(0x27, 16, 2);                                                           // Khởi tạo đối tượng LCD I2C với địa chỉ 0x27 và kích thước 16x2
MFRC522 mfrc522(10, 9);                                                                       // Khởi tạo đối tượng RFID với chân SDA 10 và RST 9
Servo sg90;                                                                                   // Khởi tạo đối tượng Servo motor
constexpr uint8_t greenLed = 7;                                                               // Chân nối LED xanh
constexpr uint8_t redLed = 6;                                                                 // Chân nối LED đỏ
constexpr uint8_t servoPin = 8;                                                               // Chân nối Servo motor
constexpr uint8_t buzzerPin = 5;                                                              // Chân nối buzzer

char initial_password[4] = {'1', '2', '3', '4'};                                              // Mật khẩu mặc định
String tagUID = "70 08 F7 55";                                                                // UID của thẻ RFID được chấp nhận
char password[4];                                                                             // Mật khẩu nhập từ keypad
boolean RFIDMode = true;                                                                      // Chế độ RFID (true) hoặc chế độ mật khẩu (false)
char key_pressed = 0;                                                                         // Biến lưu giữ nút được nhấn trên keypad
uint8_t i = 0;                                                                                // Biến đếm số ký tự mật khẩu đã nhập

const byte rows = 4;                                                                          // Số hàng trên keypad
const byte columns = 4;                                                                       // Số cột trên keypad

char hexaKeys[rows][columns] = {                                                              // Ma trận ký tự trên keypad
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte row_pins[rows] = {A0, A1, A2, A3};                                                       // Chân kết nối các hàng trên keypad
byte column_pins[columns] = {2, 1, 0};                                                        // Chân kết nối các cột trên keypad

Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);      // Khởi tạo đối tượng keypad

void setup() {
  pinMode(buzzerPin, OUTPUT);                                                                 // Đặt chân buzzer là OUTPUT
  pinMode(redLed, OUTPUT);                                                                    // Đặt chân LED đỏ là OUTPUT
  pinMode(greenLed, OUTPUT);                                                                  // Đặt chân LED xanh là OUTPUT
  sg90.attach(servoPin);                                                                      // Gắn Servo motor vào chân servoPin  
  sg90.write(0);                                                                              // Đưa Servo motor về vị trí ban đầu
  lcd.init();                                                                                 // Khởi tạo LCD
  lcd.backlight();                                                                            // Bật đèn nền LCD
  SPI.begin();                                                                                // Khởi tạo SPI communication
  mfrc522.PCD_Init();                                                                         // Khởi tạo module RFID
  lcd.clear();                                                                                // Xóa màn hình LCD
}

void loop() {
  if (RFIDMode == true) {
    lcd.setCursor(1, 0);
    lcd.print("Cua Dang Khoa");
    lcd.setCursor(1, 1);
    lcd.print("Quet The RFID");
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++)
    {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
    }
    tag.toUpperCase();
    if (tag.substring(1) == tagUID)
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("The Da Khop");
      digitalWrite(greenLed, HIGH);
      delay(3000);
      digitalWrite(greenLed, LOW);
      lcd.clear();
      lcd.print(" Nhap Mat Khau:");
      lcd.setCursor(0, 1);
      RFIDMode = false; 
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" The Khong Dung");
      lcd.setCursor(0, 1);
      lcd.print("Quet Lai The");
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(redLed, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(redLed, LOW);
      lcd.clear();
    }
  }
  if (RFIDMode == false) {
    key_pressed = keypad_key.getKey();                                                            // Đọc giá trị nút được nhấn trên keypad
    if (key_pressed)
    {
      password[i++] = key_pressed;                                                                // Lưu giá trị nút vào mảng password và tăng biến đếm i
      lcd.print("*");                                                                             // Hiển thị dấu * trên LCD
    }
    if (i == 4)                                                                                   // Nếu đã nhập đủ 4 ký tự mật khẩu
    {
      delay(200);
      if (!(strncmp(password, initial_password, 4)))                                              // So sánh mật khẩu với mật khẩu mặc định
      {
        lcd.clear();
        lcd.print(" Mat Khau Dung");
        sg90.write(90);                                                                           // Mở khóa bằng cách xoay Servo motor
        digitalWrite(greenLed, HIGH);
        delay(3000);
        digitalWrite(greenLed, LOW);
        sg90.write(0);                                                                            // Đóng khóa bằng cách đưa Servo motor về vị trí ban đầu
        lcd.clear();
        i = 0;
        RFIDMode = true;                                                                          // Chuyển về chế độ RFID
      }
      else   
      {
        lcd.clear();
        lcd.print("  Sai Mat Khau");
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(redLed, HIGH);
        delay(3000);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(redLed, LOW);
        lcd.clear();
        i = 0;
        RFIDMode = true;                                                                          // Chuyển về chế độ RFID
      }
    }
  }
}
