/*=================================================================================================
BANLINHKIEN.COM                                            *
==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_TRIG = 10;   // Chân phát sóng siêu âm
const int PIN_ECHO = 9;    // Chân nhận sóng phản hồi

void setup() {
  pinMode(PIN_PIN_TRIG, OUTPUT); // TRIG xuất xung
  pinMode(PIN_ECHO, INPUT);   // ECHO nhận xung
  
  Serial.begin(9600);
  Serial.println("He thong do khoang cach HC-SR04 bat dau...");
}

void loop() {
  // 1. PHÁT XUNG SIÊU ÂM
  // Đảm bảo chân Trig thấp trước khi phát xung
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  
  // Phát một xung HIGH trong đúng 10 micro giây
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // 2. ĐO THỜI GIAN PHẢN HỒI
  // pulseIn trả về thời gian (micro giây) từ khi phát đến khi nhận được sóng dội về
  long duration = pulseIn(PIN_ECHO, HIGH);

  // 3. TÍNH KHOẢNG CÁCH (cm)
  // Vận tốc âm thanh ~343 m/s = 0.0343 cm/us
  // Khoảng cách = (Thời gian * Vận tốc) / 2 (vì có cả lượt đi và lượt về)
  float distance = (duration * 0.0343) / 2;

  // 4. HIỂN THỊ KẾT QUẢ
  Serial.print("Khoang cach: ");
  if (distance >= 400 || distance <= 2) {
    Serial.println("Ngoai pham vi do!"); // HC-SR04 hoạt động tốt nhất từ 2cm - 400cm
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(200); // Đợi 0.2s để tránh nhiễu giữa các lần phát sóng
}