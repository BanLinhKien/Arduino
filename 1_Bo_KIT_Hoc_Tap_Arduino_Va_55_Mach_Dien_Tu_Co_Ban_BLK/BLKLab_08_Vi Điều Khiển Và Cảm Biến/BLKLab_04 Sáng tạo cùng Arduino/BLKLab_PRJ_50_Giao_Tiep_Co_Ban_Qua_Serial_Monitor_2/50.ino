// --- KHAI BÁO CẤU HÌNH ---
const int PIN_LED = 9;   // Chân kết nối LED (qua trở 220 Ohm)
char incomingByte;       // Biến lưu ký tự nhận được từ máy tính

void setup() {
  pinMode(PIN_LED, OUTPUT);
  
  // Khởi tạo giao tiếp Serial với tốc độ 9600 baud
  Serial.begin(9600);
  
  // In thông báo hướng dẫn lên Serial Monitor
  Serial.println("--- HE THONG DIEU KHIEN LED ---");
  Serial.println("Nhap '1' de BAT LED");
  Serial.println("Nhap '0' de TAT LED");
}

void loop() {
  // Kiểm tra xem có dữ liệu nào được gửi từ máy tính đến không
  if (Serial.available() > 0) {
    
    // Đọc ký tự đầu tiên trong bộ đệm Serial
    incomingByte = Serial.read();

    // Xử lý lệnh dựa trên ký tự nhận được
    if (incomingByte == '1') {
      digitalWrite(PIN_LED, HIGH);
      Serial.println("Lenh: BAT LED [OK]");
    } 
    else if (incomingByte == '0') {
      digitalWrite(PIN_LED, LOW);
      Serial.println("Lenh: TAT LED [OK]");
    }
  }
}