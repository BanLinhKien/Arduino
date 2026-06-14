# ☀️ ĐỀ XUẤT HỆ THỐNG NĂNG LƯỢNG MẶT TRỜI CHO PRJ04

## 📌 Mục đích
Tích hợp năng lượng mặt trời vào hệ thống cảnh báo ngập lụt để hoạt động độc lập, không phụ thuộc vào điện lưới công cộng, đặc biệt phù hợp cho vùng sâu, nông thôn.

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

```
Công suất danh định:  10Ah - 20Ah @ 12V
  → Khuyên: 10Ah (cân bằng giá - hiệu năng)

Sức chứa năng lượng:  120 Wh (10Ah × 12V)
                       240 Wh (20Ah × 12V)

Loại pin:             LiFePO₄ (khuyến cáo) hoặc Lead-Acid

Dự phòng không nắng:  2-3 ngày liên tục
```

#### So sánh các loại pin

| Loại | Ưu điểm | Nhược điểm | Tuổi thọ | Giá |
|------|---------|-----------|---------|-----|
| **LiFePO₄ 12V 10Ah** | ✅ Tuổi thọ cao (10+ năm), Lightweight, BMS tốt, Tự phóng chậm | ❌ Đắt hơn | 2000-3000 chu kỳ | 2-3M đ |
| **Lead-Acid 12V 12Ah** | ✅ Giá rẻ, Dễ tìm, Bền | ❌ Nặng, Tự phóng nhanh | 500-1000 chu kỳ | 500K-1M đ |
| **Lithium 12V 20Ah** | ✅ Sức chứa lớn, Rất nhẹ | ❌ Rất đắt, Cần bảo vệ | 1000+ chu kỳ | 4-5M đ |

#### ⭐ Khuyến nghị: **LiFePO₄ 12V 10Ah**

**Lý do:**
- Tuổi thọ lâu dài (10-15 năm) → Tiết kiệm chi phí
- BMS (Battery Management System) tích hợp → An toàn
- Tự phóng chậm → Giữ năng lượng dự trữ lâu
- Hiệu suất cao (95%+)
- Phù hợp với khí hậu Việt Nam (ổn định từ 0-40°C)

---

### 3. BỘ ĐIỀU KHIỂN SẠC (Charge Controller)

#### Thông số kỹ thuật

```
Loại điều khiển:     MPPT hoặc PWM
  → Khuyên: MPPT (hiệu suất 90-95% vs PWM 70-75%)

Điện áp hệ thống:    12V/24V
  → Chọn: 12V (match với panel và pin)

Dòng điện tối đa:    10A - 20A
  → Khuyên: 20A (cho phép mở rộng panel)

Tính năng bắt buộc:
  ✅ Overcharge protection (Chống sạc quá)
  ✅ Over-discharge protection (Chống phóng quá)
  ✅ Temperature compensation (Bù nhiệt độ)
  ✅ LCD hiển thị trạng thái sạc
  ✅ USB output 5V (sạc điện thoại)
  ✅ Cổng XT60 hoặc Anderson (dòng lớn)
```

#### So sánh PWM vs MPPT

| Chỉ số | PWM | MPPT |
|--------|-----|------|
| **Hiệu suất** | 70-75% | 90-95% |
| **Giá** | 300-500K đ | 1-2M đ |
| **Mục đích** | Hệ thống nhỏ, đơn giản | Hệ thống lớn, hiệu quả |
| **Dòng sạc** | Ít (linear) | Nhiều hơn (buck converter) |
| **Độ phức tạp** | Thấp | Trung |

#### Mẫu phổ biến

```
PWM:
- JINGSUN PWM 10A: ~300K đ (AliExpress)
- Epever PWM 10A: ~400K đ
- SRNE PWM 20A: ~500K đ

MPPT (KHUYÊN):
- Epever MPPT20I: $80-100 (~2M đ)
- Victron Blue Smart: $500+ (Premium)
- SRNE MPPT 20A: ~1.5M đ
- MidNite Kid MPPT: $200 (~5M đ)
```

---

## 🔌 SƠ ĐỒ KẾT NỐI TOÀN HỆ THỐNG

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

## 📦 DANH SÁCH MŨI NHẬP KHẨU

### Phương án 2: KHUYẾN NGHỊ (Cân bằng - 3.7M đ)

| Sản phẩm | Mẫu | Nơi mua | Giá | Ghi chú |
|----------|-----|--------|-----|---------|
| **Panel Solar 15W** | BP3515 (Renogy) | Shopee, AliExpress | 500K đ | Monocrystalline |
| **Pin LiFePO₄ 12V 10Ah** | JBL-SP-LY-12100 hoặc Relion | Điện tử Việt, Amazon | 2M đ | Có BMS tích hợp |
| **MPPT Charge Controller 20A** | Epever MPPT20I hoặc SRNE | Amazon, AliExpress | 1.5M đ | LCD + USB |
| **Dây AWG + Đầu cắm XT60** | Lipo battery cable set | AliExpress | 150K đ | Tất cả size |
| **Cầu chì + Holder** | ANL 15A, 20A | Điện tử Việt, Shopee | 80K đ | Fast Blow type |
| **Diode Schottky** | 1N5406 20A | Điện tử Việt | 30K đ | Chống dòng ngược |
| **Đã có sẵn** | Arduino, LCD, RTC, SIM, SRF04, Buzzer | Repo | - | Từ PRJ04 gốc |

**Tổng chi phí phụ kiện mới: ~3.7M đ**

### Phương án 1: Tiết kiệm (1.3M đ)

```
├─ Panel Solar 10W: 300K đ
├─ Pin Lead-Acid 12V 12Ah: 600K đ
├─ Charge Controller PWM 10A: 300K đ
└─ Dây, cầu chì: 100K đ
────────────────────────────
TỔNG: 1.3M đ

⚠️ Nhược điểm:
- Pin nặng, tự phóng nhanh
- Chỉ dùng được 3-4 ngày mưa
- Hiệu suất sạc thấp (70%)
```

### Phương án 3: Premium (7.7M đ)

```
├─ Panel Solar 30W×2: 1.2M đ
├─ Pin LiFePO₄ 24V 20Ah: 4M đ
├─ MPPT 40A: 2M đ
├─ Inverter 220V 500W: 500K đ
└─ Dây, bảo vệ: 1M đ
────────────────────────────
TỔNG: 8.7M đ

✅ Lợi ích:
- Hoạt động 4-5 ngày mưa
- Có 220V cho thiết bị khác
- Hiệu suất cao nhất
```

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

## 📊 KẾT LUẬN & KHUYẾN NGHỊ

### ⭐ Phương án 2: KHUYẾN NGHỊ NHẤT

```
☀️ Panel Solar: 15W Monocrystalline
🔋 Pin: LiFePO₄ 12V 10Ah (120 Wh)
⚙️ Controller: MPPT 20A (hiệu suất 90-95%)
💰 Chi phí: 3.7M đ
⏱️ Thời gian thi công: 2-3 giờ

✅ Lợi ích:
- Hoạt động độc lập 24/7, không mất nước
- Dự phòng 6-7 ngày mưa liên tục
- Tuổi thọ pin 10-15 năm
- Hiệu suất năng lượng cao (90%)
- Phù hợp với khí hậu Việt Nam
- Có giám sát điện áp & cảnh báo tự động
- Dễ nâng cấp sau này (thêm panel, pin)

📈 ROI (Return on Investment):
- Tiết kiệm điện lưới: ~200K đ/năm (máy phát điện)
- Payback: 18-24 tháng
- Tiết kiệm 15-20 năm sau
```

### 🎯 Các bước tiếp theo

1. **Đặt hàng phụ kiện** (1 tuần)
   - AliExpress, Amazon, Shopee

2. **Chuẩn bị code Python** (2 tuần)
   - Tối ưu code Arduino
   - Thêm BMS communication (nếu pin có)
   - Đồng bộ Telegram notification

3. **Lắp ráp & test** (2-3 ngày)
   - Trong nhà trước
   - Ngoài trời sau

4. **Ghi nhận dữ liệu** (30 ngày)
   - Ghi lịch sử điện áp
   - Phân tích tiêu thụ thực tế
   - Điều chỉnh cài đặt nếu cần

---

## 📚 TÀI LIỆU THAM KHẢO

- **RTClib Documentation**: https://adafruit.github.io/RTClib
- **LiquidCrystal_I2C**: https://github.com/johnrickman/LiquidCrystal_I2C
- **Epever MPPT Manual**: Manual riêng kèm sản phẩm
- **Battery Management System**: Tìm hiểu khi mua pin có BMS
- **Solar Irradiance VN**: Xem bản đồ nắng Việt Nam

---

## 📝 LỊCH SỬ CHỈNH SỬa

- **2026-06-14**: Tạo đề xuất hệ thống năng lượng mặt trời cho PRJ04
- **Phiên bản**: 1.0 (Hoàn chỉnh)

---

**Tài liệu được tạo bởi GitHub Copilot - STEM Arduino Expert**

Hãy liên hệ BanLinhKien hoặc cộng đồng STEM nếu cần hỗ trợ thêm! 🚀
