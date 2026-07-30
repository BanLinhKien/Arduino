
/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// Khai báo chân linh kiện
const int buttonPin = 11;
const int IN2 = 10;
const int IN1 = 9;
const int buzzer = 8;
const int redLed = 7;
const int yellowLed = 6;

// Biến quản lý trạng thái
bool isForward = true; 
bool lastButtonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // Trạng thái mặc định khi mới cấp điện
  updateMotor();
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin);

  // Phát hiện sự kiện nhấn nút (chuyển từ không nhấn sang nhấn)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50); // Khử nhiễu phím bấm (Debounce)
    
    // 1. Còi báo hiệu nhấn nút thành công
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);

    // 2. Dừng động cơ trong 300ms để bảo vệ mạch cầu H trước khi đảo
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    delay(1000);

    // 3. Đảo trạng thái chiều quay
    isForward = !isForward;
    
    // 4. Cập nhật đầu ra
    updateMotor();
  }
  
  lastButtonState = currentButtonState;
}

// Hàm thực thi điều khiển
void updateMotor() {
  if (isForward) {
    // Chiều thuận: IN1 HIGH, Đèn đỏ sáng
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(redLed, HIGH);
    digitalWrite(yellowLed, LOW);
  } else {
    // Chiều nghịch: IN2 HIGH, Đèn vàng sáng
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, HIGH);
  }
}