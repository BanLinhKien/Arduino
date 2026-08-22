/*=================================================================================================
 * BANLINHKIEN.COM                             *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_SIGNAL = 10;   // Chân nhận tín hiệu từ mắt thu hồng ngoại
const int PIN_MOTOR  = 9;    // Chân điều khiển tốc độ Motor (PWM)
const int PIN_BUZZER = 8;    // Chân điều khiển còi báo (Buzzer)

// --- BIẾN ĐIỀU KHIỂN ---
int lastSignalState = HIGH;  // Trạng thái tín hiệu trước đó
int signalFlag      = HIGH;  // Cờ xác nhận tín hiệu đã ổn định
int speedMode       = 0;     // Chế độ tốc độ (0, 1, 2, 3)

unsigned long lastDebounceTime = 0; // Thời điểm cuối cùng tín hiệu thay đổi
const int debounceDelay = 30;       // Thời gian lọc nhiễu 30ms

void setup() {
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);       // Cấu hình chân còi là đầu ra
  pinMode(PIN_SIGNAL, INPUT_PULLUP);
  
  // Dừng động cơ và tắt còi khi khởi động
  digitalWrite(PIN_MOTOR, LOW);
  digitalWrite(PIN_BUZZER, LOW);
}

void loop() {
  // 1. LỌC NHIỄU VÀ XỬ LÝ TÍN HIỆU ĐẦU VÀO
  int reading = digitalRead(PIN_SIGNAL);

  // Nếu tín hiệu thay đổi (do có vật cản hoặc nhiễu)
  if (reading != lastSignalState) {
    lastDebounceTime = millis();
  }

  // Khi tín hiệu đã ổn định đủ lâu
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != signalFlag) {
      signalFlag = reading;

      // Kích hoạt khi tín hiệu quay lại mức HIGH (Vật cản đã đi qua)
      if (signalFlag == HIGH) {
        speedMode++;
        if (speedMode > 3) speedMode = 0; // Quay vòng 4 cấp độ

        // --- PHÁT TIẾNG BÍP BÁO HIỆU ---
        digitalWrite(PIN_BUZZER, HIGH); // Bật còi
        delay(100);                     // Kêu trong 0.1 giây
        digitalWrite(PIN_BUZZER, LOW);  // Tắt còi
      }
    }
  }
  lastSignalState = reading;

  // 2. ĐIỀU KHIỂN TỐC ĐỘ MOTOR THEO CẤP ĐỘ
  switch (speedMode) {
    case 0: // Tắt
      analogWrite(PIN_MOTOR, 0);
      break;
    case 1: // Quay chậm (Số 1)
      analogWrite(PIN_MOTOR, 60);  
      break;
    case 2: // Quay vừa (Số 2)
      analogWrite(PIN_MOTOR, 150);
      break;
    case 3: // Quay mạnh (Số 3)
      analogWrite(PIN_MOTOR, 255);
      break;
  }
}