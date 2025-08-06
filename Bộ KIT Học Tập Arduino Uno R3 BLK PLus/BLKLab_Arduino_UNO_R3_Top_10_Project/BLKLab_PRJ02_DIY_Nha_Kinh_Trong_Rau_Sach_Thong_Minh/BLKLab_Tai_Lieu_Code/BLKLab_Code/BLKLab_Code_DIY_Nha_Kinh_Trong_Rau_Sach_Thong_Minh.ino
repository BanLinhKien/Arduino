/* =================================================================================================*
 *                                          BANLINHKIEN.COM                                         *
 *==================================================================================================*                        
 *                    PRJ02: DIY Nhà kính trồng rau sạch thông minh
 * 
 * Đấu nối: 
 * Arduino UNO R3 vs LCD 16x2 I2C
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 * Arduino UNO R3 vs Module DS1307
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 * Arduino UNO R3 vs Module LED NEO Pixel 
 * 5V <-> + (chân giữa)  ;   GND <-> - (ký hiệu trừ)  ;   D6 <-> S (ký hiệu S signal)  
 * Arduino UNO R3 vs Cảm biến độ ẩm đất
 * 5V <-> VCC  ;   GND <-> GND  ;   A0 <-> A0 
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

// Dinh nghia cac chan ket noi
#define DHT_PIN 7               // Chan cam bien nhiet do do am
#define SOIL_MOISTURE_PIN A0    // Chan cam bien do am dat
#define RELAY_PIN 8             // Chan dieu khien relay (bom nuoc)
#define NEOPIXEL_PIN 6          // Chan dieu khien LED NeoPixel
#define BUTTON_MENU 5           // Chan nut nhan Menu
#define BUTTON_OK 4             // Chan nut nhan OK
#define BUTTON_UP 3             // Chan nut nhan Len
#define BUTTON_DOWN 2           // Chan nut nhan Xuong

// Cac hang so
#define DHT_TYPE DHT11          // Loai cam bien DHT
#define NUM_LEDS 12             // So luong LED NeoPixel
#define SOIL_MOISTURE_MIN 200   // Gia tri do am dat toi thieu
#define SOIL_MOISTURE_MAX 800   // Gia tri do am dat toi da
#define DEBOUNCE_DELAY 150      // Thoi gian chong rung nut nhan
#define LONG_PRESS_TIME 1000    // Thoi gian nhan giu de vao menu cai dat

// Khoi tao cac doi tuong
LiquidCrystal_I2C lcd(0x27, 16, 2); // Man hinh LCD I2C
DHT dht(DHT_PIN, DHT_TYPE);         // Cam bien nhiet do do am
RTC_DS1307 rtc;                     // Dong ho thoi gian thuc
Adafruit_NeoPixel pixels(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800); // LED NeoPixel

// Cau truc du lieu cho ho so cay trong
struct PlantProfile {
  char name[16];        // Ten cay trong (tieng Viet khong dau)
  int moistureThreshold; // Nguong do am dat (%)
  uint32_t ledColor;    // Mau sac LED
};

// Danh sach 6 loai cay trong
PlantProfile plants[6] = {
  {"Cai chua", 60, pixels.Color(255, 0, 0)},    // Do
  {"Xa lach", 70, pixels.Color(0, 255, 0)},     // Xanh
  {"Hung que", 65, pixels.Color(0, 0, 255)},    // Duong
  {"Ot", 55, pixels.Color(255, 255, 0)},        // Vang
  {"Dua leo", 75, pixels.Color(255, 0, 255)},   // Tim
  {"Tuy chinh", 60, pixels.Color(255, 255, 255)} // Trang
};

// Cau truc du lieu cho lich tuoi nuoc
struct WateringTime {
  int hour;      // Gio tuoi
  int minute;    // Phut tuoi
  bool enabled;  // Trang thai kich hoat
};

// Lich tuoi nuoc mac dinh
WateringTime schedules[3] = {
  {8, 0, false},   // 8:00
  {12, 0, false},  // 12:00
  {18, 0, false}   // 18:00
};

// Dia chi EEPROM de luu tru
#define EEPROM_PLANT_INDEX 0      // Vi tri luu chi so cay trong
#define EEPROM_CUSTOM_MOISTURE 1  // Vi tri luu do am tuy chinh
#define EEPROM_CUSTOM_COLOR 3     // Vi tri luu mau LED tuy chinh
#define EEPROM_SCHEDULE_BASE 7    // Vi tri bat dau luu lich tuoi nuoc

// Bien toan cuc
int currentPlantIndex = 0;        // Chi so cay trong dang chon
int customMoisture = 60;          // Do am dat tuy chinh
uint32_t customColor = pixels.Color(255, 255, 255); // Mau LED tuy chinh
int currentScreen = 0;            // Man hinh hien tai (0: chinh, 1: chon cay, 2: do am, 3: lich tuoi, 4: mau LED, 5: thong tin cay)
int lastScreen = -1;              // Man hinh truoc do de kiem tra chuyen doi
int menuIndex = 0;                // Vi tri trong menu
int scheduleEditMode = 0;         // 0: chinh gio, 1: chinh phut
unsigned long lastDebounceTime = 0; // Thoi gian chong rung nut nhan
unsigned long buttonPressTime = 0; // Thoi gian bat dau nhan nut
bool lastButtonState = HIGH;       // Trang thai nut nhan truoc do
bool isLongPress = false;          // Trang thai nhan giu nut
bool rtcFailed = false;           // Trang thai loi RTC

// Trang thai nut nhan
bool menuPressed = false;
bool okPressed = false;
bool upPressed = false;
bool downPressed = false;

// Bo dem de giam giat man hinh
float lastTemp = -1;               // Nhiet do truoc do
float lastHum = -1;                // Do am truoc do
int lastSoil = -1;                 // Do am dat truoc do
int lastHour = -1;                 // Gio truoc do
int lastMinute = -1;               // Phut truoc do
int lastMenuIndex = -1;            // Chi so menu truoc do (cho man hinh chon cay)

// Ham cai dat thoi gian qua Serial
void setTimeViaSerial() {
  Serial.println("RTC khong hoat dong! Nhap thoi gian theo dinh dang: HH:MM:SS DD/MM/YYYY");
  Serial.println("Vi du: 14:30:00 12/05/2025");

  while (!Serial.available()) {
    delay(100);
  }

  String input = Serial.readStringUntil('\n');
  input.trim();

  // Parse chuoi HH:MM:SS DD/MM/YYYY
  int hour, minute, second, day, month, year;
  if (sscanf(input.c_str(), "%d:%d:%d %d/%d/%d", &hour, &minute, &second, &day, &month, &year) == 6) {
    // Kiem tra gia tri hop le
    if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60 &&
        day > 0 && day <= 31 && month > 0 && month <= 12 && year >= 2020 && year <= 2099) {
      rtc.adjust(DateTime(year, month, day, hour, minute, second));
      Serial.println("Thoi gian da duoc cap nhat thanh cong!");
      rtcFailed = false;
    } else {
      Serial.println("Du lieu khong hop le! Vui long nhap lai.");
      setTimeViaSerial(); // Thu lai
    }
  } else {
    Serial.println("Dinh dang sai! Vui long nhap lai.");
    setTimeViaSerial(); // Thu lai
  }
}

void setup() {
  // Cai dat cac chan
  pinMode(BUTTON_MENU, INPUT_PULLUP);
  pinMode(BUTTON_OK, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Tat relay

  // Khoi tao cac thanh phan
  lcd.begin(16, 2);              // Khoi tao LCD 16x2
  lcd.backlight();               // Bat den nen LCD
  dht.begin();                   // Khoi tao cam bien DHT
  delay(1000);                   // Doi cam bien DHT on dinh

  // Khoi tao Serial
  Serial.begin(9600);

  // Khoi tao RTC
  if (!rtc.begin() || rtc.now().year() < 2020) {
    rtcFailed = true;
    Serial.println("Loi: Khong the khoi dong RTC!");
    setTimeViaSerial();
  } else {
    rtcFailed = false;
  }

  pixels.begin();                // Khoi tao LED NeoPixel
  pixels.setBrightness(50);      // Dat do sang LED

  // Tai du lieu tu EEPROM
  currentPlantIndex = EEPROM.read(EEPROM_PLANT_INDEX);
  if (currentPlantIndex > 5) currentPlantIndex = 0;
  
  customMoisture = EEPROM.read(EEPROM_CUSTOM_MOISTURE);
  if (customMoisture < 0 || customMoisture > 100) customMoisture = 60;
  
  EEPROM.get(EEPROM_CUSTOM_COLOR, customColor);
  
  for (int i = 0; i < 3; i++) {
    EEPROM.get(EEPROM_SCHEDULE_BASE + i * sizeof(WateringTime), schedules[i]);
  }

  // Cai dat mau LED ban dau
  updateLEDColor();
}

void loop() {
  // Kiem tra Serial neu RTC loi
  if (rtcFailed && Serial.available()) {
    setTimeViaSerial();
  }

  // Doc du lieu cam bien
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  // Kiem tra loi doc cam bien DHT
  if (isnan(temperature) || isnan(humidity)) {
    temperature = 0.0;
    humidity = 0.0;
  }
  int soilMoistureRaw = analogRead(SOIL_MOISTURE_PIN);
  int soilMoisture = map(soilMoistureRaw, SOIL_MOISTURE_MIN, SOIL_MOISTURE_MAX, 0, 100);
  soilMoisture = constrain(soilMoisture, 0, 100);

  // Lay thoi gian hien tai
  DateTime now = rtc.now();

  // Xu ly nut nhan
  handleButtons();

  // Cap nhat man hinh
  updateDisplay(temperature, humidity, soilMoisture, now);

  // Dieu khien bom nuoc
  controlPump(soilMoisture);

  // Kiem tra lich tuoi nuoc
  checkWateringSchedule(now);

  delay(30); // Giam delay de tang toc do phan hoi
}

// Ham xu ly nut nhan
void handleButtons() {
  // Doc trang thai nut nhan
  bool menuState = digitalRead(BUTTON_MENU);
  bool okState = digitalRead(BUTTON_OK);
  bool upState = digitalRead(BUTTON_UP);
  bool downState = digitalRead(BUTTON_DOWN);

  // Xu ly nut Menu (nhan ngan va nhan giu)
  if (menuState != lastButtonState && millis() - lastDebounceTime > DEBOUNCE_DELAY) {
    if (menuState == LOW) {
      buttonPressTime = millis();
    } else {
      if (millis() - buttonPressTime < LONG_PRESS_TIME) {
        // Nhan ngan: chuyen doi giua man hinh chinh va thong tin cay
        if (currentScreen == 0) {
          currentScreen = 5;
        } else if (currentScreen == 5) {
          currentScreen = 0;
        }
      }
    }
    lastDebounceTime = millis();
  }

  if (menuState == LOW && millis() - buttonPressTime >= LONG_PRESS_TIME && !isLongPress) {
    // Nhan giu: vao menu cai dat
    currentScreen = 1;
    menuIndex = 0;
    lastMenuIndex = -1; // Dat lai de cap nhat man hinh chon cay
    isLongPress = true;
  }
  if (menuState == HIGH) {
    isLongPress = false;
  }
  lastButtonState = menuState;

  // Xu ly cac nut khac
  if (okState == LOW && !okPressed) {
    handleOKButton();
    okPressed = true;
  } else if (okState == HIGH) {
    okPressed = false;
  }

  if (upState == LOW && !upPressed) {
    handleUpButton();
    upPressed = true;
  } else if (upState == HIGH) {
    upPressed = false;
  }

  if (downState == LOW && !downPressed) {
    handleDownButton();
    downPressed = true;
  } else if (downState == HIGH) {
    downPressed = false;
  }
}

// Ham xu ly nut OK
void handleOKButton() {
  if (currentScreen == 1) {
    // Menu chon cay
    currentPlantIndex = menuIndex;
    EEPROM.update(EEPROM_PLANT_INDEX, currentPlantIndex);
    if (menuIndex < 5) {
      currentScreen = 0;
      showSaveMessage();
    } else {
      currentScreen = 2; // Cai dat do am tuy chinh
    }
    updateLEDColor();
  } else if (currentScreen == 2) {
    // Luu do am tuy chinh
    EEPROM.update(EEPROM_CUSTOM_MOISTURE, customMoisture);
    currentScreen = 3; // Chuyen sang cai dat lich tuoi
    menuIndex = 0;
    scheduleEditMode = 0; // Bat dau voi chinh gio
    showSaveMessage();
  } else if (currentScreen == 3) {
    // Chuyen doi giua chinh gio va phut
    if (scheduleEditMode == 0) {
      scheduleEditMode = 1; // Chuyen sang chinh phut
    } else {
      // Luu lich tuoi nuoc
      EEPROM.put(EEPROM_SCHEDULE_BASE + menuIndex * sizeof(WateringTime), schedules[menuIndex]);
      scheduleEditMode = 0; // Dat lai ve chinh gio
      if (menuIndex < 2) {
        menuIndex++;
      } else {
        currentScreen = 4; // Chuyen sang cai dat mau LED
      }
      showSaveMessage();
    }
  } else if (currentScreen == 4) {
    // Luu mau LED tuy chinh
    EEPROM.put(EEPROM_CUSTOM_COLOR, customColor);
    currentScreen = 0;
    updateLEDColor();
    showSaveMessage();
  }
}

// Ham xu ly nut Len
void handleUpButton() {
  if (currentScreen == 1) {
    menuIndex = (menuIndex > 0) ? menuIndex - 1 : 5;
  } else if (currentScreen == 2) {
    customMoisture = min(customMoisture + 5, 100);
  } else if (currentScreen == 3) {
    if (schedules[menuIndex].enabled) {
      if (scheduleEditMode == 0) {
        schedules[menuIndex].hour = (schedules[menuIndex].hour + 1) % 24;
      } else {
        schedules[menuIndex].minute = (schedules[menuIndex].minute + 5) % 60; // Tang 5 phut
      }
    } else {
      schedules[menuIndex].enabled = true;
    }
  } else if (currentScreen == 4) {
    // Chuyen doi giua cac mau
    static uint32_t colors[] = {
      pixels.Color(255, 0, 0),    // Do
      pixels.Color(0, 255, 0),    // Xanh
      pixels.Color(0, 0, 255),    // Duong
      pixels.Color(255, 255, 0),  // Vang
      pixels.Color(255, 0, 255),  // Tim
      pixels.Color(255, 255, 255) // Trang
    };
    static int colorIndex = 0;
    colorIndex = (colorIndex + 1) % 6;
    customColor = colors[colorIndex];
    updateLEDColor();
  }
}

// Ham xu ly nut Xuong
void handleDownButton() {
  if (currentScreen == 1) {
    menuIndex = (menuIndex < 5) ? menuIndex + 1 : 0;
  } else if (currentScreen == 2) {
    customMoisture = max(customMoisture - 5, 0);
  } else if (currentScreen == 3) {
    if (schedules[menuIndex].enabled) {
      if (scheduleEditMode == 0) {
        schedules[menuIndex].hour = (schedules[menuIndex].hour - 1 + 24) % 24;
      } else {
        schedules[menuIndex].minute = (schedules[menuIndex].minute - 5 + 60) % 60; // Giam 5 phut
      }
    } else {
      schedules[menuIndex].enabled = true;
    }
  } else if (currentScreen == 4) {
    // Chuyen doi giua cac mau
    static uint32_t colors[] = {
      pixels.Color(255, 0, 0),    // Do
      pixels.Color(0, 255, 0),    // Xanh
      pixels.Color(0, 0, 255),    // Duong
      pixels.Color(255, 255, 0),  // Vang
      pixels.Color(255, 0, 255),  // Tim
      pixels.Color(255, 255, 255) // Trang
    };
    static int colorIndex = 0;
    colorIndex = (colorIndex - 1 + 6) % 6;
    customColor = colors[colorIndex];
    updateLEDColor();
  }
}

// Ham cap nhat man hinh
void updateDisplay(float temp, float hum, int soil, DateTime now) {
  // Kiem tra su thay doi du lieu hoac man hinh
  bool needClear = (currentScreen != lastScreen); // Chi xoa man hinh khi chuyen man hinh
  bool needUpdate = needClear || 
                   (currentScreen == 0 && 
                    (abs(temp - lastTemp) >= 0.1 || abs(hum - lastHum) >= 0.5 || 
                     abs(soil - lastSoil) >= 1 || now.hour() != lastHour || now.minute() != lastMinute)) ||
                   (currentScreen == 1 && menuIndex != lastMenuIndex) ||
                   (currentScreen == 3 && (schedules[menuIndex].hour != lastHour || 
                                          schedules[menuIndex].minute != lastMinute || 
                                          scheduleEditMode != 0));

  if (!needUpdate) return;

  // Cap nhat gia tri cu
  lastTemp = temp;
  lastHum = hum;
  lastSoil = soil;
  lastHour = now.hour();
  lastMinute = now.minute();
  lastScreen = currentScreen;
  lastMenuIndex = menuIndex;

  // Chi xoa man hinh khi can thiet
  if (needClear) {
    lcd.clear();
  }

  if (currentScreen == 0) {
    // Man hinh chinh: hien thi nhiet do, do am, do am dat, thoi gian
    // Dong 1: T:25.5C H:60% (toi da 13 ky tu)
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp, 1); // Hien thi nhiet do voi 1 chu so thap phan
    lcd.print("C H:");
    lcd.print((int)hum); // Hien thi do am la so nguyen
    lcd.print("%");
    
    // Dong 2: Dat:50% 14:30 (toi da 14 ky tu)
    lcd.setCursor(0, 1);
    lcd.print("Dat:");
    lcd.print(soil);
    lcd.print("% ");
    if (now.hour() < 10) lcd.print("0");
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10) lcd.print("0");
    lcd.print(now.minute());
  } else if (currentScreen == 1) {
    // Menu chon cay
    if (needClear) {
      lcd.setCursor(0, 0);
      lcd.print("Chon cay:");
    }
    lcd.setCursor(0, 1);
    lcd.print("                "); // Xoa dong 2
    lcd.setCursor(0, 1);
    lcd.print(plants[menuIndex].name);
  } else if (currentScreen == 2) {
    // Cai dat do am tuy chinh
    lcd.setCursor(0, 0);
    lcd.print("Do am dat:");
    lcd.setCursor(0, 1);
    lcd.print(customMoisture);
    lcd.print("%");
  } else if (currentScreen == 3) {
    // Cai dat lich tuoi nuoc
    lcd.setCursor(0, 0);
    lcd.print("Lich ");
    lcd.print(menuIndex + 1);
    lcd.print(":");
    lcd.setCursor(0, 1);
    if (schedules[menuIndex].enabled) {
      if (schedules[menuIndex].hour < 10) lcd.print("0");
      lcd.print(schedules[menuIndex].hour);
      lcd.print(":");
      if (schedules[menuIndex].minute < 10) lcd.print("0");
      lcd.print(schedules[menuIndex].minute);
    } else {
      lcd.print("Tat");
    }
  } else if (currentScreen == 4) {
    // Cai dat mau LED
    lcd.setCursor(0, 0);
    lcd.print("Mau LED:");
    lcd.setCursor(0, 1);
    lcd.print(getColorName(customColor));
  } else if (currentScreen == 5) {
    // Thong tin cay
    lcd.setCursor(0, 0);
    lcd.print(plants[currentPlantIndex].name);
    lcd.setCursor(0, 1);
    lcd.print("Dat:");
    lcd.print(getMoistureThreshold());
    lcd.print("% ");
    lcd.print(getColorName(getCurrentColor()));
  }
}

// Ham dieu khien bom nuoc
void controlPump(int soilMoisture) {
  int threshold = getMoistureThreshold();
  if (soilMoisture < threshold) {
    digitalWrite(RELAY_PIN, LOW); // Bat bom
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Tat bom
  }
}

// Ham kiem tra lich tuoi nuoc
void checkWateringSchedule(DateTime now) {
  for (int i = 0; i < 3; i++) {
    if (schedules[i].enabled && 
        now.hour() == schedules[i].hour && 
        now.minute() == schedules[i].minute) {
      digitalWrite(RELAY_PIN, LOW); // Bat bom
      delay(30000); // Bom nuoc trong 30 giay
      digitalWrite(RELAY_PIN, HIGH); // Tat bom
    }
  }
}

// Ham cap nhat mau LED
void updateLEDColor() {
  uint32_t color = getCurrentColor();
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}

// Ham lay mau LED hien tai
uint32_t getCurrentColor() {
  return (currentPlantIndex == 5) ? customColor : plants[currentPlantIndex].ledColor;
}

// Ham lay nguong do am dat
int getMoistureThreshold() {
  return (currentPlantIndex == 5) ? customMoisture : plants[currentPlantIndex].moistureThreshold;
}

// Ham lay ten mau LED
const char* getColorName(uint32_t color) {
  if (color == pixels.Color(255, 0, 0)) return "Do";
  if (color == pixels.Color(0, 255, 0)) return "Xanh";
  if (color == pixels.Color(0, 0, 255)) return "Duong";
  if (color == pixels.Color(255, 255, 0)) return "Vang";
  if (color == pixels.Color(255, 0, 255)) return "Tim";
  return "Trang";
}

// Ham hien thi thong bao luu cai dat
void showSaveMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luu thanh cong!");
  delay(1000);
}