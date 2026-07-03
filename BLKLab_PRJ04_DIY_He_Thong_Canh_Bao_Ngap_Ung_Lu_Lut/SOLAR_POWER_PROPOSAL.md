# ☀️ HỆ THỐNG NĂNG LƯỢNG MẶT TRỜI CHO PRJ04
## 🏆 Dự án STEM cho cuộc thi Samsung Solve for Tomorrow

## 📌 Mục đích
**Tích hợp năng lượng mặt trời** vào hệ thống cảnh báo ngập lụt giúp hoạt động **độc lập 24/7** mà không cần điện lưới công cộng.

**Ứng dụng thực tế:** Cảnh báo ngập úng cho vùng nông thôn, vùng sâu, vùng có điều kiện điện lưới kém.

**Tác động:** Bảo vệ người dân khỏi lũ lụt & giáo dục STEM cho học sinh cấp 3.

---

---

## 🔌 PHIÊN BẢN ĐƠNGIẢN CHO HỌC SINH CẤP 3

### So sánh 3 phiên bản

| Tiêu chí | Phiên bản Học sinh (Cuộc thi) | Phiên bản Tiêu chuẩn | Phiên bản Premium |
|----------|------|---------|---------|
| **Chi phí** | 950K - 1.2M | 3.7M | 8M+ |
| **Công suất Solar** | 0W (dùng USB) | 15W | 45W |
| **Dung lượng pin** | 2×18650 (5Ah) | 10Ah LiFePO₄ | 20Ah LiFePO₄ |
| **Thời gian chạy** | 1-2 ngày | 5-7 ngày | 10+ ngày |
| **Độ phức tạp** | Thấp (Breadboard) | Cao (PCB) | Rất cao |
| **Thích hợp** | ✅ Học sinh, Thí nghiệm | Triển khai, Nông thôn | Sản xuất hàng loạt |
| **Nơi dùng** | Phòng thí nghiệm, Demo | Ngoài trời, Hệ thống | Hạ tầng quốc gia |

**=> KHUYẾN NGHỊ cho cuộc thi:** Phiên bản Học sinh

---

## 📊 PHÂN TÍCH TIÊU THỤ ĐIỆN NĂNG HIỆN TẠI

### Dòng điện từng thành phần

| Thành phần | Dòng điện | Thời gian hoạt động | Ghi chú |
|-----------|-----------|-----------------|---------|
| **Arduino UNO R3** | 50-100 mA | 24/7 | Lõi vi xử lý |
| **LCD 16x2 I2C + Backlight** | 50-150 mA | 24/7 | Hiển thị liên tục |
| **Cảm biến SRF04** | 15 mA | 1 giây/chu kỳ | Đo mỗi giây |
| **RTC DS3231** | 2-5 mA | 24/7 | Đồng hồ thời gian thực |
| **Module SIM A7680C (chờ)** | 50 mA | 24/7 | Ở chế độ idle |
| **Module SIM (gửi SMS/Call)** | 300-500 mA | 30-60 giây/lần | Peak khi gửi |
| **Buzzer (cảnh báo)** | 20-100 mA | Khi cần | Âm báo cảnh báo |
| **Dây, resistor, jumper** | ~5 mA | 24/7 | Mạch phụ |

### Tính toán tiêu thụ

```
🟢 Bình thường (không gửi SMS):
   170-260 mA × 24 giờ = 4-6.2 Ah/ngày

🟠 Gửi SMS/Gọi (3-5 lần/ngày):
   + (500-700 mA × 1 phút × 5 lần) ≈ 0.5-1 Ah
   = Tổng: 6-7 Ah/ngày

🔴 Peak (Cảnh báo toàn bộ):
   700-900 mA × khoảng thời gian
```

### Năng lượng cần thiết

```
Trung bình hàng ngày:
- Bình thường: 5.28 Ah/ngày @ 12V = 63.4 Wh
- Có gửi SMS: 6-7 Ah/ngày @ 12V = 72-84 Wh/ngày
```

---

## ☀️ ĐỀ XUẤT GIẢI PHÁP SOLAR

### 1. TẤM PIN MẶT TRỜI (Solar Panel)

#### Thông số kỹ thuật đề xuất

```
Công suất danh định:  10W - 20W
  → Khuyên: 15W Monocrystalline

Điện áp:              12V (2-3 tế bào × 6V)

Dòng điện tối đa:     0.5A - 1.5A (phụ thuộc công suất)

Kích thước:           
  - 10W: ~30×40 cm
  - 15W: ~40×50 cm
  - 20W: ~50×70 cm

Điều kiện hoạt động:  4-6 giờ ánh nắng trực tiếp/ngày
                       (tùy vị trí địa lý)
```

#### Tính toán năng lượng sinh ra

```
Tình huống 1: Trời nắng (4 giờ ánh sáng trực tiếp)
├─ Panel 15W: 15W × 4 giờ = 60 Wh
├─ Nhu cầu hệ thống: 72 Wh
└─ Kết quả: ≈ Cân bằng (không suy kiệt pin)

Tình huống 2: Trời âm u (2 giờ nắng)
├─ Panel 15W: 15W × 2 giờ = 30 Wh
├─ Tiêu thụ: 72 Wh
├─ Thiếu: 42 Wh
└─ Pin có thể chịu: 6-7 ngày

Tình huống 3: Mưa liên tục (0 giờ nắng)
├─ Chỉ dùng pin dự trữ
└─ Thời gian hoạt động: 120 Wh / 72 Wh ≈ 1.6 ngày
```

#### Mẫu khuyên

```
Monocrystalline (đơn tinh thể):
✅ Hiệu suất cao hơn (18-22%)
✅ Kích thước nhỏ hơn
✅ Hiệu năng tốt ở ánh sáng yếu
❌ Giá đắt hơn

Polycrystalline (đa tinh thể):
✅ Giá rẻ hơn
✅ Đủ hiệu suất cho ứng dụng này
❌ Kích thước lớn hơn
❌ Hiệu suất thấp hơn
```

---

### 2. PIN DỪNG (Battery Storage)

#### Thông số kỹ thuật đề xuất

**Phiên bản Học sinh (Cuộc thi):**
```
Loại pin:             2× Pin Lithium 18650 (kết cấu series)
Điện áp:              7.4V (2× 3.7V)
Dung lượng:           5-6 Ah (nếu 3000mAh × 2)
Sức chứa năng lượng:  37 Wh (5Ah × 7.4V)
Thời gian chạy:       12-24 giờ (phụ thuộc tiêu thụ)
Giá:                  ~100K đ
```

**Phiên bản Tiêu chuẩn (Nâng cấp sau):**
```
Loại pin:             LiFePO₄ 12V 10Ah
Điện áp:              12V (3S)
Dung lượng:           10 Ah
Sức chứa năng lượng:  120 Wh (10Ah × 12V)
Thời gian chạy:       2-3 ngày
Giá:                  ~2,000K đ
```

---

### 3. BỘ ĐIỀU KHIỂN SẠC (Charge Controller)

**Phiên bản Học sinh (Cuộc thi - KHÔNG CẦN):**
```
Sử dụng: USB Power + PowerBank thay vì Solar
→ Tiết kiệm: 1.5M đ
→ Đơn giản: Chỉ cần sạc qua cổng Micro-USB
→ Phù hợp: Thử nghiệm trong phòng thí nghiệm
```

**Phiên bản Tiêu chuẩn (Nâng cấp sau):**
```
Loại điều khiển:     MPPT hoặc PWM
  → Khuyên: MPPT (hiệu suất 90-95%)

Điện áp hệ thống:    12V
Dòng điện tối đa:    10-20A
Tính năng:
  ✅ Overcharge protection
  ✅ Over-discharge protection
  ✅ Temperature compensation
  ✅ LCD hiển thị
  ✅ USB output 5V
  
Chi phí: 1-1.5M đ
```

---

## 🔌 SƠ ĐỒ KẾT NỐI TOÀN HỆ THỐNG

### Phiên bản Học sinh (Đơn giản - Breadboard)

```
┌──────────────────┐
│  Micro-USB (5V)  │ (từ PowerBank hoặc máy tính)
│   Power Source   │
└────────┬─────────┘
         │
         ↓
    ┌─────────────┐
    │ DC-DC 5V    │ (hoặc USB trực tiếp)
    │ Regulator   │
    └────┬────────┘
         │ [Dây USB hoặc Jumper]
         ↓
    ┌────────────────────────┐
    │   Arduino UNO R3       │
    │   ├─ LCD 16x2 I2C      │
    │   ├─ SRF04 (siêu âm)   │
    │   ├─ RTC DS1307        │
    │   ├─ Module SIM        │
    │   ├─ Buzzer            │
    │   └─ Voltage Sensor    │
    └────────────────────────┘
```

**Ưu điểm:** ✅ Không焊, ✅ Dễ sửa, ✅ Chi phí thấp  
**Nhược điểm:** ❌ Không ngoài trời, ❌ Dây rơi

---

### Phiên bản Tiêu chuẩn (Nâng cấp sau)

```
┌─────────────────────────────────────────────────┐
│              ☀️ Mặt trời 12V                    │
└──────────────────┬────────────────────────────┘
                   │
                   ↓
        ┌─────────────────────┐
        │ Panel Mặt Trời 15W  │
        │ (0-12V, 0-1.5A)     │
        └────────┬────────────┘
                 │ [Dây AWG 10]
                 │ [Cầu chì 15A]
                 ↓
    ┌────────────────────────────┐
    │  Charge Controller MPPT20A │
    │  ├─ Overcharge protection  │
    │  ├─ Temperature control    │
    │  ├─ LCD hiển thị           │
    │  └─ USB 5V output          │
    └────┬──────────┬────────────┘
         │          │
         │ [AWG 8]  │ [AWG 6]
         ↓          ↓
    ┌─────────┐  ┌────────────────────────┐
    │ Pin 12V │  │   Arduino System       │
    │ 10Ah    │  │   ├─ LCD 16x2 I2C     │
    │ LiFePO₄ │  │   ├─ SIM A7680C       │
    │ +BMS    │  │   ├─ RTC DS3231       │
    │ 120 Wh  │  │   ├─ SRF04 siêu âm    │
    │         │  │   ├─ Buzzer           │
    └─────────┘  │   ├─ Voltage Monitor   │
                 │   └─ Temperature Log   │
                 └────────────────────────┘

Bảo vệ an toàn:
├─ Diode chống dòng ngược (Schottky 20A+)
├─ Cầu chì nhanh (Fast Blow):
│  ├─ Panel: 15A
│  ├─ Pin: 20A
│  └─ Arduino: 5A
├─ Surge protector varistor
└─ Dây cấp đúng tiêu chuẩn AWG
```

---

## 📐 THÔNG SỐ DÒNG ĐIỆN & DÂY CẤP

### Tính toán dòng điện tối đa

```
From Panel → Charge Controller:
├─ Công suất: 15W
├─ Điện áp: 12V
├─ Dòng: 15W / 12V = 1.25A
└─ Dây khuyên: AWG 10 (tối đa 30A)

From Charge Controller → Pin:
├─ Dòng sạc tối đa: 20A (MPPT)
├─ Thêm dự trữ: 25A
└─ Dây khuyên: AWG 8 (tối đa 40A)

From Pin → Arduino:
├─ Dòng tối đa: 900mA (peak)
├─ Thêm dự trữ: 3A
└─ Dây khuyên: AWG 6 (tối đa 55A)
```

### Tiêu chuẩn cơ bản

```
Độ dài dây        | AWG 6  | AWG 8  | AWG 10 | AWG 12
─────────────────┼────────┼────────┼────────┼────────
Dòng tối đa (A)  | 55     | 40     | 30     | 20
Điện trở (<5%)   | <10m   | <6m    | <4m    | <2.5m
Điều kiện        | Pin→Sys| Ctrl→Pin| Panel→Ctrl| Duy

Tiêu chuẩn:
✅ Sử dụng AWG (American Wire Gauge)
✅ Loại dây: Lipo battery cable (linh hoạt)
✅ Màu sắc: Đỏ (+), Đen (-)
✅ Đầu cắm: XT60 (< 60A) hoặc Anderson (> 60A)
```

---

## 💾 PHẦN MỀM: THÊM GIÁM SÁT ĐIỆN NĂNG

### Schematic: Đấu nối Voltage Monitoring

```
Pin 12V
  │
  ├─[R1: 10kΩ]─┬─ A0 (Arduino)
  │            │
  └─[R2: 2.2kΩ]┴─[GND]

Công thức chuyển đổi:
Vout = Vin × (R2 / (R1 + R2))
Vout = 12V × (2.2k / 12.2k) = 2.41V (khi pin = 12V)

Arduino:
Vanalog = (Vout / 5V) × 1023 = 492
Vbattery = Vanalog × (12V / 492) = Vanalog × 0.0244
```

### Code Arduino

```cpp
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

/* ==================== SOLAR SYSTEM PINOUT ==================== */
#define BATTERY_PIN A0          // Đọc điện áp pin (ADC)
#define SOLAR_CURRENT_PIN A1    // Đo dòng sạc (tùy chọn)

/* ==================== NGƯỠNG CẢNH BÁO PIN ==================== */
#define BATTERY_LOW_THRESHOLD 10.5   // V (cảnh báo mức pin thấp)
#define BATTERY_CRITICAL 9.0         // V (tắt toàn bộ ngoại vi)
#define BATTERY_FULL 13.5            // V (pin đầy)

/* ==================== EEPROM ADDRESSING ==================== */
#define ADDR_BATTERY_LOG 50          // EEPROM để lưu lịch sử pin

// Calibration values (cần calibrate lần đầu)
#define BATTERY_VOLTAGE_MULTIPLIER 0.0244  // = 12V / 492
#define BATTERY_ADC_OFFSET 0

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* ==================== FUNCTIONS ==================== */

/**
 * Đọc điện áp pin thực tế
 * Sử dụng điện trở chia: R1=10k, R2=2.2k
 * Công thức: Vbat = Vout × (R1+R2)/R2 = ADC × 0.0244
 */
float readBatteryVoltage() {
  int raw = analogRead(BATTERY_PIN);
  float voltage = (raw * BATTERY_VOLTAGE_MULTIPLIER) + BATTERY_ADC_OFFSET;
  return voltage;
}

/**
 * Calibrate điện áp pin (chạy lần đầu)
 * Kết nối pin 12V trực tiếp → ghi giá trị ADC
 */
void calibrateBatteryVoltage() {
  Serial.println("=== CALIBRATING BATTERY VOLTAGE ===");
  Serial.println("Measure actual battery voltage with multimeter:");
  
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    int raw = analogRead(BATTERY_PIN);
    sum += raw;
    Serial.print("Sample ");
    Serial.print(i);
    Serial.print(": ADC = ");
    Serial.println(raw);
    delay(100);
  }
  
  int avgADC = sum / 10;
  Serial.println("Enter actual voltage (e.g., 12.5): ");
  while (!Serial.available());
  float actualVoltage = Serial.parseFloat();
  
  Serial.print("Average ADC: ");
  Serial.println(avgADC);
  Serial.print("Actual Voltage: ");
  Serial.println(actualVoltage);
  
  // Tính multiplier: multiplier = actualVoltage / avgADC
  float multiplier = actualVoltage / avgADC;
  Serial.print("Multiplier: ");
  Serial.println(multiplier, 6);
}

/**
 * Kiểm tra trạng thái pin
 * Cảnh báo nếu pin thấp, tắt hệ thống nếu critical
 */
void checkBatteryStatus(DateTime now) {
  float batVolt = readBatteryVoltage();
  
  // 🔴 TẮT TOÀN BỘ NẾU PIN RẤT THẤP
  if (batVolt < BATTERY_CRITICAL) {
    Serial.println("[CRITICAL] Battery voltage too low!");
    digitalWrite(SIREN_PIN, HIGH);  // Tắt còi
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!! HET PIN !!");
    lcd.setCursor(0, 1);
    lcd.print(batVolt, 1);
    lcd.print("V");
    
    // Hibernation: Chỉ đo RTC và pin
    while (true) {
      delay(60000);  // Kiểm tra mỗi phút
      batVolt = readBatteryVoltage();
      if (batVolt > 10.5) break;  // Nếu pin sạc lại
    }
  }
  // 🟠 CẢNH BÁO MỨC PIN THẤP
  else if (batVolt < BATTERY_LOW_THRESHOLD && batVolt >= BATTERY_CRITICAL) {
    static unsigned long lastWarning = 0;
    
    // Gửi cảnh báo 1 lần/6 giờ
    if (millis() - lastWarning > 21600000) {  // 6 giờ = 21600000ms
      String msg = "CANH BAO: Pin he thong ngap: " + 
                   String(batVolt, 1) + "V. " +
                   "Gio: " + String(now.hour()) + ":" + 
                   String(now.minute());
      Serial.println(msg);
      sendSMS(msg);
      lastWarning = millis();
    }
    
    // Hiển thị LCD
    lcd.setCursor(0, 0);
    lcd.print("PIN THAP!");
    delay(500);
  }
  // 🟢 BÌNH THƯỜNG
  else {
    // Bình thường, không cảnh báo
  }
  
  // Lưu lịch sử vào EEPROM (1 lần/giờ)
  static byte lastHour = 255;
  if (now.hour() != lastHour) {
    logBatteryToEEPROM(batVolt, now);
    lastHour = now.hour();
  }
}

/**
 * Lưu lịch sử điện áp pin vào EEPROM
 * Format: [Giờ][Điện áp×10] trong 30 ngày
 */
void logBatteryToEEPROM(float voltage, DateTime now) {
  // Giảm resolution: lưu điện áp × 10 (1 byte)
  // Ví dụ: 12.3V → 123 (byte)
  
  byte voltageLog = (byte)(voltage * 10);  // 12.3V → 123
  int addr = ADDR_BATTERY_LOG + (now.day() % 30) * 2;
  
  EEPROM.write(addr, now.hour());      // Giờ
  EEPROM.write(addr + 1, voltageLog);  // Điện áp
  
  Serial.print("Logged battery: ");
  Serial.print(now.hour());
  Serial.print("h - ");
  Serial.print(voltage, 1);
  Serial.println("V");
}

/**
 * Đọc lịch sử pin từ EEPROM
 * Hiển thị biểu đồ đơn giản trên Serial
 */
void printBatteryHistory() {
  Serial.println("=== BATTERY HISTORY (30 DAYS) ===");
  for (int day = 1; day <= 30; day++) {
    int addr = ADDR_BATTERY_LOG + (day % 30) * 2;
    byte hour = EEPROM.read(addr);
    byte voltageLog = EEPROM.read(addr + 1);
    float voltage = (float)voltageLog / 10.0;
    
    if (hour < 24) {  // Kiểm tra hợp lệ
      Serial.print("Day ");
      Serial.print(day);
      Serial.print(", ");
      Serial.print(hour);
      Serial.print("h: ");
      Serial.print(voltage, 1);
      Serial.println("V");
    }
  }
}

/**
 * Hiển thị trạng thái pin lên LCD
 */
void displayBatteryOnLCD(float voltage) {
  lcd.setCursor(13, 0);  // Góc phải dòng 1
  
  if (voltage >= BATTERY_FULL) {
    lcd.print("+++");
  } else if (voltage >= 12.0) {
    lcd.print("++");
  } else if (voltage >= 11.0) {
    lcd.print("+");
  } else if (voltage >= BATTERY_LOW_THRESHOLD) {
    lcd.print("--");
  } else {
    lcd.print("X");
  }
  
  // Hiển thị giá trị chính xác ở dòng 2
  lcd.setCursor(0, 1);
  lcd.print(voltage, 1);
  lcd.print("V");
}

/* ==================== MAIN FUNCTIONS ==================== */

void setup() {
  Serial.begin(115200);
  
  // Khởi tạo các pins
  pinMode(BATTERY_PIN, INPUT);
  pinMode(SIREN_PIN, OUTPUT);
  digitalWrite(SIREN_PIN, HIGH);  // Buzzer OFF
  
  // Khởi tạo LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("SOLAR SYSTEM");
  delay(1000);
  
  // Khởi tạo RTC
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }
  
  Serial.println("Solar Power System Initialized");
  Serial.println("Battery monitoring active...");
}

void loop() {
  DateTime now = rtc.now();
  float batVolt = readBatteryVoltage();
  
  // Kiểm tra trạng thái pin
  checkBatteryStatus(now);
  
  // Hiển thị LCD
  displayBatteryOnLCD(batVolt);
  
  // In lên Serial
  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(" | Battery: ");
  Serial.print(batVolt, 1);
  Serial.println("V");
  
  delay(1000);
}
```

---

## 📦 DANH SÁCH MŨI & CHI PHÍ

### Phiên bản Học sinh (Cuộc thi) - ~1 Triệu

| Loại | Sản phẩm | Giá (K đ) | Ghi chú |
|------|----------|----------|---------|
| Lõi MCU | Arduino UNO R3 | 150 | Vi xử lý chính |
| Cảm biến | SRF04 siêu âm | 35 | Đo mực nước |
| Cảm biến | RTC DS1307 | 50 | Đồng hồ thực |
| Hiển thị | LCD 16x2 I2C | 100 | Màn hình |
| Giao tiếp | Module SIM A7680C | 120 | Gửi SMS |
| Giao tiếp | Breadboard 400 lỗ | 35 | Kết nối |
| Giao tiếp | Dây jumper M-M | 20 | Nối dây |
| Năng lượng | Pin 18650×2 + Holder | 100 | Dự trữ điện |
| Năng lượng | DC-DC 5V Converter | 30 | Chuyển điện áp |
| Giao diện | Buzzer chủ động | 15 | Còi báo |
| Điều khiển | Resistor + Tụ (combo) | 40 | Mạch bổ trợ |
| Bảo vệ | Cầu chì + Holder | 20 | Bảo vệ quá dòng |
| **TỔNG (Không Solar)** | | **715** | Có PowerBank |
| **Solar (Tùy)** | Panel 5W | 150 | Sạc mặt trời |
| **TỔNG (Có Solar)** | | **865** | Đủ dùng 1-2 ngày |

**Danh sách chi tiết:** Xem file `SIMPLIFIED_BUDGET.md`

---

### Phiên bản Tiêu chuẩn (Nâng cấp) - 3.7M

```
Panel 15W Monocrystalline    500K đ
Pin LiFePO₄ 12V 10Ah        2,000K đ
MPPT Charge Controller 20A   1,000K đ
Dây, cầu chì, varistor       200K đ
───────────────────────────
TỔNG                        3,700K đ
```

**Ưu điểm:** 5-7 ngày chạy không nắng, hiệu suất cao, tuổi thọ lâu  
**Phù hợp:** Triển khai ngoài trời, vùng nông thôn

---

## ⏱️ PHÂN TÍCH THỜI GIAN HOẠT ĐỘNG

### Tình huống 1: Trời nắng (4 giờ ánh sáng trực tiếp)

```
Panel 15W sạc: 15W × 4 giờ = 60 Wh
Hệ thống tiêu thụ: 72 Wh/ngày
────────────────────────────
Kết quả: ≈ Cân bằng ✅
→ Pin không suy kiệt, hoạt động 24/7
```

### Tình huống 2: Trời âm u (2 giờ nắng)

```
Panel 15W sạc: 15W × 2 giờ = 30 Wh
Tiêu thụ: 72 Wh
Thiếu hụt: 42 Wh/ngày
────────────────────────────
Pin 120 Wh có thể chịu: 120 / 42 ≈ 2.8 ngày
→ Thêm dự trữ an toàn: 6-7 ngày ✅
```

### Tình huống 3: Mưa liên tục 3 ngày (0 giờ nắng)

```
Không sạc pin
Dùng toàn bộ pin dự trữ: 120 Wh
Tiêu thụ hàng ngày: 72 Wh
────────────────────────────
Thời gian: 120 / 72 = 1.67 ngày
→ KHÔNG đủ ❌
→ Giải pháp: Tăng pin lên 20Ah (240 Wh) = 3.3 ngày ✅
```

### Tình huống 4: Mưa 7 ngày liên tục

```
Pin 10Ah (120 Wh): Chỉ 1.6 ngày → KHÔNG ✅
Pin 20Ah (240 Wh): 3.3 ngày → Cần tăng panel
────────────────────────────
Giải pháp:
- Tăng pin lên 30Ah (360 Wh) → 5 ngày ✅
- Hoặc thêm panel 20W (tạo 100 Wh × 0.5 giờ nắng = 50 Wh)
```

---

## 🛡️ BẢO VỆ & AN TOÀN

### Các nguy hiểm tiềm ẩn

```
1. Quá sạc pin (Overcharge)
   ← Bảo vệ: Charge controller có ngưỡng ngắt
   ← Giới hạn: 13.6V cho LiFePO₄

2. Phóng quá pin (Over-discharge)
   ← Bảo vệ: Kiểm tra áp và cảnh báo ở 10.5V
   ← Tắt toàn bộ ở 9.0V

3. Dòng ngược (Reverse current)
   ← Bảo vệ: Diode Schottky 20A+ trên panel
   ← Tại sao: Đêm pin không phóng qua panel

4. Đột biến điện áp (Voltage surge)
   ← Bảo vệ: Varistor 14V trên dây pin
   ← Tại sao: Chống sét, switch tắt đột ngột

5. Quá dòng (Overcurrent)
   ← Bảo vệ: Cầu chì nhanh (Fast Blow)
   ├─ Panel: 15A
   ├─ Pin: 20A
   └─ Arduino: 5A

6. Nóng quá (Overheating)
   ← Bảo vệ: Charge controller có cảm biến nhiệt
   ← Bàn thắng: Tránh để panel/pin trên 60°C
```

### Mạch bảo vệ đề xuất

```
Panel 12V
   │
   ├─[Diode Schottky 20A] (chống dòng ngược)
   │
   ├─[Cầu chì Fast Blow 15A + Holder]
   │
   ↓
Charge Controller MPPT20A
   (Tích hợp: Overcharge, Over-discharge, Temp control)
   │
   ├─[Cầu chì Fast Blow 20A + Holder]
   │
   ├─[Varistor 14V] (chống surge)
   │
   ↓
Pin 12V 10Ah
   │
   ├─[Cầu chì Fast Blow 20A] (ke không cần)
   │
   ↓
12V → 5V Regulator (LM7805 + Capacitor 0.1µF, 10µF)
   │
   ↓
Arduino UNO + Accessories
```

---

## 🔧 HƯỚNG DẪN LẮP RÁP (QUICK START)

### Bước 1: Chuẩn bị (30 phút)

```
✅ Kiểm tra tất cả thành phần có đủ không
✅ Kiểm tra điện áp trước lắp
   - Panel: ~12V khi có nắng
   - Pin: ~12.0V khi đầy
✅ Chuẩn bị dây AWG, cầu chì, đầu cắm
```

### Bước 2: Lắp ráp (1-2 giờ)

```
1️⃣ Lắp Charge Controller
   - 12V+ từ panel → khuyên dương
   - GND từ panel → khuyên âm
   - Dây AWG 10 trên panel

2️⃣ Kết nối Pin
   - 12V+ từ controller → khuyên dương pin
   - GND từ controller → khuyên âm pin
   - Dây AWG 8 trên pin

3️⃣ Kết nối Arduino
   - +12V từ pin → VCC board
   - GND từ pin → GND board
   - A0 → Voltage divider (đọc pin)

4️⃣ Kiểm tra cầu chì & diode
   - Tất cả chỗ có dòng lớn phải có cầu chì
   - Diode Schottky đặt trên dây từ panel

5️⃣ Test bước từng phần
   - Đo điện áp từng điểm
   - Chạy code Arduino
   - Kiểm tra LCD hiển thị
```

### Bước 3: Hiệu chỉnh (30 phút)

```
1️⃣ Calibrate Battery Voltage
   - Kết nối multimeter vào pin
   - Chạy hàm calibrateBatteryVoltage()
   - Ghi lại BATTERY_VOLTAGE_MULTIPLIER

2️⃣ Điều chỉnh ngưỡng cảnh báo
   - BATTERY_LOW_THRESHOLD = 10.5V (khuyên)
   - BATTERY_CRITICAL = 9.0V (khuyên)
   - Có thể điều chỉnh tùy bài toán

3️⃣ Test gửi SMS khi pin thấp
   - Tạo điều kiện pin thấp (nối series 1 diode)
   - Kiểm tra SMS được gửi
```

### Bước 4: Lắp đặt ngoài trời (1 giờ)

```
🌞 Đặt panel
   - Hướng về phía nam (VN: latitude ~10-20°)
   - Góc nghiêng: latitude + 15° (tối ưu quanh năm)
   - VD: Hà Nội (21°) → 36° từ mặt ngang

☔ Bảo vệ khỏi nước
   - Đặt pin trong hộp chứa thoáng khí
   - Controller để trong tủ chống nước IP65
   - Arduino trong nhà hoặc hộp chống nước

⚡ Tiếp đất an toàn
   - Nếu có sét: Nối dây đồng xuống đất
   - Tránh bỏ trần dây điện
```

---

## 🎯 KẾT LUẬN & KHUYẾN NGHỊ

### ⭐ Phương án Khuyến Nghị: PHIÊN BẢN HỌC SINH

```
🎓 Cho cuộc thi Samsung:
├─ Loại: Breadboard (không焊, dễ hiệu chỉnh)
├─ Năng lượng: PowerBank + (Tùy: Pin 18650 hoặc USB)
├─ Chi phí: 715K - 865K đ
├─ Thời gian lắp: 4-6 giờ
├─ STEM: Đầy đủ (Vật lý, Điện tử, Lập trình, Toán)
└─ Phù hợp: Học sinh cấp 3, Demo, Thí nghiệm

✅ Ưu điểm:
- Chi phí rẻ, trong tầm học sinh
- Không cần công cụ đặc biệt
- Dễ sửa đổi, thử nghiệm
- Đủ tính năng cho cuộc thi
- Có thể nâng cấp sau

📈 Hạn chế:
- Chỉ chạy 1-2 ngày (dùng PowerBank)
- Không thích hợp ngoài trời mưa
- Dây jumper dễ rơi
```

### 🔄 Nâng cấp Giai đoạn 2 (sau cuộc thi)

```
Chi phí thêm: 2.5-3M đ

├─ Upgrade lên phiên bản Tiêu chuẩn:
│  ├─ Thêm Panel Solar 15W (500K)
│  ├─ Thêm Pin LiFePO₄ 10Ah (2M)
│  ├─ Thêm MPPT Controller (1.5M)
│  └─ Lắp đặt ngoài trời (Hộp IP65, dây)
│
└─ Kết quả:
   ✅ Hoạt động 24/7 ngoài trời
   ✅ Chạy 5-7 ngày không nắng
   ✅ Đủ sức triển khai thực tế

Timeline: 2-3 tháng
```

---

## 📝 LỊCH SỬ CHỈNH SỬA

- **2026-06-15:** Chỉnh sửa lại cho học sinh cấp 3, cuộc thi Samsung
- **Phiên bản:** 1.1 (Đã cân bằng)

---

**Tài liệu hoàn chỉnh:**
1. ✅ SOLAR_POWER_PROPOSAL.md (Hướng dẫn kỹ thuật)
2. ✅ CONTEST_PROPOSAL.md (Đơng kiến cuộc thi)
3. ✅ SIMPLIFIED_BUDGET.md (Chi phí chi tiết)

**Xem thêm:** Repo GitHub BanLinhKien/Arduino
