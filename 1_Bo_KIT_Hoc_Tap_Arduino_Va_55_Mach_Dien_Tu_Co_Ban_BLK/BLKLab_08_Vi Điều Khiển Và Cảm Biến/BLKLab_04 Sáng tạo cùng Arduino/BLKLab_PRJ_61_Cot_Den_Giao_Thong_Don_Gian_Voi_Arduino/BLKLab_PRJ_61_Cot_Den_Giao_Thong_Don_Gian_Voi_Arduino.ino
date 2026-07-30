/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
// Mảng chứa các chân điều khiển LED 7 thanh (a, b, c, d, e, f, g, dp)
int ledPins[8] = {6, 7, 9, 10, 11, 13, 12, 8}; 

// Chân điều khiển đèn giao thông
int greenLed  = 3;
int yellowLed = 4;
int redLed    = 5;

// Biến điều khiển thời gian
int count = 9;               // Bắt đầu đếm ngược từ 9
int mode = 0;                // Chế độ hiện tại (0: Đỏ, 1: Xanh, 2: Vàng)
unsigned long lastTime = 0;
const int delayTime = 1000;  // 1 giây

// Mảng mã nhị phân cho LED 7 thanh (Anode chung - Mức 0 là sáng)
const byte number[] = {
  0b11000000, // 0
  0b11111001, // 1
  0b10100100, // 2
  0b10110000, // 3
  0b10011001, // 4
  0b10010010, // 5
  0b10000010, // 6
  0b11111000, // 7
  0b10000000, // 8
  0b10010000  // 9
};

void setup() {
  // Cài đặt OUTPUT cho LED 7 thanh
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], HIGH); // Tắt hết (Anode chung)
  }
  
  // Cài đặt OUTPUT cho đèn giao thông
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // 1. QUẢN LÝ BỘ ĐẾM THỜI GIAN (Dùng millis để tránh block chương trình)
  if ((millis() - lastTime) >= delayTime) {
    lastTime = millis();
    count--; // Giảm số giây sau mỗi 1 giây thực tế

    // Xử lý khi bộ đếm về hết
    if (count < 0) {
      mode++;          // Chuyển sang màu đèn tiếp theo
      mode = mode % 3; // Reset vòng lặp 0 -> 1 -> 2
      
      // Thiết lập lại thời gian cho từng màu đèn
      if (mode == 2) { 
        count = 2;     // Đèn Vàng đếm 3 giây (2 về 0)
      } else {
        count = 9;     // Đèn Đỏ và Xanh đếm 10 giây (9 về 0)
      }
    }
  }

  // 2. HIỂN THỊ SỐ LÊN LED 7 THANH
  for (int i = 0; i < 8; i++) {
    // Trích xuất từng bit từ mảng mã số để xuất ra chân tương ứng
    digitalWrite(ledPins[i], (number[count] >> i) & 0b1);
  }

  // 3. ĐIỀU KHIỂN ĐÈN GIAO THÔNG THEO CHẾ ĐỘ (MODE)
  updateTrafficLights(mode);
}

// Hàm bổ trợ điều khiển đèn để code gọn hơn
void updateTrafficLights(int currentMode) {
  switch (currentMode) {
    case 0: // ĐÈN ĐỎ
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      digitalWrite(yellowLed, LOW);
      break;
    case 1: // ĐÈN XANH
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      digitalWrite(yellowLed, LOW);
      break;
    case 2: // ĐÈN VÀNG
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, LOW);
      digitalWrite(yellowLed, HIGH);
      break;
  }
}