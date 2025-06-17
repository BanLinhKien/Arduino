/********************************************************************************
 *                            BANLINHKIEN.COM                                   *
 ********************************************************************************                            
 *               HƯỚNG DẪN LÀM BỘ PHÁT HIỆN KHÓI VÀ KHÍ GAS 
 *                ĐIỀU KHIỂN ĐÓNG MỞ CỦA VÀ BẬT QUẠT HÚT           
 Đấu nối:
 *         Arduino          |           SERVO 1
            D10             |           Dây cam
 *         Arduino          |           SERVO 2
            D11             |           Dây cam
 *         Arduino          |           Relay
            D12             |            IN
 *         Arduino          |            MQ2
             A0             |            A0
 *         Arduino          |           Còi chíp
             D9             |          Chân dương
 ********************************************************************************/
#include <Servo.h>
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter bo_loc(2, 2, 0.001);
// Khai báo các chân kết nối
const int mq2Pin = A0;
const int buzzerPin = 9;
const int servo1Pin = 10;
const int servo2Pin = 11;
const int relayPin = 12;
// Giá trị ngưỡng cho mức khí độc
int smokeThreshold = 2000;

// Khởi tạo đối tượng động cơ servo
 Servo servo1; 
 Servo servo2; 

void setup() {
  // Khởi tạo cổng Serial
  Serial.begin(9600);

  // Khởi tạo chân kết nối cho các thiết bị
  pinMode(mq2Pin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Đưa động cơ servo về vị trí ban đầu
  servo1.write(180);
  servo2.write(90);
}

void loop() {
  // Đọc giá trị từ cảm biến MQ2
  int smoke = analogRead(mq2Pin);
  smoke = bo_loc.updateEstimate(smoke);
  // In giá trị lên Serial Monitor để theo dõi
 // Serial.print("Smoke:300 ");
  Serial.println(smoke);

  // Kiểm tra nếu giá trị khí độc vượt quá ngưỡng
  if (smoke > smokeThreshold) {
    digitalWrite(buzzerPin, HIGH);  // Kích hoạt còi
    digitalWrite(relayPin, HIGH);
    servo1.write(90);  // Quay động cơ servo 1 về giữa
    servo2.write(180);  // Quay động cơ servo 2 về giữa

   
  }
  else {
    digitalWrite(buzzerPin, LOW);  // Tắt còi
    digitalWrite(relayPin, LOW);  // Tắt quạt
    servo1.write(180);  // Động cơ servo 1 ở vị trí ban đầu
    servo2.write(90);  // Động cơ servo 2 ở vị trí ban đầu
  }
}
