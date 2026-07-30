/*=================================================================================================
 * BANLINHKIEN.COM                                                                                *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_BUTTON = 8;    // Chân kết nối nút nhấn
const int PIN_LED    = 9;    // Chân kết nối LED

// --- BIẾN ĐIỀU KHIỂN ---
int lastButtonState = HIGH;  // Trạng thái nút nhấn lần cuối (HIGH vì dùng PULLUP)
int buttonFlag      = HIGH;  // Cờ lưu trạng thái ổn định sau khi khử rung
int ledMode         = 0;     // Biến đếm trạng thái LED (0: Tắt, 1: Bật)

unsigned long lastDebounceTime = 0; // Lưu thời điểm cuối cùng nút bị nhiễu
const int debounceDelay = 30;       // Thời gian chờ để xác nhận nút nhấn (30ms)

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP); // Sử dụng điện trở kéo lên nội bộ
  
  // Khởi tạo giao tiếp Serial ở tốc độ 9600 baud
  Serial.begin(9600);
  Serial.println("He thong da khoi dong!");
  Serial.println("Trang thai ban dau -> LED: TAT");
  Serial.println("--------------------------------");
}

void loop() {
  // 1. ĐỌC TRẠNG THÁI NÚT NHẤN (KHỬ RUNG)
  int reading = digitalRead(PIN_BUTTON);

  // Nếu nút vừa thay đổi (do nhấn xuống hoặc do nhiễu)
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset lại đồng hồ đếm thời gian khử rung
  }

  // Nếu trạng thái duy trì ổn định lâu hơn thời gian trễ (30ms)
  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    // Nếu trạng thái thực sự thay đổi so với "cờ" hiện tại
    if (reading != buttonFlag) {
      buttonFlag = reading; // Cập nhật cờ trạng thái mới

      // --- PHẦN HIỂN THỊ TRẠNG THÁI ---
      
      if (buttonFlag == LOW) {
        // Trạng thái LOW (vì dùng INPUT_PULLUP) nghĩa là đang nhấn nút
        Serial.println("Nut nhan: DANG NHAN");
      } 
      else if (buttonFlag == HIGH) {
        // Trạng thái HIGH nghĩa là đã nhả nút
        Serial.println("Nut nhan: DA NHA");
        
        // CHỈ đảo trạng thái LED khi nút được nhả ra
        ledMode = !ledMode; 
        
        // In trạng thái LED mới cập nhật
        Serial.print("Den LED : ");
        if (ledMode == 1) {
          Serial.println("SANG");
        } else {
          Serial.println("TAT");
        }
        Serial.println("--------------------------------");
      }
    }
  }

  // Lưu lại giá trị đọc được cho lần lặp kế tiếp
  lastButtonState = reading;

  // 2. ĐIỀU KHIỂN LED THEO TRẠNG THÁI ledMode
  digitalWrite(PIN_LED, ledMode);
}