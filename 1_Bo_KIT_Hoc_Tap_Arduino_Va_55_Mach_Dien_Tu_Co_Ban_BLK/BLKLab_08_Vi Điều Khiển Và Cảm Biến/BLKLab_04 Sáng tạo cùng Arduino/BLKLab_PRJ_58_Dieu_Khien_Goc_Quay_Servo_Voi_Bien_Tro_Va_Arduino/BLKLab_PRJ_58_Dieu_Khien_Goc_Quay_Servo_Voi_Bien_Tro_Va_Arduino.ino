/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

#include <Servo.h>            // Thư viện điều khiển servo

// --- KHAI BÁO CẤU HÌNH ---
Servo myServo;                // Khởi tạo đối tượng servo
const int PIN_SERVO = 9;      // Chân tín hiệu điều khiển servo
const int PIN_POT   = A0;     // Chân đọc giá trị biến trở

void setup() {
  myServo.attach(PIN_SERVO);  // Gán chân điều khiển servo vào chân 9
  pinMode(PIN_POT, INPUT);    // Cài đặt chân A0 là đầu vào analog
  
  Serial.begin(9600);         // Khởi tạo Serial để theo dõi giá trị nếu cần
}

void loop() {
  // 1. Đọc giá trị Analog từ biến trở (0 - 1023)
  int value = analogRead(PIN_POT); 

  // 2. Chuyển đổi giá trị Analog sang góc quay (0 - 180 độ)
  int angle = (value * 180.0) / 1023.0; 

  // 3. Điều khiển servo quay theo góc đã tính toán
  myServo.write(angle); 

  // In giá trị lên Serial Monitor để kiểm tra
  Serial.print("Gia tri ADC: "); Serial.print(value);
  Serial.print(" => Goc quay: "); Serial.println(angle);

  delay(15); // Đợi một chút để servo kịp di chuyển đến vị trí mới
}