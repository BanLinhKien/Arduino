/*=================================================================================================
 *                                     BANLINHKIEN.COM                                            *
 ==================================================================================================
*/
// --- KHAI BÁO CẤU HÌNH ---
const int PIN_BUZZER = 8;    // Chân điều khiển còi báo
const int PIN_LED    = 9;    // Chân điều khiển đèn LED báo động
const int PIN_SENSOR = 10;   // Chân nhận tín hiệu từ cảm biến gas (MQ-2)

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_SENSOR, INPUT_PULLUP); // Sử dụng PULLUP để tín hiệu ổn định hơn

  // Đảm bảo hệ thống im lặng khi mới khởi động
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUZZER, LOW);
}

void loop() {
  // Đọc trạng thái cảm biến (0: Phát hiện gas, 1: Bình thường)
  int gasStatus = digitalRead(PIN_SENSOR);

  if (gasStatus == LOW) { 
    // --- TRẠNG THÁI BÁO ĐỘNG ---
    digitalWrite(PIN_BUZZER, HIGH); // Bật còi liên tục để gây chú ý mạnh
    
    // Nháy đèn LED nhanh để cảnh báo trực quan
    digitalWrite(PIN_LED, HIGH);
    delay(100); 
    digitalWrite(PIN_LED, LOW);
    delay(100);
  } 
  else {
    // --- TRẠNG THÁI AN TOÀN ---
    digitalWrite(PIN_LED, LOW);
    digitalWrite(PIN_BUZZER, LOW);
  }
}