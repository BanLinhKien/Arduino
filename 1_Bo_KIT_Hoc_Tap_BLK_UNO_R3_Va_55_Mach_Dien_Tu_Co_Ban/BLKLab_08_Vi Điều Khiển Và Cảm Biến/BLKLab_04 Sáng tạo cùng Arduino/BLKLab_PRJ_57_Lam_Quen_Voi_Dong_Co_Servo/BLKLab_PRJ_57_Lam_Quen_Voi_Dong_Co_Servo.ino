/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

#include <Servo.h>      // Khai báo thư viện điều khiển servo

// --- KHAI BÁO CẤU HÌNH ---
Servo myServo;          // Tạo đối tượng servo để điều khiển
const int SERVO_PIN = 9; // Chân tín hiệu điều khiển Servo

void setup() {
  myServo.attach(SERVO_PIN); // Gắn chân tín hiệu Servo vào chân số 9
  myServo.write(0);          // Đưa Servo về vị trí xuất phát (0 độ)
  delay(1000);               // Chờ 1 giây để Servo ổn định vị trí
}

void loop() {
  // Quay servo đến góc 90 độ (Vị trí chính giữa)
  myServo.write(90);  
  delay(500); 
  
  // Quay servo đến góc 180 độ (Vị trí tối đa)
  myServo.write(180); 
  delay(500); 
  
  // Quay lại góc 90 độ
  myServo.write(90);  
  delay(500); 
  
  // Quay về góc 0 độ (Vị trí ban đầu)
  myServo.write(0);   
  delay(500); 
}