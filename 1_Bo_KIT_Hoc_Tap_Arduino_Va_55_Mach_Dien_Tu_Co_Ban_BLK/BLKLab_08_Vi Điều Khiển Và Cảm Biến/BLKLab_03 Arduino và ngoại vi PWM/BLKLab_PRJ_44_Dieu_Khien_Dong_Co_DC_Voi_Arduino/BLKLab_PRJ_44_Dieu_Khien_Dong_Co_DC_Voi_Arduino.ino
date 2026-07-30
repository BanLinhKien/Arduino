/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_GATE = 9;  

void setup() {
  // Cài đặt chân điều khiển là đầu ra
  pinMode(PIN_GATE, OUTPUT);
}

void loop() {
  // 1. Chạy tối đa tốc độ (100% công suất)
  // Giá trị 255 tương ứng với điện áp liên tục VCC (thường là 5V)
  analogWrite(PIN_GATE, 255); 
  delay(2000); 

  // 2. Chạy tốc độ trung bình (Khoảng 50% công suất)
  // Giá trị 127 tạo ra xung PWM với độ rộng 50%
  analogWrite(PIN_GATE, 127);
  delay(2000);

  // 3. Dừng động cơ (0% công suất)
  analogWrite(PIN_GATE, 0);
  delay(2000);
}