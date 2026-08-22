/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_BUZZER = 8;     // Chân kết nối còi báo động
const int PIN_LED    = 9;     // Chân kết nối đèn LED cảnh báo
const int PIN_FLAME  = 10;    // Chân tín hiệu từ cảm biến lửa (Flame Sensor)

// --- BIẾN TÙY CHỈNH ---
int blinkSpeed = 50;          // Tốc độ nháy đèn (ms) khi có báo động

void setup() {
  // Thiết lập chức năng cho các chân
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_FLAME, INPUT);

  // Đảm bảo các thiết bị tắt khi vừa khởi động
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUZZER, LOW);
}

void loop() {
  // Đọc trạng thái cảm biến (Mức LOW thường là phát hiện lửa)
  int fireStatus = digitalRead(PIN_FLAME);

  if (fireStatus == LOW) { 
    // --- TRẠNG THÁI NGUY HIỂM ---
    digitalWrite(PIN_BUZZER, HIGH); // Bật còi báo động liên tục
    
    // Tạo hiệu ứng chớp tắt đèn LED
    digitalWrite(PIN_LED, HIGH);
    delay(blinkSpeed);
    digitalWrite(PIN_LED, LOW);
    delay(blinkSpeed);
  } 
  else {
    // --- TRẠNG THÁI AN TOÀN ---
    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
  }
}