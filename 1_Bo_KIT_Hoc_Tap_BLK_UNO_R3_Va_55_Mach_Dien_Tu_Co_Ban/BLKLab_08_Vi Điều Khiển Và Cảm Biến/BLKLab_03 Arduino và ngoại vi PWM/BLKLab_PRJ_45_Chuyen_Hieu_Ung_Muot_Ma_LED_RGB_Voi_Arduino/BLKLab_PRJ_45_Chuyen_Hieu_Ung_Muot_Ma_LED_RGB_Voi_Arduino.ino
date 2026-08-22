/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// Khai báo các chân PWM kết nối với LED RGB
const int redPin = 11;   
const int greenPin = 10; 
const int bluePin = 9;  

// Thời gian trễ giữa mỗi bước chuyển màu (tốc độ chuyển hiệu ứng)
// Số càng nhỏ thì chuyển màu càng nhanh
const int fadeSpeed = 10; 

void setup() {
  // Cài đặt các chân là đầu ra (OUTPUT)
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // 1. Chuyển dần từ Đỏ sang Xanh lá
  for (int i = 0; i <= 255; i++) {
    setColor(255 - i, i, 0); 
    delay(fadeSpeed);
  }
  
  // 2. Chuyển dần từ Xanh lá sang Xanh dương
  for (int i = 0; i <= 255; i++) {
    setColor(0, 255 - i, i);
    delay(fadeSpeed);
  }
  
  // 3. Chuyển dần từ Xanh dương về lại Đỏ
  for (int i = 0; i <= 255; i++) {
    setColor(i, 0, 255 - i);
    delay(fadeSpeed);
  }
}

// Hàm phụ trợ để thiết lập độ sáng cho cả 3 màu cùng lúc
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}