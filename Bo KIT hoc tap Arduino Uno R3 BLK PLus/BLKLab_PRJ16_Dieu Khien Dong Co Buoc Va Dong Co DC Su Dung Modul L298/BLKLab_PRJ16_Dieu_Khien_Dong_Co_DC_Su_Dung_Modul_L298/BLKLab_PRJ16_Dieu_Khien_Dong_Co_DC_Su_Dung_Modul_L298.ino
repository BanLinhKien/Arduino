/*
 * ===========================================================================================================================*
 *                                                        BANLINHKIEN.COM
 * ===========================================================================================================================*                           
 *                                        Điều khiển động cơ DC sử dụng Modul L298 V3
 * Đấu nối: 
 + Arduino với Modul L298 V3:
  -D5: nối với chân IN1 của Modul L298 V3 
  -D6: nối với chân IN2 của Modul L298 V3 
  -D10: nối với chân IN3 của Modul L298 V3 
  -D11: nối với chân IN4 của Modul L298 V3 
 + Modul L298 V3 với Động cơ DC:
  -OUTA: nối với chân VCC của Động cơ 1
  -OUTB: nối với chân GND của Động cơ 1
  -OUTC: nối với chân VCC của Động cơ 2
  -OUTD: nối với chân GND của Động cơ 2
 */

const int IN1 = 5;                                            // Định nghĩa chân số 5 là IN1
const int IN2 = 6;                                            // Định nghĩa chân số 6 là IN2
const int IN3 = 10;                                           // Định nghĩa chân số 10 là IN3
const int IN4 = 11;                                           // Định nghĩa chân số 11 là IN4
const int MAX_SPEED = 255;                                    // Tốc độ tối đa của động cơ
const int MIN_SPEED = 0;                                      // Tốc độ tối thiểu của động cơ

int currentSpeed = 0;                                         // Biến lưu trữ tốc độ hiện tại của động cơ
int currentState = 0;                                         // Biến lưu trữ trạng thái hiện tại của chương trình

void setup()
{
  pinMode(IN1, OUTPUT);                                       // Đặt chân IN1 là OUTPUT
  pinMode(IN2, OUTPUT);                                       // Đặt chân IN2 là OUTPUT
  pinMode(IN3, OUTPUT);                                       // Đặt chân IN3 là OUTPUT
  pinMode(IN4, OUTPUT);                                       // Đặt chân IN4 là OUTPUT
}

void stopMotors() {
  digitalWrite(IN1, LOW);                                     // Tắt đầu ra của IN1
  digitalWrite(IN2, LOW);                                     // Tắt đầu ra của IN2
  digitalWrite(IN3, LOW);                                     // Tắt đầu ra của IN3
  digitalWrite(IN4, LOW);                                     // Tắt đầu ra của IN4
}

void setSpeed(int speed) {
  currentSpeed = constrain(speed, MIN_SPEED, MAX_SPEED);      // Giới hạn tốc độ trong khoảng MIN_SPEED đến MAX_SPEED
}

void motor_1_Tien() {
  digitalWrite(IN1, HIGH);                                    // Đặt IN1 lên mức HIGH
  analogWrite(IN2, 255 - currentSpeed);                       // Điều khiển IN2 với PWM, giảm tốc độ
}

void motor_1_Lui() {
  digitalWrite(IN1, LOW);                                     // Đặt IN1 xuống mức LOW
  analogWrite(IN2, currentSpeed);                             // Điều khiển IN2 với PWM, tăng tốc độ
}

void motor_2_Tien() {
  analogWrite(IN3, currentSpeed);                             // Điều khiển IN3 với PWM, tăng tốc độ
  digitalWrite(IN4, LOW);                                     // Đặt IN4 xuống mức LOW
}

void motor_2_Lui() {
  analogWrite(IN4, 255 - currentSpeed);                       // Điều khiển IN4 với PWM, giảm tốc độ
  digitalWrite(IN3, HIGH);                                    // Đặt IN3 lên mức HIGH
}

void loop()
{
  switch (currentState) {                                     // Bắt đầu kiểm tra các trạng thái
    case 0:                                                   // Chạy trái
      setSpeed(MAX_SPEED);                                    // Đặt tốc độ tối đa
      motor_1_Lui();                                          // Chạy động cơ 1 lùi
      motor_2_Tien();                                         // Chạy động cơ 2 tiến
      delay(5000);                                            // Chạy trong 5 giây
      break;

    case 1:                                                   // Chạy phải
      setSpeed(MAX_SPEED);                                    // Đặt tốc độ tối đa
      motor_1_Tien();                                         // Chạy động cơ 1 tiến
      motor_2_Lui();                                          // Chạy động cơ 2 lùi
      delay(5000);                                            // Chạy trong 5 giây
      break;

    case 2:                                                   // Chạy thẳng
      setSpeed(MAX_SPEED);                                    // Đặt tốc độ tối đa
      motor_1_Tien();                                         // Chạy động cơ 1 tiến
      motor_2_Tien();                                         // Chạy động cơ 2 tiến
      delay(5000);                                            // Chạy trong 5 giây
      break;

    case 3:                                                   // Chạy ngược lại
      setSpeed(MAX_SPEED);                                    // Đặt tốc độ tối đa
      motor_1_Lui();                                          // Chạy động cơ 1 lùi
      motor_2_Lui();                                          // Chạy động cơ 2 lùi
      delay(5000);                                            // Chạy trong 5 giây
      break;

    case 4:                                                   // Dừng trong 2 giây
      stopMotors();                                           // Tắt tất cả động cơ
      delay(2000);                                            // Dừng trong 2 giây
      break;
  }

  currentState = (currentState + 1) % 5;                      // Chuyển đổi trạng thái (có 5 trạng thái)
}
