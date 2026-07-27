/*
 * ======================================================================================
 * TÊN BỘ KIT : BỘ KIT HỌC TẬP ARDUINO UNO R3 BLK PLUS
 * DỰ ÁN      : BLKLab_PRJ02_He_Thong_Thu_Hoach_Nuoc_Mua_Thong_Minh
 * TÊN PRJ    : HỆ THỐNG THU HOẠCH NƯỚC MƯA THÔNG MINH VÀ ĐIỀU TIẾT NƯỚC TỰ ĐỘNG
 * ĐƠN VỊ     : BÁN LINH KIỆN (banlinhkien.com)
 * ======================================================================================
 * 
 * 1. MỤC TIÊU & TÍNH NĂNG CHÍNH:
 *    - Tách xả rửa bẩn nước mưa đầu mùa theo thời gian xác nhận mưa (tính bằng GIÂY).
 *    - Thu gom nước mưa sạch vào bể chứa khi hết thời gian xả rửa.
 *    - Tự động tưới cây theo độ ẩm đất (Bật khi đất khô <= Ngưỡng Khô, Tắt khi đất ẩm >= Ngưỡng Ẩm).
 *    - Đo mực nước bể theo % dựa trên Chiều cao bể (tính bằng MM).
 *    - Mở van Servo xả tràn khi bể đầy >= Ngưỡng Đầy (90%). Khóa bơm khi bể cạn <= Ngưỡng Cạn (10%).
 *    - Menu cài đặt 9 thông số trên LCD1602 lưu bộ nhớ EEPROM không mất khi cúp điện.
 *    - Hỗ trợ Chế độ Thủ công (MANUAL) bật/tắt Bơm và xoay Servo trực tiếp bằng nút bấm.
 * 
 * 2. SƠ ĐỒ ĐẤU NỐI CHÂN (PINOUT ARDUINO UNO R3):
 *    -----------------------------------------------------------------------------------
 *    | Linh kiện / Module   | Chân linh kiện | Chân Arduino Uno | Ghi chú             |
 *    -----------------------------------------------------------------------------------
 *    | Cảm biến Mưa         | DO             | D2               | LOW = Có mưa        |
 *    | Cảm biến Siêu âm     | TRIG / ECHO    | D3 / D4          | Đo mực nước bể      |
 *    | Nút BACK (BT5)       | 1 chân Nút     | D5               | ESC / Bật Bơm (MAN) |
 *    | Nút OK (BT6)         | 1 chân Nút     | D6               | OK / Giữ 1.5s ĐổiMode|
 *    | Nút DOWN (BT7)       | 1 chân Nút     | D7               | Cuộn xuống / Giảm(-)|
 *    | Module Relay (Bơm)   | IN             | D8               | LOW = Bật Bơm       |
 *    | Động cơ Servo SG90   | Signal (Cam)   | D9 (PWM)         | Van xoay 15° - 165° |
 *    | Nút UP (BT10)        | 1 chân Nút     | D10              | Cuộn lên / Tăng(+)  |
 *    | Vòng LED NeoPixel 12 | DIN            | D11              | LED báo màu RGB     |
 *    | Còi báo Buzzer       | Signal         | D12              | Còi bíp & Cảnh báo  |
 *    | Màn hình LCD1602+RTC | SDA / SCL      | A4 / A5 (I2C)    | Bus I2C chung       |
 *    | Cảm biến Đất         | AO             | A0 (Analog)      | ADC độ ẩm 0 - 1023  |
 *    -----------------------------------------------------------------------------------
 * ======================================================================================
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

// ===================== KHAI BÁO STRUCT & ENUM (ĐẶT ĐẦU FILE) =====================
// Cấu trúc quản lý chống dội nút bấm (Debounce)
struct Button {
  byte pin;
  bool lastRaw;
  bool stable;
  unsigned long changedMs;
  unsigned long downMs;
  bool longSent;
};

// Cấu trúc lưu trữ 9 thông số cài đặt vào bộ nhớ EEPROM
struct Settings {
  uint16_t rainConfirmSec; // 1. TG xác nhận mưa & xả rửa bẩn (5 - 300 giây)
  uint16_t soilDry;        // 2. Ngưỡng đất khô bật bơm (10% - 50%)
  uint16_t soilWet;        // 3. Ngưỡng đất ẩm tắt bơm (40% - 80%)
  uint16_t pumpMaxSec;     // 4. Thời gian bơm tối đa (10 - 120 giây)
  uint16_t tankFull;       // 5. Ngưỡng bể đầy xả tràn (70% - 98%)
  uint16_t tankLow;        // 6. Ngưỡng bể cạn khóa bơm (5% - 25%)
  uint16_t servoXa;        // 7. Góc xoay Servo xả (0 - 90 độ)
  uint16_t servoThu;       // 8. Góc xoay Servo thu (90 - 180 độ)
  uint16_t tankHeightMm;   // 9. Chiều cao bể chứa (100 - 2000 mm)
};

// Cấu trúc quản lý mục menu hiển thị trên LCD 1602
struct MenuItem {
  const char* label;
  uint16_t* valPtr;
  uint16_t minVal;
  uint16_t maxVal;
  uint16_t step;
  const char* unit;
};

enum Mode { AUTO_MODE, MANUAL_MODE };
enum RainState { WAIT_RAIN, CONFIRM_RAIN, COLLECT_WATER, FULL_DRAIN };
enum UIState { MAIN_SCREEN, MENU_BROWSE, MENU_EDIT };

// ===================== KHAI BÁO CHÂN (PINOUT ARDUINO UNO CHUẨN) =====================
const byte PIN_RAIN   = 2;   // Cảm biến mưa DO
const byte PIN_TRIG   = 3;   // HC-SR04 TRIG
const byte PIN_ECHO   = 4;   // HC-SR04 ECHO
const byte PIN_BACK   = 5;   // Nút BACK / ESC (BT5 - Chân D5)
const byte PIN_OK     = 6;   // Nút OK / ENTER (BT6 - Chân D6)
const byte PIN_DOWN   = 7;   // Nút DOWN (BT7 - Chân D7)
const byte PIN_RELAY  = 8;   // Module Relay Bơm
const byte PIN_SERVO  = 9;   // Động cơ Servo SG90 (Chân D9 PWM)
const byte PIN_UP     = 10;  // Nút UP (BT10 - Chân D10)
const byte PIN_NEOPIX = 11;  // Vòng NeoPixel 12 LED
const byte PIN_BUZZER = 12;  // Còi báo nhỏ (<= 20mA)
const byte PIN_SOIL   = A0;  // Cảm biến độ ẩm đất AO

// ===================== KHAI BÁO MODULE & BIẾN TOÀN CỤC =====================
LiquidCrystal_I2C lcd(0x27, 16, 2); // Đổi thành 0x3F nếu màn hình không lên chữ
RTC_DS3231 rtc;
bool rtcPresent = false;
Servo waterServo;
Adafruit_NeoPixel pixels(12, PIN_NEOPIX, NEO_GRB + NEO_KHZ800);

const byte RELAY_ON  = LOW;   // Relay kích mức LOW
const byte RELAY_OFF = HIGH;

const int DAT_KHO_RAW = 1023; // ADC đất khô
const int DAT_UOT_RAW = 450;  // ADC đất ẩm ngập

const unsigned long KHOANG_KHO_MS  = 5000UL;
const unsigned long LCD_REFRESH_MS = 300UL;

Settings settings;
const int EEPROM_MAGIC = 0xA8; // Mã kiểm tra cập nhật EEPROM 9 thông số

// Danh mục 9 thông số cài đặt tùy chỉnh trên LCD
const byte NUM_ITEMS = 9;
MenuItem menuList[NUM_ITEMS] = {
  {"1.TG XacNhan", &settings.rainConfirmSec, 5,   300,  5,  "s"},
  {"2.Dat Kho",   &settings.soilDry,        10,  50,   1,  "%"},
  {"3.Dat Am",    &settings.soilWet,        40,  80,   1,  "%"},
  {"4.TG Bom",    &settings.pumpMaxSec,     10,  120,  5,  "s"},
  {"5.Be Day",    &settings.tankFull,       70,  98,   1,  "%"},
  {"6.Be Can",    &settings.tankLow,        5,   25,   1,  "%"},
  {"7.Goc Xa",    &settings.servoXa,        0,   90,   5,  "d"},
  {"8.Goc Thu",   &settings.servoThu,       90,  180,  5,  "d"},
  {"9.Cao Be",    &settings.tankHeightMm,   100, 2000, 10, "mm"}
};

Mode mode = AUTO_MODE;
RainState rainState = WAIT_RAIN;
UIState uiState = MAIN_SCREEN;

bool pumpOn = false;
bool servoCollecting = false;
bool confirmedRain = false;
bool emergencyStop = false;

byte mainPage = 0;
byte menuIndex = 0;
uint16_t tempEditVal = 0;

unsigned long rainStartMs = 0;
unsigned long lastWetMs = 0;
unsigned long pumpStartMs = 0;
unsigned long lastLcdMs = 0;
unsigned long lastSensorMs = 0;
unsigned long lastSerialMs = 0;

float waterPercent = 0;
float currentDistance = -1;
int soilPercent = 0;
int soilRaw = 0;
bool levelError = false;
bool soilError = false;

Button bUp   = {PIN_UP,   HIGH, HIGH, 0, 0, false};
Button bDown = {PIN_DOWN, HIGH, HIGH, 0, 0, false};
Button bOk   = {PIN_OK,   HIGH, HIGH, 0, 0, false};
Button bBack = {PIN_BACK, HIGH, HIGH, 0, 0, false};

// ===================== CÁC HÀM XỬ LÝ EEPROM & NÚT =====================
void saveSettings() {
  EEPROM.write(0, EEPROM_MAGIC);
  EEPROM.put(1, settings);
  Serial.println(F("[EEPROM] Da luu cai dat 9 thong so moi!"));
}

void loadSettings() {
  if (EEPROM.read(0) == EEPROM_MAGIC) {
    EEPROM.get(1, settings);
  } else {
    settings.rainConfirmSec = 60;  // Mặc định xác nhận mưa 60 giây (vừa xả rửa vừa xác nhận)
    settings.soilDry        = 30;
    settings.soilWet        = 60;
    settings.pumpMaxSec     = 30;
    settings.tankFull       = 90;
    settings.tankLow        = 10;
    settings.servoXa        = 15;  // Mặc định góc Xả 15 độ
    settings.servoThu       = 165; // Mặc định góc Thu 165 độ
    settings.tankHeightMm   = 300; // Mặc định chiều cao bể 300 mm
    saveSettings();
  }
}

unsigned long rainConfirmMs() {
  return (unsigned long)settings.rainConfirmSec * 1000UL;
}

unsigned long pumpMaxMs() {
  return (unsigned long)settings.pumpMaxSec * 1000UL;
}

void beep(unsigned int ms = 50) {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(ms);
  digitalWrite(PIN_BUZZER, LOW);
}

bool updateButton(Button &b, bool &shortPress, bool &longPress) {
  shortPress = false;
  longPress = false;
  bool raw = digitalRead(b.pin);
  unsigned long now = millis();

  if (raw != b.lastRaw) {
    b.lastRaw = raw;
    b.changedMs = now;
  }

  if (now - b.changedMs >= 35 && raw != b.stable) {
    b.stable = raw;
    if (b.stable == LOW) {
      b.downMs = now;
      b.longSent = false;
    } else {
      if (!b.longSent && now - b.downMs >= 40) shortPress = true;
    }
  }

  if (b.stable == LOW && !b.longSent && now - b.downMs >= 1500) {
    b.longSent = true;
    longPress = true;
  }
  return b.stable == LOW;
}

// ===================== HIỂN THỊ MÀN HÌNH LCD CHUẨN 16 KÝ TỰ =====================
void setPixels(uint8_t r, uint8_t g, uint8_t b) {
  pixels.fill(pixels.Color(r, g, b));
  pixels.show();
}

void showMessage(const __FlashStringHelper *line1, const __FlashStringHelper *line2) {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(line1);
  lcd.setCursor(0, 1); lcd.print(line2);
}

void changeUIState(UIState newState) {
  uiState = newState;
  lcd.clear();
}

void drawLCD() {
  if (emergencyStop) {
    lcd.setCursor(0, 0); lcd.print(F("DUNG KHAN CAP!  "));
    lcd.setCursor(0, 1); lcd.print(F("BAM BACK HUY    "));
    return;
  }

  if (uiState == MAIN_SCREEN) {
    if (mainPage == 0) {
      byte h = 0, m = 0;
      if (rtcPresent) {
        DateTime now = rtc.now();
        h = now.hour();
        m = now.minute();
      } else {
        unsigned long totalSec = millis() / 1000;
        h = (totalSec / 3600) % 24;
        m = (totalSec / 60) % 60;
      }

      lcd.setCursor(0, 0);
      lcd.print(mode == AUTO_MODE ? F("AUTO ") : F("MAN  "));
      if (h < 10) lcd.print('0'); lcd.print(h); lcd.print(':');
      if (m < 10) lcd.print('0'); lcd.print(m);
      lcd.print(F(" B:")); 
      if ((int)waterPercent < 100) lcd.print(' ');
      if ((int)waterPercent < 10) lcd.print(' ');
      lcd.print((int)waterPercent); lcd.print(F("%"));

      lcd.setCursor(0, 1);
      lcd.print(F("Dat:")); 
      if (soilPercent < 100) lcd.print(' ');
      if (soilPercent < 10) lcd.print(' ');
      lcd.print(soilPercent); lcd.print(F("% "));
      lcd.print(pumpOn ? F("BOM:ON ") : F("BOM:OFF"));
    } else if (mainPage == 1) {
      lcd.setCursor(0, 0);
      lcd.print(F("MUA:"));
      if (confirmedRain) lcd.print(F("THU NUOC    "));
      else if (digitalRead(PIN_RAIN) == LOW) lcd.print(F("XA RAIN     "));
      else lcd.print(F("TANH MUA    "));

      lcd.setCursor(0, 1);
      lcd.print(servoCollecting ? F("SERVO:THU NUOC  ") : F("SERVO:XA NUOC   "));
    } else {
      lcd.setCursor(0, 0);
      lcd.print(F("KC BE:"));
      if (levelError) {
        lcd.print(F("LOI         "));
      } else {
        lcd.print(currentDistance, 1); lcd.print(F("cm    "));
      }

      lcd.setCursor(0, 1);
      lcd.print(F("CAO BE:")); lcd.print(settings.tankHeightMm);
      lcd.print(F("mm   "));
    }
  } 
  else if (uiState == MENU_BROWSE) {
    byte topIndex = (menuIndex / 2) * 2;
    for (byte r = 0; r < 2; r++) {
      byte idx = topIndex + r;
      lcd.setCursor(0, r);
      if (idx < NUM_ITEMS) {
        lcd.print(idx == menuIndex ? '>' : ' ');
        lcd.print(menuList[idx].label);
        lcd.print(':');
        uint16_t val = *(menuList[idx].valPtr);
        if (val < 1000) lcd.print(' ');
        if (val < 100) lcd.print(' ');
        if (val < 10) lcd.print(' ');
        lcd.print(val);
        lcd.print(menuList[idx].unit);
        lcd.print(F("   "));
      } else {
        lcd.print(F("                "));
      }
    }
  } 
  else if (uiState == MENU_EDIT) {
    lcd.setCursor(0, 0);
    lcd.print(F("SUA: "));
    lcd.print(menuList[menuIndex].label);
    lcd.print(F("       "));

    lcd.setCursor(0, 1);
    lcd.print(F("VAL:["));
    if (tempEditVal < 1000) lcd.print(' ');
    if (tempEditVal < 100) lcd.print(' ');
    if (tempEditVal < 10) lcd.print(' ');
    lcd.print(tempEditVal);
    lcd.print(menuList[menuIndex].unit);
    lcd.print(F("]   "));
  }
}

// ===================== ĐỌC CẢM BIẾN AN TOÀN =====================
float readDistanceOnce() {
  if (digitalRead(PIN_ECHO) == HIGH) {
    return -1.0;
  }

  digitalWrite(PIN_TRIG, LOW); delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long duration = pulseIn(PIN_ECHO, HIGH, 8000UL);
  if (duration == 0) return -1.0;
  return duration * 0.0343 / 2.0; // Khoảng cách tính bằng cm
}

void updateSensors() {
  unsigned long now = millis();
  if (now - lastSensorMs < 1000) return;
  lastSensorMs = now;

  currentDistance = readDistanceOnce();
  if (currentDistance < 0) {
    levelError = true;
  } else {
    levelError = false;
    float distMm = currentDistance * 10.0;
    float tankHeight = (float)settings.tankHeightMm;
    float minBlindZoneMm = 50.0; // Khoảng mù 5cm

    if (distMm >= tankHeight) {
      waterPercent = 0;
    } else if (distMm <= minBlindZoneMm) {
      waterPercent = 100;
    } else {
      float waterDepthMm = tankHeight - distMm;
      float maxWaterDepthMm = tankHeight - minBlindZoneMm;
      waterPercent = constrain((waterDepthMm * 100.0) / maxWaterDepthMm, 0.0, 100.0);
    }
  }

  soilRaw = analogRead(PIN_SOIL);
  soilError = (soilRaw < 0 || soilRaw > 1023);
  soilPercent = constrain(map(soilRaw, DAT_KHO_RAW, DAT_UOT_RAW, 0, 100), 0, 100);
}

bool rainWet() {
  return digitalRead(PIN_RAIN) == LOW;
}

// ===================== ĐIỀU KHIỂN CHẤP HÀNH =====================
void setPump(bool on) {
  if (on && mode == AUTO_MODE && (levelError || waterPercent <= settings.tankLow)) {
    on = false;
  }
  if (on && emergencyStop) on = false;

  if (on && !pumpOn) pumpStartMs = millis();
  pumpOn = on;
  digitalWrite(PIN_RELAY, on ? RELAY_ON : RELAY_OFF);
  
  Serial.print(F("[RELAY PUMP] BOM MANG TRANG THAI: "));
  Serial.println(pumpOn ? F("ON (KICH RELAY)") : F("OFF (NGAT RELAY)"));
}

void setServoCollect(bool collect) {
  servoCollecting = collect;
  uint16_t angle = collect ? settings.servoThu : settings.servoXa;
  
  if (!waterServo.attached()) {
    waterServo.attach(PIN_SERVO, 500, 2500);
  }
  
  waterServo.write(angle);
  
  Serial.print(F("[SERVO SG90] DAT GOC XOAY CAI DAT TRUCTIEP: "));
  Serial.print(angle);
  Serial.println(collect ? F(" DEG (GOC THU)") : F(" DEG (GOC XA)"));
}

void emergency() {
  emergencyStop = true;
  setPump(false);
  setServoCollect(false);
  setPixels(120, 0, 0);
  drawLCD();
  beep(300);
  Serial.println(F("[ALERT] DUNG KHAN CAP!"));
}

void updateAuto() {
  unsigned long now = millis();
  bool wet = rainWet();

  if (wet) lastWetMs = now;

  // Bắt đầu có mưa -> Vừa xả rửa bẩn vừa tính thời gian xác nhận mưa
  if (rainState == WAIT_RAIN && wet) {
    rainStartMs = now;
    confirmedRain = false;
    rainState = CONFIRM_RAIN;
    setServoCollect(false); // Van giữ vị trí XẢ rửa bẩn
    setPixels(0, 0, 100);
    beep(80);
    Serial.println(F("[MUA] Phat hien mua! Bat dau dem TG Xac Nhan Mua & Xa rua bieu mau..."));
  }

  // Trong giai đoạn xác nhận mưa & xả rửa
  if (rainState == CONFIRM_RAIN) {
    if (now - rainStartMs >= rainConfirmMs() && now - lastWetMs < KHOANG_KHO_MS) {
      confirmedRain = true;
      rainState = (waterPercent >= settings.tankFull) ? FULL_DRAIN : COLLECT_WATER;
      setServoCollect(rainState == COLLECT_WATER);
      setPixels(0, 100, 0);
      Serial.println(F("[MUA] Xac nhan mua thanh cong & Da xa rua xong! Chuyen van sang THU NUOC SACH."));
    } else if (now - lastWetMs >= KHOANG_KHO_MS) {
      rainState = WAIT_RAIN;
      setServoCollect(false);
      setPixels(0, 0, 20);
      Serial.println(F("[MUA] Mua qua ngan -> Huy xac nhan."));
    }
  }

  if (rainState == COLLECT_WATER && waterPercent >= settings.tankFull) {
    rainState = FULL_DRAIN;
    setServoCollect(false);
    setPixels(120, 60, 0);
    beep(150);
    Serial.println(F("[CANH BAO] Be day -> Xa tran!"));
  }

  if (rainState == FULL_DRAIN && waterPercent <= (settings.tankFull - 10) && wet) {
    rainState = COLLECT_WATER;
    setServoCollect(true);
  }

  if (rainState != WAIT_RAIN && now - lastWetMs >= KHOANG_KHO_MS) {
    rainState = WAIT_RAIN;
    confirmedRain = false;
    setServoCollect(false);
    setPump(false);
    setPixels(0, 0, 20);
    Serial.println(F("[MUA] Tanh mua -> Quay ve cho."));
  }

  if (rainState != CONFIRM_RAIN && rainState != FULL_DRAIN && !levelError && waterPercent > settings.tankLow) {
    if (soilPercent <= settings.soilDry && !pumpOn) {
      setPump(true);
      Serial.println(F("[BOM] Dat kho -> Bat bom tuoi!"));
    }
    if (soilPercent >= settings.soilWet && pumpOn) {
      setPump(false);
      Serial.println(F("[BOM] Dat du am -> Tat bom!"));
    }
  }
}

// ===================== XỬ LÝ NÚT BẤM VÀ UX MENU =====================
void handleButtons() {
  bool upS, upL, downS, downL, okS, okL, backS, backL;

  bool upHeld   = updateButton(bUp, upS, upL);
  bool downHeld = updateButton(bDown, downS, downL);
  bool okHeld   = updateButton(bOk, okS, okL);
  bool backHeld = updateButton(bBack, backS, backL);

  if (emergencyStop) {
    if (backS) { 
      emergencyStop = false; 
      showMessage(F("HUY DUNG KHAN CAP"), F("KHOI DONG LAI...")); 
      delay(1000); 
      drawLCD(); 
    }
    return;
  }

  // --- 1. MÀN HÌNH CHÍNH ---
  if (uiState == MAIN_SCREEN) {
    if (okL) { // Giữ OK (BT6 D6) 1.5s -> Đổi AUTO / MANUAL
      beep(150);
      setPump(false);
      mode = (mode == AUTO_MODE) ? MANUAL_MODE : AUTO_MODE;
      showMessage(mode == AUTO_MODE ? F("CHUYEN SANG AUTO") : F("CHUYEN SANG MAN"), F("BOM DA TAT"));
      delay(1000);
      changeUIState(MAIN_SCREEN);
      drawLCD();
      Serial.print(F("[MODE] Chuyen sang che do: "));
      Serial.println(mode == AUTO_MODE ? F("AUTO") : F("MANUAL"));
    } else if (okS) { // Bấm OK (BT6 D6) ngắn -> Vào Menu Cài đặt
      beep(50);
      menuIndex = 0;
      changeUIState(MENU_BROWSE);
      drawLCD();
    } else if (mode == MANUAL_MODE) {
      // Ở CHẾ ĐỘ MANUAL:
      if (backS) { // Bấm BACK (BT5 D5) -> Bật/Tắt Bơm
        beep(50);
        Serial.println(F("[MANUAL BUTTON] Bam BACK -> Doi trang thai Bom"));
        setPump(!pumpOn);
        drawLCD();
      } else if (upS) { // Bấm UP (BT10 D10) -> Xoay Servo Thu/Xả
        beep(50);
        bool newCollectState = !servoCollecting;
        Serial.print(F("[MANUAL BUTTON] Bam UP -> Xoay Servo sang trang thai: "));
        Serial.println(newCollectState ? F("THU") : F("XA"));
        setServoCollect(newCollectState);
        drawLCD();
      } else if (downS) { // Bấm DOWN (BT7 D7) -> Chuyển trang xem LCD
        beep(30);
        mainPage = (mainPage + 1) % 3;
        drawLCD();
      }
    } else {
      // Ở CHẾ ĐỘ AUTO:
      if (upS) { // Bấm UP -> Chuyển trang
        beep(30);
        mainPage = (mainPage + 2) % 3;
        drawLCD();
      } else if (downS) { // Bấm DOWN -> Chuyển trang
        beep(30);
        mainPage = (mainPage + 1) % 3;
        drawLCD();
      }
    }
  }
  // --- 2. MENU DUYỆT DANH SÁCH THÔNG SỐ ---
  else if (uiState == MENU_BROWSE) {
    if (upS) { // Cuộn lên (BT10 D10)
      beep(30);
      if (menuIndex > 0) menuIndex--;
      else menuIndex = NUM_ITEMS - 1;
      drawLCD();
    } else if (downS) { // Cuộn xuống (BT7 D7)
      beep(30);
      if (menuIndex < NUM_ITEMS - 1) menuIndex++;
      else menuIndex = 0;
      drawLCD();
    } else if (okS) { // Chọn mục sửa (BT6 D6)
      beep(50);
      tempEditVal = *(menuList[menuIndex].valPtr);
      changeUIState(MENU_EDIT);
      drawLCD();
    } else if (backS) { // Thoát về màn hình chính (BT5 D5)
      beep(50);
      changeUIState(MAIN_SCREEN);
      drawLCD();
    }
  }
  // --- 3. CHẾ ĐỘ SỬA GIÁ TRỊ THÔNG SỐ ---
  else if (uiState == MENU_EDIT) {
    if (upS) { // Tăng giá trị (BT10 D10)
      beep(30);
      uint16_t st = menuList[menuIndex].step;
      if (tempEditVal + st <= menuList[menuIndex].maxVal) {
        tempEditVal += st;
      } else {
        tempEditVal = menuList[menuIndex].maxVal;
      }
      drawLCD();
    } else if (downS) { // Giảm giá trị (BT7 D7)
      beep(30);
      uint16_t st = menuList[menuIndex].step;
      if (tempEditVal >= menuList[menuIndex].minVal + st) {
        tempEditVal -= st;
      } else {
        tempEditVal = menuList[menuIndex].minVal;
      }
      drawLCD();
    } else if (okS) { // Lưu giá trị (BT6 D6)
      beep(100);
      *(menuList[menuIndex].valPtr) = tempEditVal;
      saveSettings();
      setServoCollect(servoCollecting);
      changeUIState(MENU_BROWSE);
      drawLCD();
    } else if (backS) { // Hủy sửa (BT5 D5)
      beep(50);
      changeUIState(MENU_BROWSE);
      drawLCD();
    }
  }
}

// ===================== KHỞI TẠO HỆ THỐNG =====================
void setup() {
  Serial.begin(9600);
  Serial.println(F("\n=== BLKLab_PRJ02 HE THONG THU HOACH NUOC MUA THONG MINH ==="));

  Wire.begin();

  loadSettings();

  pinMode(PIN_RAIN, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BACK, INPUT_PULLUP);
  pinMode(PIN_OK, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_UP, INPUT_PULLUP);

  digitalWrite(PIN_RELAY, RELAY_OFF);
  digitalWrite(PIN_BUZZER, LOW);

  waterServo.attach(PIN_SERVO, 500, 2500);
  setServoCollect(false); // Khởi động an toàn: XẢ theo góc cài đặt servoXa

  lcd.init();
  lcd.backlight();
  pixels.begin();
  pixels.setBrightness(35);
  setPixels(0, 0, 20);

  rtcPresent = rtc.begin();
  if (rtcPresent) {
    Serial.println(F("[RTC] Tim thay DS3231/DS1307"));
    if (rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println(F("[RTC] Khong tim thay RTC, chay gio noi bu!"));
  }

  showMessage(F("RAINWATER UNO"), F("KHOI DONG..."));
  delay(1200);
  changeUIState(MAIN_SCREEN);
  drawLCD();
  Serial.println(F("[SYSTEM] Khoidong thanh cong. Vao Main Loop."));
}

// ===================== VÒNG LẶP CHÍNH =====================
void loop() {
  handleButtons();
  updateSensors();

  if (!emergencyStop && uiState == MAIN_SCREEN) {
    if (mode == AUTO_MODE) updateAuto();
  }

  if (pumpOn && millis() - pumpStartMs >= pumpMaxMs()) {
    setPump(false);
    showMessage(F("BOM QUA THOI GIAN"), F("DA TU DONG TAT"));
    beep(250);
    delay(1200);
    changeUIState(MAIN_SCREEN);
    drawLCD();
  }

  if (uiState == MAIN_SCREEN && !emergencyStop && millis() - lastLcdMs >= LCD_REFRESH_MS) {
    lastLcdMs = millis();
    drawLCD();
  }

  if (millis() - lastSerialMs >= 1000) {
    lastSerialMs = millis();
    Serial.print(F("Mode:")); Serial.print(mode == AUTO_MODE ? F("AUTO") : F("MAN"));
    Serial.print(F(" | Rain:")); Serial.print(rainWet() ? F("WET") : F("DRY"));
    Serial.print(F(" | Dist:")); Serial.print(currentDistance, 1);
    Serial.print(F("cm | Tank:")); Serial.print((int)waterPercent);
    Serial.print(F("% | Soil:")); Serial.print(soilPercent);
    Serial.print(F("% | Pump:")); Serial.println(pumpOn ? F("ON") : F("OFF"));
  }
}
