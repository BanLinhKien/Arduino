

/* =================================================================================================*
 *                                          BANLINHKIEN.COM                                         *
 *==================================================================================================*                        
 *                 	   PRJ04: DIY hệ thống cảnh báo ngập úng, lũ lụt
 * 
 * Đấu nối: 
 * Arduino UNO R3 vs LCD 16x2 I2C
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 * Arduino UNO R3 vs Module DS1307
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 * Arduino UNO R3 vs Module SIM A7680C
 * 5V <-> VCC  ;   GND <-> GND  ;   D10 <-> TX  ;   D11 <-> RX
 * Arduino UNO R3 vs Cảm biến siêu âm SRF04
 * 5V <-> VCC  ;   GND <-> GND  ;   D3 <-> TRIG ;   D2 <-> ECHO
 */
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define SIREN_PIN 6
#define SIM_TX 10
#define SIM_RX 11
#define HOUR_SEND 19
#define MINUTE_SEND 00

// Địa chỉ EEPROM
#define ADDR_MIN_DISTANCE 0             // 4 bytes cho float
#define ADDR_MAX_DISTANCE 4             // 4 bytes cho float
#define ADDR_LAST_DAY 8                 // 1 byte cho ngày

RTC_DS3231 rtc;
SoftwareSerial simSerial(SIM_RX, SIM_TX);
LiquidCrystal_I2C lcd(0x27, 16, 2);     // Địa chỉ I2C LCD, thường là 0x27 hoặc 0x3F

float danger_distance = 10.0;           // Ngưỡng khoảng cách nguy hiểm (cm)
float hysteresis_on = 2.0;              // Độ trễ bật cảnh báo (cm)
float hysteresis_off = 5.0;             // Độ trễ tắt cảnh báo (cm)
bool alerting = false;
float min_distance = 1000.0;            // Khoảng cách nhỏ nhất (mực nước cao nhất)
float max_distance = 0.0;               // Khoảng cách lớn nhất (mực nước thấp nhất)
String phone_number = "03xxxxxxxx";     // Số điện thoại nhận cảnh báo
#define MCU_SIM_BAUDRATE 9600           // Baud rate cho SIM A7680X
#define SET_BAUDRATE "AT+IPREX=9600"    // Cài đặt baud rate
bool report_sent = false;

// Biến cho tính trung bình
float distance_samples[10];             // Mảng lưu 10 mẫu khoảng cách
int sample_index = 0;                   // Chỉ số mẫu hiện tại
float average_distance = 0.0;           // Trung bình khoảng cách
bool samples_filled = false;            // Kiểm tra mảng mẫu đã đầy chưa

void setup() {
  Serial.begin(MCU_SIM_BAUDRATE);
  simSerial.begin(MCU_SIM_BAUDRATE);
  delay(200);

  // Khởi tạo SIM module
  sim_at_cmd("AT");                     // Kiểm tra AT Command
  sim_at_cmd("AT+IPREX?");              // Kiểm tra tốc độ baud rate
  sim_at_cmd(SET_BAUDRATE);             // Cài tốc độ baud rate
  sim_at_cmd("ATI");                    // Thông tin module SIM
  sim_at_cmd("AT+CPIN?");               // Kiểm tra trạng thái thẻ SIM
  sim_at_cmd("AT+CSQ");                 // Kiểm tra tín hiệu mạng
  sim_at_cmd("AT+CIMI");                // Kiểm tra IMSI
  sim_at_cmd("AT+CMGF=1");              // Khởi động chức năng SMS
  sim_at_cmd("AT+CNMI=1,2,0,0,0");      // Cấu hình nhận tin nhắn

  if (!rtc.begin()) {
    Serial.println("Không tìm thấy RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC mất nguồn, đặt thời gian ban đầu");
    rtc.adjust(DateTime(2023, 1, 1, 0, 0, 0)); // Đặt thời gian mặc định
  }
  Serial.println("Nhập thời gian (YYYY-MM-DD HH:MM:SS) để cập nhật RTC");

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Canh Bao Lu");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Khoi Dong");
  delay(500);
  for (int i = 0; i < 7; i++) {
    lcd.setCursor(9 + i, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SIREN_PIN, OUTPUT);
  digitalWrite(SIREN_PIN, HIGH);

  // Khởi tạo mảng mẫu
  for (int i = 0; i < 10; i++) {
    distance_samples[i] = 0.0;
  }

  // Khôi phục dữ liệu từ EEPROM
  min_distance = readFloatFromEEPROM(ADDR_MIN_DISTANCE);
  max_distance = readFloatFromEEPROM(ADDR_MAX_DISTANCE);
  if (min_distance > 1000.0 || max_distance < 0.0) { // Kiểm tra giá trị hợp lệ
    min_distance = 1000.0;
    max_distance = 0.0;
    writeFloatToEEPROM(ADDR_MIN_DISTANCE, min_distance);
    writeFloatToEEPROM(ADDR_MAX_DISTANCE, max_distance);
  }
}

void loop() {
  DateTime now = rtc.now();
  float distance = measureDistance();

  // Kiểm tra và cập nhật thời gian từ Serial
  checkSerialForTimeUpdate();
  
  // Cập nhật mẫu và tính trung bình
  distance_samples[sample_index] = distance;
  sample_index = (sample_index + 1) % 10; // Vòng lặp chỉ số
  if (sample_index == 0) samples_filled = true; // Mảng đầy sau 10 mẫu
  average_distance = 0.0;
  int sample_count = samples_filled ? 10 : sample_index;
  if (sample_count > 0) {
    for (int i = 0; i < sample_count; i++) {
      average_distance += distance_samples[i];
    }
    average_distance /= sample_count;
  }

  // Cập nhật khoảng cách min và max dựa trên khoảng cách hiện tại
  if (distance < min_distance) {
    min_distance = distance;
    writeFloatToEEPROM(ADDR_MIN_DISTANCE, min_distance);
  }
  if (distance > max_distance) {
    max_distance = distance;
    writeFloatToEEPROM(ADDR_MAX_DISTANCE, max_distance);
  }

  // Xóa dữ liệu EEPROM và đặt lại min/max khi sang ngày mới (sau báo cáo 20:00)
  byte last_day = EEPROM.read(ADDR_LAST_DAY);
  if (now.day() != last_day && now.hour() >= 20) {
    min_distance = distance;
    max_distance = distance;
    writeFloatToEEPROM(ADDR_MIN_DISTANCE, min_distance);
    writeFloatToEEPROM(ADDR_MAX_DISTANCE, max_distance);
    EEPROM.write(ADDR_LAST_DAY, (byte)now.day());
  }

  // Gửi báo cáo hàng ngày lúc 20:00
  if (now.hour() == HOUR_SEND && now.minute() == MINUTE_SEND && now.second() == 00 && !report_sent) {
    Serial.println("Thực hiện gửi tin nhắn báo nước theo ngày ");
    String message1 = "Bao cao muc nuoc ngay " + String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + ": Muc cao nhat: " + String(min_distance) + " cm, Muc thap nhat: " + String(max_distance) + " cm.";
    Serial.println(message1);
    sendSMS(message1);
    delay(200);
    report_sent = true;
    EEPROM.write(ADDR_LAST_DAY, (byte)now.day()); // Cập nhật ngày báo cáo
  }
  else if (now.hour() != 20) {
    report_sent = false;
  }

  // Logic cảnh báo dựa trên trung bình
  if (samples_filled && !alerting && average_distance < danger_distance - hysteresis_on) {
    alerting = true;
    String message2 = "Canh bao: Muc nuoc cao luc " + String(now.hour()) + ":" + String(now.minute()) + " ngay " + String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + ". Muc nuoc hien tai: " + String(average_distance) + " cm";
    digitalWrite(SIREN_PIN, LOW);
    Serial.println(message2);
    sendSMS(message2);
    makeCall();
    
  }
  else if (alerting && average_distance > danger_distance + hysteresis_off) {
    alerting = false;
    digitalWrite(SIREN_PIN, HIGH);
  }

  // Hiển thị thời gian, khoảng cách, trung bình hoặc cảnh báo lên LCD và Serial
  displayData(now, distance, average_distance);

  delay(1000); // Đo mỗi giây
}

float measureDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // Tính khoảng cách dựa trên khoảng cách âm thanh
  return distance;
}

void checkSerialForTimeUpdate() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() == 19 && input[4] == '-' && input[7] == '-' && input[10] == ' ' && input[13] == ':' && input[16] == ':') {
      int year = input.substring(0, 4).toInt();
      int month = input.substring(5, 7).toInt();
      int day = input.substring(8, 10).toInt();
      int hour = input.substring(11, 13).toInt();
      int minute = input.substring(14, 16).toInt();
      int second = input.substring(17, 19).toInt();
      
      if (year >= 2000 && year <= 2099 && month >= 1 && month <= 12 && day >= 1 && day <= 31 &&
          hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 && second >= 0 && second <= 59) {
        rtc.adjust(DateTime(year, month, day, hour, minute, second));
        Serial.println("Thời gian RTC đã được cập nhật: " + input);
      } else {
        Serial.println("Định dạng thời gian không hợp lệ. Vui lòng nhập: YYYY-MM-DD HH:MM:SS");
      }
    } else {
      Serial.println("Định dạng thời gian không đúng. Vui lòng nhập: YYYY-MM-DD HH:MM:SS");
    }
  }
}

void sim_at_wait() {
  delay(100);
  while (simSerial.available()) {
    Serial.write(simSerial.read());
  }
}

bool sim_at_cmd(String cmd) {
  simSerial.println(cmd);
  sim_at_wait();
  return true;
  delay(100);
}

bool sim_at_send(char c) {
  simSerial.write(c);
  return true;
}

void sendSMS(String message) {
  Serial.println("Thực hiện gửi tin nhắn ");
  sim_at_cmd("AT");
  sim_at_cmd("AT+CSQ");       // Kiểm tra tín hiệu mạng
  sim_at_cmd("AT+CMGF=1");
  sim_at_cmd("AT+CSCS=\"GSM\"");
  String temp = "AT+CMGS=\"" + phone_number + "\"\r";
  sim_at_cmd(temp);
  sim_at_cmd(message);
  sim_at_send(0x1A); // Ký tự kết thúc SMS
  delay(1000);
}

void makeCall() {
  Serial.println("Thực hiện cuộc gọi ");
  String temp = "ATD" + phone_number + ";";
  sim_at_cmd(temp);
  delay(20000); // Gọi trong 20 giây
  sim_at_cmd("ATH"); // Ngắt cuộc gọi
  delay(1000);
}

void sendDailyReport(DateTime now) {
  Serial.println("Thực hiện gửi tin nhắn báo nước theo ngày ");
  String message = "Bao cao muc nuoc ngay " + String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + ": Muc cao nhat (k/c min): " + String(min_distance) + " cm, Muc thap nhat (k/c max): " + String(max_distance) + " cm.";
  sendSMS(message);
}

void displayData(DateTime now, float distance, float avg_distance) {
  // In lên Serial Monitor
  Serial.print("Thoi gian: ");
  Serial.print(now.hour());
  Serial.print(":");
  if (now.minute() < 10) Serial.print("0");
  Serial.print(now.minute());
  Serial.print(" ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.year());
  Serial.print(" | Khoang cach: ");
  Serial.print(distance);
  Serial.print(" cm | Trung binh: ");
  Serial.print(avg_distance);
  Serial.println(" cm");

  // Hiển thị lên LCD
  lcd.clear();
  if (alerting) {
    lcd.setCursor(0, 0);
    lcd.print("CANH BAO LU LUT!");
    lcd.setCursor(0, 1);
    lcd.print("Muc: ");
    lcd.print(avg_distance);
    lcd.print(" cm");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Gio: ");
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10) lcd.print("0");
    lcd.print(now.minute());
    lcd.print(":");
    if (now.second() < 10) lcd.print("0");
    lcd.print(now.second());
    lcd.setCursor(0, 1);
    lcd.print("Muc: ");
    lcd.print(avg_distance);
    lcd.print(" cm");
  }
}

void writeFloatToEEPROM(int address, float value) {
  byte* p = (byte*)&value;
  for (int i = 0; i < sizeof(float); i++) {
    EEPROM.write(address + i, p[i]);
  }
}

float readFloatFromEEPROM(int address) {
  float value;
  byte* p = (byte*)&value;
  for (int i = 0; i < sizeof(float); i++) {
    p[i] = EEPROM.read(address + i);
  }
  return value;
}
