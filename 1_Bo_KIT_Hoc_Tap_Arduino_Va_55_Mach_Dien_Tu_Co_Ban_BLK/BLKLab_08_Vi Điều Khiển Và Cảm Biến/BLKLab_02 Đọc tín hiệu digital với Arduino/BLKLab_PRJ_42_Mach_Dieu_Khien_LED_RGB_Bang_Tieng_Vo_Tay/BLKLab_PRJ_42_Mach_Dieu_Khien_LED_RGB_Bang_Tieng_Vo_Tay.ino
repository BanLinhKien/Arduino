/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_RED    = 7;
const int PIN_GREEN  = 6;
const int PIN_BLUE   = 5;
const int PIN_SENSOR = 8;

// --- BIẾN ĐIỀU KHIỂN ---
int mode = 0;               // Chế độ hiện tại (0-3)
int count = 0;              // Đếm số lần vỗ tay
unsigned long lastTime = 0; // Lưu thời điểm vỗ tay lần đầu

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_SENSOR, INPUT);
}

void loop() {
  // 1. XỬ LÝ TÍN HIỆU VỖ TAY (DOUBLE CLAP)
  if (digitalRead(PIN_SENSOR) == LOW) { // Phát hiện tiếng động
    delay(150); // Chống dội âm thanh (Debounce)
    count++;

    if (count == 1) {
      lastTime = millis(); // Ghi lại thời điểm vỗ lần 1
    } 
    else if (count == 2) {
      // Nếu vỗ lần 2 trong khoảng 1.5 giây kể từ lần 1
      if ((millis() - lastTime) <= 1500) {
        mode++;
        if (mode > 3) mode = 0; // Reset về chế độ 0 nếu vượt quá 4 chế độ
      }
      count = 0; // Reset biến đếm để chờ chu kỳ vỗ tay mới
    }
  }

  // Tự động reset count nếu lần vỗ thứ 2 quá trễ (> 1.5s)
  if (count == 1 && (millis() - lastTime) > 1500) {
    count = 0;
  }

  // 2. THỰC THI CÁC CHẾ ĐỘ MÀU
  switch (mode) {
    case 0: // Tắt hết
      updateRGB(0, 0, 0);
      break;
    case 1: // Xanh dương
      updateRGB(0, 0, 1);
      break;
    case 2: // Xanh lục
      updateRGB(0, 1, 0);
      break;
    case 3: // Đỏ
      updateRGB(1, 0, 0);
      break;
  }
}

// Hàm bổ trợ để viết code gọn hơn
void updateRGB(int r, int g, int b) {
  digitalWrite(PIN_RED, r);
  digitalWrite(PIN_GREEN, g);
  digitalWrite(PIN_BLUE, b);
}