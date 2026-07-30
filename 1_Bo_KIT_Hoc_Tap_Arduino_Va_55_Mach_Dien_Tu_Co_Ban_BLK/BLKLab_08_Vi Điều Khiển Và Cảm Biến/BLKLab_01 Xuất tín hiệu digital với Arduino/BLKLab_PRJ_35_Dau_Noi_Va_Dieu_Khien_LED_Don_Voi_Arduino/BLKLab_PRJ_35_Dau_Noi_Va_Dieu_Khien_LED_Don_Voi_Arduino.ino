/*=================================================================================================
 *                                     BANLINHKIEN.COM                                            *
 ==================================================================================================
                         HƯỚNG DẪN ĐIỀU KHIỂN ĐỘNG CƠ SERVO BẰNG BIẾN TRỞ
 ==================================================================================================
*/
// --- KHAI BÁO BIẾN ---
const int LED_PIN = 10;   

void setup() {
  // Cấu hình chân LED_PIN là đầu ra (OUTPUT)
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // Xuất mức cao (5V): BẬT LED
  delay(500);                  // Dừng chương trình 500ms (0.5 giây)

  digitalWrite(LED_PIN, LOW);  // Xuất mức thấp (0V): TẮT LED
  delay(500);                  // Dừng chương trình 500ms
}