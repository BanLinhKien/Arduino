/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_BUZZER = 8;    // Chân còi báo động
const int PIN_LED    = 9;    // Chân đèn LED chỉ thị
const int PIN_SENSOR = 10;   // Chân cảm biến vật cản hồng ngoại

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_SENSOR, INPUT);

  // Trạng thái chờ ban đầu
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUZZER, LOW);
}

void loop() {
  // Đọc trạng thái cảm biến (1: Bị chắn/Phát hiện vật cản, 0: Bình thường)
  int isTriggered = digitalRead(PIN_SENSOR);

  if (isTriggered == HIGH) { 
    // --- CHẾ ĐỘ CẢNH BÁO ---
    // Vòng lặp chạy 30 lần, mỗi lần 100ms (50ms bật + 50ms tắt)
    // Tổng thời gian hú còi và nháy đèn khoảng 3 giây
    for (int i = 0; i < 30; i++) {
      digitalWrite(PIN_LED, HIGH);
      digitalWrite(PIN_BUZZER, HIGH);
      delay(50);
      
      digitalWrite(PIN_LED, LOW);
      digitalWrite(PIN_BUZZER, LOW);
      delay(50);
    }
  } 
  else {
    // --- CHẾ ĐỘ GIÁM SÁT ---
    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
  }
}