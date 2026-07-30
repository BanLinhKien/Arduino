/*=================================================================================================
 *                                     BANLINHKIEN.COM                                            *
 ==================================================================================================
*/
// --- KHAI BÁO CẤU HÌNH ---
const int PIN_BLUE  = 11;    // Chân điều khiển LED Xanh dương
const int PIN_GREEN = 12;    // Chân điều khiển LED Xanh lục
const int PIN_RED   = 13;    // Chân điều khiển LED Đỏ
const int DELAY_MS  = 300;   // Thời gian chờ giữa các bước (300ms)

void setup() {
  // Thiết lập tất cả các chân LED là đầu ra (OUTPUT)
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
}

void loop() {
  // Bước 1: Chỉ bật màu Xanh dương (Blue)
  digitalWrite(PIN_RED, LOW);   // Tắt Đỏ
  digitalWrite(PIN_GREEN, LOW); // Tắt Xanh lục
  digitalWrite(PIN_BLUE, HIGH); // Bật Xanh dương
  delay(DELAY_MS);

  // Bước 2: Chỉ bật màu Xanh lục (Green)
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_GREEN, HIGH);
  digitalWrite(PIN_BLUE, LOW);
  delay(DELAY_MS);

  // Bước 3: Chỉ bật màu Đỏ (Red)
  digitalWrite(PIN_RED, HIGH);
  digitalWrite(PIN_GREEN, LOW);
  digitalWrite(PIN_BLUE, LOW);
  delay(DELAY_MS);
}