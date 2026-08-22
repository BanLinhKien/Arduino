/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

#include <Servo.h>         // Thư viện điều khiển servo

// --- KHAI BÁO CẤU HÌNH ---
Servo myServo;             // Khởi tạo đối tượng servo
const int PIN_SERVO    = 3; // Chân tín hiệu servo
const int PIN_TRIG     = 7; // Chân phát siêu âm
const int PIN_ECHO     = 6; // Chân thu siêu âm
const int PIN_LED_XANH = 4; // Đèn báo mở cửa
const int PIN_LED_DO   = 5; // Đèn báo đóng cửa

// --- BIẾN TÙY CHỈNH ---
const int LIMIT_DIST = 15;   // Khoảng cách kích hoạt (15cm)
const int OPEN_TIME  = 1500; // Thời gian giữ cửa mở (1.5 giây)

void setup() {
  myServo.attach(PIN_SERVO);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED_XANH, OUTPUT);
  pinMode(PIN_LED_DO, OUTPUT);
  
  // Trạng thái ban đầu: Cửa đóng (0 độ), đèn đỏ sáng
  myServo.write(0);
  digitalWrite(PIN_LED_DO, HIGH);
  digitalWrite(PIN_LED_XANH, LOW);
  
  Serial.begin(9600); // Theo dõi khoảng cách qua máy tính
}

void loop() {
  // 1. PHÁT XUNG SIÊU ÂM
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // 2. TÍNH KHOẢNG CÁCH
  long duration = pulseIn(PIN_ECHO, HIGH);
  float distance = (duration * 0.0343) / 2;

  // 3. LOGIC ĐIỀU KHIỂN CỬA VÀ ĐÈN
  if (distance > 0 && distance <= LIMIT_DIST) {
    // PHÁT HIỆN VẬT CẢN -> MỞ CỬA
    Serial.println("Co nguoi! Dang mo cua...");
    
    digitalWrite(PIN_LED_XANH, HIGH); // Bật đèn xanh
    digitalWrite(PIN_LED_DO, LOW);    // Tắt đèn đỏ
    myServo.write(90);                // Servo quay 90 độ để mở
    
    delay(OPEN_TIME);                 // Giữ cửa mở trong 1.5 giây
  } 
  else {
    // KHÔNG CÓ NGƯỜI -> ĐÓNG CỬA
    digitalWrite(PIN_LED_XANH, LOW);  // Tắt đèn xanh
    digitalWrite(PIN_LED_DO, HIGH);   // Bật đèn đỏ
    myServo.write(0);                 // Servo quay về 0 độ để đóng
    
    delay(200); // Nghỉ 0.2 giây trước chu kỳ quét tiếp theo
  }
}