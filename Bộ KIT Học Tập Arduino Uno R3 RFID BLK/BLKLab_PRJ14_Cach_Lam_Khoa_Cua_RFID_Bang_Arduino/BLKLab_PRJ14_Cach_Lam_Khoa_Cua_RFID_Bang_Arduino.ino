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
  -A0: nối với dây cam của động cơ servo    
 + Còi buzzer:
  -D8: nối với chân dương của còi buzzer
 + Led:
  -D6: nối với chân dương của led red
  -D7: nối với chân dương của led green
 + Keypad 4x4:
  -D5: nối với chân hàng 1 của keypad 4x4
  -D4: nối với chân hàng 2 của keypad 4x4
  -D3: nối với chân hàng 3 của keypad 4x4
  -D2: nối với chân hàng 4 của keypad 4x4
  -A3: nối với chân cột 1 của keypad 4x4
  -A2: nối với chân cột 2 của keypad 4x4  
  -A1: nối với chân cột 3 của keypad 4x4    
  -không sử dụng cột 4 với các ký tự A,B,C,D
 */
#include <SPI.h>              // Thư viện giao tiếp ngoại vi nối tiếp (cho RFID)
#include <MFRC522.h>          // Thư viện điều khiển module RFID RC522
#include <LiquidCrystal_I2C.h>// Thư viện điều khiển màn hình LCD qua giao tiếp I2C
#include <Keypad.h>           // Thư viện xử lý ma trận phím
#include <Servo.h>            // Thư viện điều khiển động cơ Servo

// --- ĐỊNH NGHĨA CÁC CHÂN CẮM (PIN ASSIGNMENT) ---
#define RST_PIN     9         // Chân Reset của module RFID
#define SS_PIN      10        // Chân Slave Select (SDA) của module RFID
#define SERVO_PIN   A0        // Chân điều khiển Servo (sử dụng Analog làm Digital)
#define BUZZER_PIN  8         // Chân còi báo động
#define RED_LED     6         // Chân LED đỏ (Báo lỗi/Từ chối)
#define GREEN_LED   7         // Chân LED xanh (Thành công/Mở cửa)

// --- KHỞI TẠO ĐỐI TƯỢNG (OBJECT INITIALIZATION) ---
MFRC522 mfrc522(SS_PIN, RST_PIN);     // Khởi tạo thực thể cho RFID
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Địa chỉ I2C 0x27, màn hình 16 cột 2 dòng
Servo sg90;                           // Đối tượng điều khiển động cơ Servo

// --- CẤU HÌNH DỮ LIỆU BẢO MẬT ---
String tagUID = "D0 95 DB 5F";        // UID thẻ chủ (Cần thay đổi theo thẻ thực tế)
char initial_password[5] = "1234";    // Mật khẩu lưu trữ (4 ký tự + ký tự kết thúc)
char password_input[5];               // Bộ đệm lưu trữ các phím vừa nhấn
uint8_t pass_index = 0;               // Biến đếm vị trí ký tự đang nhập

// --- CẤU HÌNH MA TRẬN PHÍM (KEYPAD SETUP) ---
const byte ROWS = 4; // 4 hàng
const byte COLS = 4; // 4 cột (Lưu ý: Bạn đang khai báo 3 chân cắm ở dưới, đây là lỗi tiềm ẩn)
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2};    // Chân nối với hàng (R1, R2, R3, R4)
byte colPins[COLS] = {A3, A2, A1};    // Chân nối với cột (C1, C2, C3) - Cần 4 chân nếu dùng 4x4
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- BIẾN QUẢN LÝ TRẠNG THÁI (STATE MANAGEMENT) ---
boolean RFIDMode = true;              // true: Đang chờ quét thẻ | false: Đang chờ nhập mã
boolean updateLCD = true;             // Cờ hiệu để cập nhật nội dung LCD (tránh nhấp nháy)

void setup() {
  Serial.begin(9600);                 // Khởi tạo Serial Monitor để debug
  SPI.begin();                        // Khởi tạo bus SPI
  mfrc522.PCD_Init();                 // Khởi tạo module RFID
  
  lcd.init();                         // Khởi tạo màn hình LCD
  lcd.backlight();                    // Bật đèn nền LCD
  
  sg90.attach(SERVO_PIN);             // Gắn chân điều khiển cho Servo
  sg90.write(0);                      // Thiết lập góc quay ban đầu (Cửa đóng)
  
  pinMode(BUZZER_PIN, OUTPUT);        // Thiết lập chân đầu ra cho Còi
  pinMode(RED_LED, OUTPUT);           // Thiết lập chân đầu ra cho LED Đỏ
  pinMode(GREEN_LED, OUTPUT);         // Thiết lập chân đầu ra cho LED Xanh
  
  Serial.println(F("He thong san sang...")); // F() giúp tiết kiệm bộ nhớ RAM
}

void beep(int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void unlock() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MAT KHAU DUNG");
  lcd.setCursor(0, 1);
  lcd.print("DANG MO CUA...");
  
  digitalWrite(GREEN_LED, HIGH);      // Bật LED xanh báo hiệu
  beep(200);                          // Bíp xác nhận
  sg90.write(180);                    // Quay Servo đến góc 180 độ để mở khóa
  delay(3000);                        // Giữ cửa mở trong 3 giây
  
  sg90.write(0);                      // Tự động đóng cửa sau khi hết thời gian
  digitalWrite(GREEN_LED, LOW);
  
  // Reset trạng thái về ban đầu để đón người tiếp theo
  RFIDMode = true;
  updateLCD = true;
  pass_index = 0;
}

void accessDenied(String msg) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(msg);
  digitalWrite(RED_LED, HIGH);        // Bật LED đỏ báo lỗi
  beep(1000);                         // Báo động kéo dài 1 giây
  delay(1000);
  digitalWrite(RED_LED, LOW);
  
  // Reset trạng thái quay lại bước quét thẻ
  RFIDMode = true;
  updateLCD = true;
  pass_index = 0;
}

void loop() {
  // --- CHẾ ĐỘ 1: QUÉT THẺ RFID ---
  if (RFIDMode) {
    if (updateLCD) {
      lcd.clear();
      lcd.print("  CUA DANG KHOA ");
      lcd.setCursor(0, 1);
      lcd.print(" QUET THE RFID  ");
      updateLCD = false;
    }

    // Kiểm tra xem có thẻ mới được đưa vào đầu đọc hay không
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

    // Tiến hành đọc UID từ thẻ
    String readUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      // Định dạng HEX: Thêm số 0 nếu giá trị nhỏ hơn 16 để đủ 2 ký tự
      readUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      readUID.concat(String(mfrc522.uid.uidByte[i], HEX));
      if (i < mfrc522.uid.size - 1) readUID.concat(" "); // Thêm dấu cách giữa các byte
    }
    readUID.toUpperCase(); // Chuyển sang chữ hoa để so sánh
    
    Serial.print(F("The vua quet: "));
    Serial.println(readUID);

    // So sánh UID đọc được với UID chủ đã lưu
    if (readUID == tagUID) {
      beep(100);
      lcd.clear();
      lcd.print("THE CHINH XAC!");
      delay(1000);
      lcd.clear();
      lcd.print("NHAP MAT KHAU:");
      lcd.setCursor(0, 1);
      RFIDMode = false; // Chuyển sang chế độ nhập Keypad
    } else {
      accessDenied("THE KHONG DUNG!");
    }
  } 
  
  // --- CHẾ ĐỘ 2: NHẬP MẬT MÃ QUA KEYPAD ---
  else {
    char key = keypad.getKey(); // Đọc phím nhấn từ Keypad
    
    if (key) {
      if (key == '*') { // Phím '*' dùng để xóa và nhập lại từ đầu
        pass_index = 0;
        lcd.setCursor(0, 1);
        lcd.print("                "); // Xóa sạch dòng 2 của LCD
        lcd.setCursor(0, 1);
        beep(50);
      } 
      else if (pass_index < 4) { // Chỉ nhận tối đa 4 ký tự
        beep(50);
        password_input[pass_index++] = key;
        lcd.print("*"); // Hiển thị dấu sao để bảo mật mật khẩu
      }

      // Khi đã nhập đủ 4 ký tự, tiến hành kiểm tra
      if (pass_index == 4) {
        password_input[4] = '\0'; // Thêm ký tự Null để kết thúc chuỗi C-style string
        delay(500);               // Chờ một chút để người dùng thấy dấu '*' cuối cùng
        
        // So sánh chuỗi nhập vào với mật khẩu mặc định
        if (strcmp(password_input, initial_password) == 0) {
          unlock();               // Mở cửa
        } else {
          accessDenied("SAI MAT KHAU!"); // Báo lỗi
        }
      }
    }
  }
}