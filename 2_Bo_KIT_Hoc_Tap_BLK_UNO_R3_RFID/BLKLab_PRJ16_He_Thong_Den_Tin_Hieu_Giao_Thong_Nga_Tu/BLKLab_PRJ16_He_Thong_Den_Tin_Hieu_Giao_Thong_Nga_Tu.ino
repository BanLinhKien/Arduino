/*
  ================================================================================================
 *                                     BANLINHKIEN.COM                                            *
  ================================================================================================
 * - Chân thanh (Segments): 
  a->D8
  b->D7
  c->D6
  d->D5
  e->D4
  f->D3
  g->D2
  h->VCC
 * - Chân chọn số (Digits): 
  Nghìn  ---->D9
  Trăm   ---->D19
  Chục   ---->D11
  Đơn vị ---->D12
 * * B. ĐÈN GIAO THÔNG (LED ĐƠN):
 * - Hướng Nam-Bắc (NS): Đỏ->A0, Vàng->A1, Xanh->A2
 * - Hướng Đông-Tây (EW): Đỏ->A3, Vàng->A4, Xanh->A5
 */

// --- KHAI BÁO CHÂN ---
const int segPins[7] = {8, 7, 6, 5, 4, 3, 2}; // a, b, c, d, e, f, g

// Chân chọn vị trí số trên module
const int digitUnit = 12;     // Hàng đơn vị
const int digitTen = 11;      // Hàng chục
const int digitHundred = 10;  // Hàng trăm
const int digitThousand = 9;  // Hàng nghìn

// Chân đèn giao thông
const int NS_R = A0, NS_Y = A1, NS_G = A2;
const int EW_R = A3, EW_Y = A4, EW_G = A5;

// Mã hex cho LED Anot chung (LOW = Sáng nét, HIGH = Tắt nét)
const byte numCode[10] = {
  0b11000000, // 0
  0b11111001, // 1
  0b10100100, // 2
  0b10110000, // 3
  0b10011001, // 4
  0b10010010, // 5
  0b10000010, // 6
  0b11111000, // 7
  0b10000000, // 8
  0b10010000  // 9
};

// --- BIẾN TOÀN CỤC CHO LOGIC ---
unsigned long previousMillis = 0;
int countDown = 15;
bool isNSRed = true; // Biến trạng thái: true -> Nam-Bắc Đỏ, Đông-Tây Xanh/Vàng

void setup() {
  // Cài đặt OUTPUT cho LED 7 thanh
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }
  pinMode(digitUnit, OUTPUT); 
  pinMode(digitTen, OUTPUT);
  pinMode(digitHundred, OUTPUT); 
  pinMode(digitThousand, OUTPUT);

  // Cài đặt OUTPUT cho Đèn giao thông
  pinMode(NS_R, OUTPUT); pinMode(NS_Y, OUTPUT); pinMode(NS_G, OUTPUT);
  pinMode(EW_R, OUTPUT); pinMode(EW_Y, OUTPUT); pinMode(EW_G, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Logic đếm ngược mỗi giây (1000ms)
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    countDown--; // Giảm đi 1 mỗi giây

    // Khi đếm qua 0, reset lại 15 và đảo chiều giao thông
    if (countDown < 0) {
      countDown = 15;
      isNSRed = !isNSRed; 
    }
  }

  // 2. Logic Đèn Giao Thông
  if (isNSRed) {
    // Chiều Nam - Bắc: Đèn Đỏ giữ nguyên toàn thời gian (15s)
    digitalWrite(NS_R, HIGH); digitalWrite(NS_Y, LOW); digitalWrite(NS_G, LOW);
    
    // Chiều Đông - Tây: Xanh (15 -> 6), Vàng (5 -> 0)
    if (countDown > 5) {
      digitalWrite(EW_R, LOW); digitalWrite(EW_Y, LOW); digitalWrite(EW_G, HIGH);
    } else {
      digitalWrite(EW_R, LOW); digitalWrite(EW_Y, HIGH); digitalWrite(EW_G, LOW);
    }
  } else {
    // Chiều Đông - Tây: Đèn Đỏ giữ nguyên toàn thời gian (15s)
    digitalWrite(EW_R, HIGH); digitalWrite(EW_Y, LOW); digitalWrite(EW_G, LOW);
    
    // Chiều Nam - Bắc: Xanh (15 -> 6), Vàng (5 -> 0)
    if (countDown > 5) {
      digitalWrite(NS_R, LOW); digitalWrite(NS_Y, LOW); digitalWrite(NS_G, HIGH);
    } else {
      digitalWrite(NS_R, LOW); digitalWrite(NS_Y, HIGH); digitalWrite(NS_G, LOW);
    }
  }

  // 3. Quét hiển thị LED 7 thanh liên tục
  hienThiSo(countDown);
}

// --- CÁC HÀM PHỤ TRỢ ---

void hienThiSo(int num) {
  int nghin = num / 1000;
  int tram = (num / 100) % 10;
  int chuc = (num / 10) % 10;
  int donvi = num % 10;

  // Hiển thị hàng nghìn
  tatCacDigit();
  xuatSegment(numCode[nghin]);
  digitalWrite(digitThousand, HIGH);
  delayMicroseconds(3000);

  // Hiển thị hàng trăm
  tatCacDigit();
  xuatSegment(numCode[tram]);
  digitalWrite(digitHundred, HIGH);
  delayMicroseconds(3000);

  // Hiển thị hàng chục
  tatCacDigit();
  xuatSegment(numCode[chuc]);
  digitalWrite(digitTen, HIGH); 
  delayMicroseconds(3000);

  // Hiển thị hàng đơn vị
  tatCacDigit();
  xuatSegment(numCode[donvi]);
  digitalWrite(digitUnit, HIGH);  
  delayMicroseconds(3000);
}

void xuatSegment(byte maHex) {
  // Duyệt qua 7 bit để xuất ra các chân a, b, c, d, e, f, g
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], bitRead(maHex, i));
  }
}

void tatCacDigit() {
  // Tắt tất cả các chân điều khiển vị trí để chống lem màu 
  digitalWrite(digitUnit, LOW);
  digitalWrite(digitTen, LOW);
  digitalWrite(digitHundred, LOW);
  digitalWrite(digitThousand, LOW);
}