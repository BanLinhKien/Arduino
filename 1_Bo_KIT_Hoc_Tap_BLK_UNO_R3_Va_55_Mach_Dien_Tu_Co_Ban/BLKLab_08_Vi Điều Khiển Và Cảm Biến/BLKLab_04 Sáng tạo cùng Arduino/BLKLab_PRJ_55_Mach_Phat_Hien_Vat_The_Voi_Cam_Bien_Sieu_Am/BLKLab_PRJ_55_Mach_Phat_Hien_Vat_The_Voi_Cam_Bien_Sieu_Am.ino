/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_TRIG = 10;     // Chân phát sóng siêu âm
const int PIN_ECHO = 9;      // Chân nhận sóng phản hồi
const int PIN_LED  = 8;      // Chân điều khiển đèn LED

// --- BIẾN TÙY CHỈNH ---
const int LIMIT_DIST = 40;   // Khoảng cách kích hoạt đèn (40cm)
const int OPEN_TIME  = 2000; // Thời gian duy trì sáng (2 giây)

void setup() {
  pinMode(PIN_TRIG, OUTPUT); // Thiết lập chân Trig là đầu ra
  pinMode(PIN_ECHO, INPUT);  // Thiết lập chân Echo là đầu vào
  pinMode(PIN_LED, OUTPUT);  // Thiết lập chân LED là đầu ra
  
  // Đảm bảo các thiết bị tắt khi vừa khởi động
  digitalWrite(PIN_LED, LOW);
  
  Serial.begin(9600);        // Khởi tạo giao tiếp Serial để theo dõi khoảng cách
  Serial.println("He thong den thong minh - BANLINHKIEN.COM");
}

void loop() {
  // 1. PHÁT XUNG SIÊU ÂM
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // 2. ĐO THỜI GIAN VÀ TÍNH KHOẢNG CÁCH
  long duration = pulseIn(PIN_ECHO, HIGH);
  float distance = (duration * 0.0343) / 2;

  // 3. LOGIC ĐIỀU KHIỂN ĐÈN
  if (distance > 0 && distance < LIMIT_DIST) {
    // Nếu phát hiện có vật cản trong phạm vi 40cm
    digitalWrite(PIN_LED, HIGH);   // Bật đèn
    Serial.print("Phat hien vat can o: ");
    Serial.print(distance);
    Serial.println("cm - BAT DEN");
    
    delay(OPEN_TIME);              // Giữ đèn sáng trong 2 giây
  } 
  else {
    // Nếu không có vật cản hoặc ngoài phạm vi
    digitalWrite(PIN_LED, LOW);    // Tắt đèn
  }

  delay(100); // Nghỉ 0.1 giây trước khi đo chu kỳ tiếp theo
}