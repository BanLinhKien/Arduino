#include <Stepper.h>

const int stepsPerRevolution = 64;                                        // Số bước mỗi vòng quay cho động cơ của bạn
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);                      // Khởi tạo đối tượng Stepper cho động cơ

int stepCount = 0;                                                        // Số bước đếm
int motorSpeed = 0;                                                       // Tốc độ mặc định của động cơ
int analogValue = 0;                                                      // Giá trị analog từ biến trở

void setup() {
  Serial.begin(9600);                                                     // Khởi tạo cổng Serial 
}

void loop() {
  analogValue = analogRead(A0);                                           // Đọc giá trị từ chân A0, nơi bạn kết nối biến trở
  motorSpeed = map(analogValue, 0, 1023, 0, 500);                         // Chuyển giá trị analog sang tốc độ động cơ (0-500)
  myStepper.setSpeed(motorSpeed);                                         // Đặt tốc độ động cơ dựa trên giá trị tốc độ
  myStepper.step(stepsPerRevolution);                                     // Quay động cơ 1 vòng theo chiều kim đồng hồ
  stepCount++;                                                            // Tăng số vòng quay

  Serial.print("Revolutions: ");
  Serial.print(stepCount);
  Serial.print(", Speed: ");
  Serial.println(motorSpeed);
}
