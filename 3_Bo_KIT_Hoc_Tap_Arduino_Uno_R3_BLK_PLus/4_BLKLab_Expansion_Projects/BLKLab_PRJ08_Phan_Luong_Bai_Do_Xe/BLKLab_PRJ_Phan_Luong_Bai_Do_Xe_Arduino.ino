/**
 * =========================================================================================
 * * SƠ ĐỒ ĐẤU NỐI PHẦN CỨNG:
 * * 1. MÀN HÌNH LCD I2C (16x2)
 * - VCC  -> 5V
 * - GND  -> GND
 * - SDA  -> A4 (hoặc chân SDA trên Arduino)
 * - SCL  -> A5 (hoặc chân SCL trên Arduino)
 * * 2. CẢM BIẾN SIÊU ÂM 1 (BÊN TRÁI / CẢM BIẾN 1)
 * - VCC  -> 5V
 * - GND  -> GND
 * - TRIG -> Chân Digital 7
 * - ECHO -> Chân Digital 6
 * * 3. CẢM BIẾN SIÊU ÂM 2 (BÊN PHẢI / CẢM BIẾN 2)
 * - VCC  -> 5V
 * - GND  -> GND
 * - TRIG -> Chân Digital 5
 * - ECHO -> Chân Digital 4
 * * 4. VÒNG LED NEOPIXEL (12 LED)
 * - VCC  -> 5V
 * - GND  -> GND
 * - DIN  -> Chân Digital 3
 * =========================================================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

// -------------------------------------------------------------------------
// 1. CẤU HÌNH CHÂN TÍN HIỆU VÀ ĐỐI TƯỢNG PHẦN CỨNG
// -------------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int TRIG_1 = 7; 
const int ECHO_1 = 6;
const int TRIG_2 = 5; 
const int ECHO_2 = 4;
const int NUM_READINGS = 5; // Số lần đọc mẫu để lọc nhiễu cảm biến

#define PIN_NEO 3           
#define NUM_LEDS 12         
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_NEO, NEO_GRB + NEO_KHZ800);

// -------------------------------------------------------------------------
// 2. THÔNG SỐ VÀ NGƯỠNG PHÂN LOẠI
// -------------------------------------------------------------------------
const float KHOANG_CACH_2_CB = 20.0; // Khoảng cách vật lý giữa 2 cảm biến (cm)
const float MOC_PHAT_HIEN_XE = 10.0; // Khoảng cách tối đa để xác nhận có xe (cm)
const float RONG_MAX_XE_MAY  = 5.0;  // Ngưỡng chiều rộng tối đa của xe máy (cm)

// -------------------------------------------------------------------------
// 3. CÁC BIẾN QUẢN LÝ TRẠNG THÁI HỆ THỐNG
// -------------------------------------------------------------------------
// Các trạng thái: 0 = Đang quét, 1 = Xe máy, 2 = Ô tô, 3 = Chờ (Không có xe)
int trangThaiXe = 3; 
int trangThaiCu = -1; 

bool xeTonTai = false;                 
unsigned long thoiGianVao = 0;         
unsigned long thoiGianMatTienHieu = 0; 
float maxChieuRong = 0.0;              

const unsigned long TG_DANG_QUET     = 1000; // Thời gian duy trì trạng thái quét (ms)
const unsigned long TG_CHONG_NHIEU_DI= 1500; // Độ trễ xác nhận xe đã rời đi (ms)

// -------------------------------------------------------------------------
// 4. BIẾN ĐIỀU KHIỂN ĐA NHIỆM CHO HIỆU ỨNG LED
// -------------------------------------------------------------------------
// Hiệu ứng 1: Quét nhanh khi phát hiện xe
int spinPos = 0; 
unsigned long thoiGianChayLed = 0;
const unsigned long TOC_DO_LED = 12; // ms/frame

// Hiệu ứng 2: Chế độ chờ (Radar siêu chậm)
unsigned long thoiGianRadar = 0;
int headPos = 0; 
const unsigned long TOC_DO_RADAR = 220; // ms/frame

// Cấu hình dải màu mờ dần cho hiệu ứng đuôi Radar
const uint8_t brightTail[12] = {255, 140, 60, 20, 5, 0, 0, 0, 0, 0, 0, 0};

// -------------------------------------------------------------------------
// CHƯƠNG TRÌNH KHỞI TẠO (SETUP)
// -------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  // Khởi tạo chân tín hiệu cảm biến
  pinMode(TRIG_1, OUTPUT); pinMode(ECHO_1, INPUT);
  pinMode(TRIG_2, OUTPUT); pinMode(ECHO_2, INPUT);

  // Khởi tạo vòng LED
  strip.begin();
  strip.setBrightness(150); 
  strip.show();             

  // Khởi tạo màn hình LCD và hiển thị thông điệp khởi động
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("HT PHAN LUONGBLK"); 
  lcd.setCursor(0, 1); lcd.print("   XIN CHAO!    ");
  delay(2000);
  lcd.clear();
}

// -------------------------------------------------------------------------
// CHƯƠNG TRÌNH CHÍNH (LOOP)
// -------------------------------------------------------------------------
void loop() {
  // Lấy dữ liệu đã được lọc nhiễu từ 2 cảm biến
  float dist1 = getFilteredDistance(TRIG_1, ECHO_1);
  float dist2 = getFilteredDistance(TRIG_2, ECHO_2);
  
  // Xác định xem có xe đi vào vùng nhận diện hay không
  bool coXeHienTai = (dist1 < MOC_PHAT_HIEN_XE || dist2 < MOC_PHAT_HIEN_XE);

  // BƯỚC 1: XỬ LÝ LƯU TRỮ VÀ TÍNH TOÁN KÍCH THƯỚC XE
  if (coXeHienTai) {
    thoiGianMatTienHieu = millis(); 
    
    // Đánh dấu mốc thời gian khi xe mới bắt đầu tiến vào
    if (!xeTonTai) {
      xeTonTai = true;
      thoiGianVao = millis();
      maxChieuRong = 0.0; 
    }
    
    // Chuẩn hóa khoảng cách (loại bỏ các giá trị nhiễu vượt quá khoảng cách 2 cảm biến)
    float valid_d1 = (dist1 > KHOANG_CACH_2_CB) ? 10.0 : dist1;
    float valid_d2 = (dist2 > KHOANG_CACH_2_CB) ? 10.0 : dist2;
    
    // Tính toán chiều rộng của phương tiện
    float chieuRongVat = KHOANG_CACH_2_CB - (valid_d1 + valid_d2);
    
    // Cập nhật giá trị chiều rộng lớn nhất ghi nhận được
    if (chieuRongVat > maxChieuRong) {
      maxChieuRong = chieuRongVat;
    }
  }

  // BƯỚC 2: CẬP NHẬT TRẠNG THÁI HỆ THỐNG DỰA TRÊN DỮ LIỆU
  if (xeTonTai) {
    // Nếu mất tín hiệu đủ lâu, xác nhận xe đã rời đi
    if (!coXeHienTai && (millis() - thoiGianMatTienHieu >= TG_CHONG_NHIEU_DI)) {
      xeTonTai = false; 
      trangThaiXe = 3;  
    } 
    else {
      // Đang trong thời gian quét để lấy mẫu
      if (millis() - thoiGianVao < TG_DANG_QUET) {
        trangThaiXe = 0; 
      } 
      // Kết thúc quét, tiến hành phân loại phương tiện
      else {
        if (maxChieuRong > RONG_MAX_XE_MAY) {
          trangThaiXe = 2; // Phân loại: Ô tô
        } else {
          trangThaiXe = 1; // Phân loại: Xe máy
        }
      }
    }
  } else {
    trangThaiXe = 3; // Trạng thái chờ
  }

  // BƯỚC 3: XỬ LÝ ĐA NHIỆM CHO CÁC HIỆU ỨNG LED NEOPIXEL
  if (trangThaiXe == 0) {
    // Hiệu ứng: Sao chổi vàng quay nhanh (Đang quét xe)
    if (millis() - thoiGianChayLed >= TOC_DO_LED) {
      thoiGianChayLed = millis(); 
      
      strip.clear();
      strip.setPixelColor(spinPos, strip.Color(255, 180, 0));             
      strip.setPixelColor((spinPos + 11) % 12, strip.Color(120, 80, 0));   
      strip.setPixelColor((spinPos + 10) % 12, strip.Color(40, 25, 0));    
      strip.setPixelColor((spinPos + 9) % 12, strip.Color(10, 3, 0));      
      strip.show();
      
      spinPos = (spinPos + 1) % 12; 
    }
  }
  else if (trangThaiXe == 3) {
    // Hiệu ứng: Radar màu lục lam quay chậm (Đang chờ)
    if (millis() - thoiGianRadar >= TOC_DO_RADAR) { 
      thoiGianRadar = millis();
      
      strip.clear();
      for (int i = 0; i < 12; i++) {
        int indexFromHead1 = (headPos - i + 12) % 12;
        int indexFromHead2 = ((headPos + 6) - i + 12) % 12;
        
        uint8_t currentBright = brightTail[indexFromHead1];
        if (brightTail[indexFromHead2] > currentBright) {
          currentBright = brightTail[indexFromHead2];
        }
        
        if (currentBright > 0) {
          uint8_t g_val = (100 * currentBright) / 255;
          uint8_t b_val = (255 * currentBright) / 255;
          strip.setPixelColor(i, strip.Color(0, g_val, b_val));
        }
      }
      
      strip.show();
      headPos = (headPos + 1) % 12; 
    }
  }

  // BƯỚC 4: XUẤT THÔNG TIN LÊN LCD VÀ CHỐT MÀU LED DỰA TRÊN PHÂN LOẠI
  if (trangThaiXe != trangThaiCu) {
    lcd.clear(); 
    
    switch (trangThaiXe) {
      case 0: // Đang quét
        lcd.setCursor(0, 0); lcd.print(" DANG QUET XE...");
        lcd.setCursor(0, 1); lcd.print("  Vui long cho  ");
        break;
        
      case 1: // Xác nhận Xe máy
        lcd.setCursor(0, 0); lcd.print("<-- XE MAY");
        lcd.setCursor(0, 1); printCentered("Moi re TRAI");
        setSolidColor(strip.Color(0, 255, 0));   // LED xanh lá
        break;
        
      case 2: // Xác nhận Ô tô
        lcd.setCursor(0, 0); lcd.print("===== O TO =====");
        lcd.setCursor(2, 1); lcd.print("Moi re PHAI ->");
        setSolidColor(strip.Color(230, 0, 255)); // LED tím hồng
        break;
        
      case 3: // Chờ xe
        lcd.setCursor(0, 0); lcd.print("HT PHAN LUONG XE"); 
        lcd.setCursor(0, 1); printCentered("Dang cho xe... ");
        break;
    }
    
    // Cập nhật trạng thái để tránh việc xóa/vẽ lại LCD liên tục
    trangThaiCu = trangThaiXe; 
  }
}

// -------------------------------------------------------------------------
// CÁC HÀM TIỆN ÍCH / PHỤ TRỢ DỮ LIỆU
// -------------------------------------------------------------------------

/**
 * Hiển thị một màu đồng nhất trên toàn bộ vòng LED NeoPixel.
 * @param color Mã màu dạng dải 32-bit (RGB)
 */
void setSolidColor(uint32_t color) {
  for(int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

/**
 * Căn giữa chuỗi văn bản trên một dòng của màn hình LCD (16 ký tự).
 * @param text Chuỗi ký tự cần căn giữa
 */
void printCentered(const char* text) {
  int len = strlen(text);
  int spaces = (16 - len) / 2;
  for (int i = 0; i < spaces; i++) lcd.print(" ");
  lcd.print(text);
}

/**
 * Lấy khoảng cách từ cảm biến siêu âm với cơ chế lấy trung bình nhiều mẫu để lọc nhiễu.
 * @param trigPin Chân phát xung
 * @param echoPin Chân nhận xung
 * @return Khoảng cách đo được tính bằng cm
 */
float getFilteredDistance(int trigPin, int echoPin) {
  float sum = 0;
  int validReadings = 0;
  
  for (int i = 0; i < NUM_READINGS; i++) {
    // Phát xung siêu âm
    digitalWrite(trigPin, LOW); delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Tính toán thời gian phản hồi (timeout 25000 tương đương ~400cm)
    long duration = pulseIn(echoPin, HIGH, 25000); 
    
    if (duration > 0) {
      sum += (duration * 0.0343 / 2); // Quy đổi thời gian ra cm
      validReadings++;
    }
    delay(4); // Độ trễ ngắn giữa các lần đọc mẫu
  }
  
  // Trả về giá trị phi thực tế nếu không có phản hồi hợp lệ (lỗi cảm biến hoặc xa ngoài tầm)
  if (validReadings == 0) return 999.0; 
  
  return sum / validReadings;
}