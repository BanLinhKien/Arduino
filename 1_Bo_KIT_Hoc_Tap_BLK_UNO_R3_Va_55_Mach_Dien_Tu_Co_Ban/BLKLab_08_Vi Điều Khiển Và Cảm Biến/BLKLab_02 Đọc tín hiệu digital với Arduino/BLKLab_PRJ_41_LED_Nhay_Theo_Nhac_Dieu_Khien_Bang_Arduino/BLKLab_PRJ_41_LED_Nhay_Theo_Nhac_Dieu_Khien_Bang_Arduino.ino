/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
int ledPins[] = {3, 4, 5, 6, 7}; // Mảng chứa 5 chân điều khiển LED
const int PIN_SENSOR = 8;        // Chân nhận tín hiệu từ cảm biến âm thanh
const int LED_COUNT  = 5;        // Tổng số lượng LED

void setup() {
  // Sử dụng vòng lặp để thiết lập OUTPUT cho toàn bộ LED
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Cài đặt chân cảm biến
  pinMode(PIN_SENSOR, INPUT);
}

void loop() {
  // Đọc tín hiệu từ cảm biến âm thanh (Mức LOW thường là có âm thanh)
  if (digitalRead(PIN_SENSOR) == LOW) { 

    // Hiệu ứng nháy đồng loạt 3 lần cực nhanh theo nhịp Bass
    for (int nhay = 0; nhay < 3; nhay++) {
      
      // Bật tất cả các đèn
      for (int i = 0; i < LED_COUNT; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
      delay(50); // Thời gian sáng

      // Tắt tất cả các đèn
      for (int i = 0; i < LED_COUNT; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      delay(10); // Thời gian nghỉ giữa các lần nháy
    }
  } 
  else {
    // Nếu không có âm thanh, tắt toàn bộ LED
    for (int i = 0; i < LED_COUNT; i++) {
      digitalWrite(ledPins[i], LOW);
    }
  }
}