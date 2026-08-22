/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_IN = A0;                // Chân nhận tín hiệu Analog
const float SERIES_RESISTOR = 220.0;   // Điện trở cố định 220 Ohm (Mắc từ A0 xuống GND)
const float NOMINAL_RESISTANCE = 47.0; // Điện trở NTC 47D tại 25°C

void setup() {
  pinMode(PIN_IN, INPUT);
  Serial.begin(9600);
  Serial.println("He thong do nhiet do - BANLINHKIEN.COM");
}

void loop() {
  // 1. Đọc giá trị Analog từ cảm biến
  int analogValue = analogRead(PIN_IN);

  // 2. Tính điện áp tại chân A0 (Thang đo 0-5V tương ứng 0-1023)
  float voltage = analogValue * (5.0 / 1023.0);

  // 3. Tính điện trở hiện tại của NTC 47D
  // Công thức: R_ntc = R_fixed * ( (Vin / Vout) - 1 )
  float resistance;
  resistance = SERIES_RESISTOR * ( (5.0 / voltage) - 1.0 );

  // 4. Công thức xấp xỉ tuyến tính (Chỉ chính xác quanh vùng 25 - 30 độ C)
  // Hệ số 1.5 đại diện cho sự thay đổi Ohm trên mỗi độ C (đặc trưng cho NTC 47D)
  float temperature = 25.0 + (NOMINAL_RESISTANCE - resistance) / 1.5; 

  // 5. HIỂN THỊ KẾT QUẢ 
  Serial.print("Dien tro NTC: ");
  Serial.print(resistance, 1);
  Serial.print(" Ohm | Nhiet do: ");
  Serial.print(temperature, 1); // In 1 chữ số thập phân
  Serial.println(" *C");

  delay(500); // Đợi 0.5 giây trước lần đo tiếp theo
}