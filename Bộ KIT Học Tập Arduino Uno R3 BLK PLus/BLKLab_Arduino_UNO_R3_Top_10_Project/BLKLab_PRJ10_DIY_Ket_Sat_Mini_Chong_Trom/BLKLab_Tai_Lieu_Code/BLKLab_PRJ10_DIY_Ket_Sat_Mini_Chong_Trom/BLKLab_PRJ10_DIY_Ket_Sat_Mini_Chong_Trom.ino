
/*=========================================================================
 *                      BANLINHKIEN.COM                                   *
 *=========================================================================                     
 *                  DIY KÉT SẮT MINI CHỐNG TRỘM                           *
 *=========================================================================
 */
#include <Keypad.h>                         // Thư viện xử lý bàn phím ma trận 4x4
#include <Servo.h>                          // Thư viện điều khiển động cơ Servo
#include <EEPROM.h>                         // Thư viện ghi/đọc bộ nhớ vĩnh cửu
#include <Wire.h>                           // Thư viện giao tiếp I2C
#include <RTClib.h>                         // Thư viện đồng hồ thời gian thực (RTC DS1307/DS3231)
#include <LiquidCrystal_I2C.h>              // Thư viện màn hình LCD giao tiếp I2C
#include <HCSR04.h>                         // Thư viện cảm biến siêu âm (đo khoảng cách cửa)

// ================================================================
// PHẦN 1: CẤU HÌNH HỆ THỐNG (CONFIGURATION)
// ================================================================

// --- HARDWARE PINS (Định nghĩa chân kết nối) ---
const int SERVO_PIN     = 12;               // Chân điều khiển Servo (Khóa/Mở chốt)
const int LED_PIN       = 11;               // Đèn LED trạng thái/báo động
const int BUZZER_PIN    = 10;               // Còi chíp báo động (Active Buzzer)
const int VIBRATION_PIN = 13;               // Cảm biến rung (phát hiện đập phá)
const int TRIG_PIN      = A2;               // Chân phát sóng siêu âm
const int ECHO_PIN      = A3;               // Chân thu sóng siêu âm

// --- CONFIG (Cấu hình mật khẩu) ---
const String MASTER_PASSWORD  = "2026";     // Mật khẩu chủ (Dùng để cứu hộ)
const String DEFAULT_PASSWORD = "1234";     // Mật khẩu mặc định nếu EEPROM chưa có dữ liệu

// --- EEPROM MAP (Bản đồ bộ nhớ) ---
// Dùng để định vị dữ liệu được lưu ở đâu trong bộ nhớ
const int ADDR_PASS_LEN      = 0;           // Địa chỉ lưu độ dài mật khẩu hiện tại
const int ADDR_PASS_VAL      = 1;           // Địa chỉ bắt đầu lưu chuỗi mật khẩu
const int ADDR_HISTORY_START = 50;          // Địa chỉ bắt đầu vùng nhớ lưu lịch sử
const int HISTORY_MSG_LEN    = 12;          // Độ dài tối đa của một dòng lịch sử (VD: "15/01 10:30")

// --- TIMING (Cấu hình thời gian) ---
const unsigned long PHASE_1_OPEN  = 10000;  // 10 giây đầu: Thời gian chờ người dùng mở cánh cửa ra
const unsigned long PHASE_2_WORK  = 60000;  // 60 giây sau: Thời gian cho phép két mở để làm việc
const int CLOSE_THRESHOLD = 07;             // Ngưỡng khoảng cách (cm). Dưới mức này coi như cửa đã đóng.
const unsigned long MENU_TIMEOUT  = 15000;  // Thời gian tự thoát menu (đổi pass/xem lịch sử) nếu không bấm gì

// ================================================================
// PHẦN 2: KHỞI TẠO ĐỐI TƯỢNG (OBJECT INITIALIZATION)
// ================================================================

Servo myServo;                              // Tạo đối tượng Servo
RTC_DS1307 rtc;                             // Tạo đối tượng đồng hồ RTC
LiquidCrystal_I2C lcd(0x27, 16, 2);         // Tạo đối tượng LCD (Địa chỉ I2C thường là 0x27 hoặc 0x3F)
HCSR04 hc(TRIG_PIN, ECHO_PIN);              // Tạo đối tượng cảm biến siêu âm

// --- KEYPAD CONFIG (Cấu hình bàn phím) ---
const byte ROWS = 4; // 4 hàng
const byte COLS = 4; // 4 cột
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'}, // #: Enter
  {'4','5','6','B'}, // *: Xóa 
  {'7','8','9','C'}, // C: Xóa hết
  {'*','0','#','D'}  // Mật khẩu chỉ chứa số
};
byte rowPins[ROWS] = {9, 8, 7, 6};          // Chân Arduino nối với hàng của Keypad
byte colPins[COLS] = {5, 4, 3, 2};          // Chân Arduino nối với cột của Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// ================================================================
// PHẦN 3: BIẾN TOÀN CỤC (GLOBAL VARIABLES)
// ================================================================

// -- Biến quản lý trạng thái khóa --
String inputPassword = "";                  // Chuỗi lưu mật khẩu người dùng đang nhập
String currentPassword = "";                // Mật khẩu hiện tại (đọc từ EEPROM)
bool isLocked = true;                       // Trạng thái: true = Đang khóa, false = Đang mở
int wrongAttempts = 0;                      // Đếm số lần nhập sai liên tiếp
String history[5];                          // Mảng lưu 5 lịch sử mở gần nhất

// -- Biến quản lý cảm biến & thời gian --
int distance = 0;                           // Khoảng cách đo được từ cảm biến siêu âm
unsigned long unlockTime = 0;               // Mốc thời gian (millis) khi bắt đầu mở khóa
unsigned long lastTimeUpdate = 0;           // Biến phụ để cập nhật màn hình mỗi giây (tránh nháy LCD)
unsigned long lastSensorRead = 0;           // Biến phụ để giới hạn tốc độ đọc cảm biến (tránh nhiễu)

// -- Biến quản lý báo động --
bool isAlarming = false;                    // Cờ báo động: true = Đang kêu
unsigned long alarmStartTime = 0;           // Thời điểm bắt đầu báo động
String alarmReason = "";                    // Lưu lý do báo động để hiển thị lên LCD

// -- Khai báo hàm  --
void clearI2CBus();                         // Hàm xóa lỗi bus I2C (quan trọng để tránh treo LCD khi khởi động lại)

// ================================================================
// PHẦN 4: SETUP 
// ================================================================
void setup() {
  Serial.begin(9600);
  clearI2CBus(); // Thực hiện xóa lỗi xung I2C trước khi khởi tạo thiết bị
  
  lcd.init(); lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("SYSTEM BOOT v2.0");
  
  // --- RTC RETRY (Cơ chế thử lại RTC) ---
  // Nếu RTC lỏng pin hoặc lỗi, hệ thống sẽ thử kết nối lại 5 lần
  bool rtcStatus = false;
  for (int i = 0; i < 5; i++) {
    if (rtc.begin()) { rtcStatus = true; break; }
    lcd.setCursor(0, 1); lcd.print("Retry RTC... "); lcd.print(i + 1);
    delay(300); 
    if(i == 2) clearI2CBus(); // Nếu thử 2 lần không được thì reset bus I2C lần nữa
  }

  if (!rtcStatus) { 
    lcd.clear(); lcd.print("ERR: RTC FAILURE"); while (1); // Treo hệ thống nếu RTC hỏng hẳn
  }
  if (!rtc.isrunning()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Cập nhật giờ từ máy tính nạp code

  // --- IO CONFIG (Cấu hình chân vào/ra) ---
  myServo.attach(SERVO_PIN);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT); 
  pinMode(VIBRATION_PIN, INPUT); 

  // --- LOAD DATA (Tải dữ liệu) ---
  loadSystemData();   // Đọc mật khẩu từ EEPROM
  loadHistoryData();  // Đọc lịch sử từ EEPROM

  lockSafe(); // Khởi động xong thì đưa về trạng thái KHÓA ngay lập tức
  lcd.clear();
}

// ================================================================
// PHẦN 5: LOOP (VÒNG LẶP CHÍNH)
// ================================================================
void loop() {
  // Kiến trúc "Round Robin": Thực hiện lần lượt các tác vụ
  system_run_logic();     // 1. Xử lý logic đóng/mở/báo động
  system_run_keypad();    // 2. Quét bàn phím và xử lý lệnh
  checkVibrationSensor(); // 3. Quét cảm biến rung
}

// ================================================================
// PHẦN 6: LOGIC HỆ THỐNG (UPDATED & QUAN TRỌNG NHẤT)
// ================================================================

void system_run_logic() {
  // 1. Ưu tiên cao nhất: Nếu đang báo động thì xử lý báo động rồi thoát, không làm gì khác
  if (isAlarming) { handleAlarmState(); return; }

  // 2. Logic khi hệ thống ĐANG MỞ (Unlock Logic)
  if (!isLocked) {
      unsigned long timePassed = millis() - unlockTime; // Tính thời gian đã trôi qua kể từ khi mở

      // --- GIAI ĐOẠN 1: MỞ CỬA TỰ DO (10s đầu tiên) ---
      // Người dùng có 8s để kéo cánh cửa ra.
      if (timePassed < PHASE_1_OPEN) {
          if (millis() - lastSensorRead > 200) { 
              readDistance(); lastSensorRead = millis();
              // Nếu khoảng cách > ngưỡng (tức là cửa đã mở ra thật) -> Reset bộ đếm để giữ cửa mở
              if (distance > CLOSE_THRESHOLD) { 
                  unlockTime = millis() - PHASE_1_OPEN; // Nhảy qua giai đoạn 1, sang giai đoạn 2
                  lcd.clear(); return; 
              }
          }
          // Hiển thị đếm ngược thời gian chờ mở cửa
          if(millis() - lastTimeUpdate >= 1000) {
              lcd.setCursor(0, 0); lcd.print("MO KHOA THANH CONG");
              lcd.setCursor(0, 1); lcd.print("MOI MO CUA: ");
              lcd.print((PHASE_1_OPEN - timePassed)/1000); lcd.print("s ");
              lastTimeUpdate = millis();
          }
      } 
      // --- GIAI ĐOẠN 2: CHẾ ĐỘ LÀM VIỆC (60s tiếp theo) ---
      // Cửa đang mở để người dùng thao tác lấy đồ.
      else if (timePassed < (PHASE_1_OPEN + PHASE_2_WORK)) {
          long timeLeft = (PHASE_1_OPEN + PHASE_2_WORK - timePassed) / 1000;

          if(millis() - lastTimeUpdate >= 500) { // Cập nhật màn hình mỗi 0.5s
              lcd.setCursor(0, 0); 
              
              // === TÍNH NĂNG MỚI: CẢNH BÁO 15 GIÂY CUỐI ===
              if (timeLeft < 15) {
                 lcd.print("!! SAP HET GIO !!");
                 // Kêu bíp ngắn cảnh báo 
                 if (millis() % 1000 < 200) digitalWrite(BUZZER_PIN, HIGH);
                 else digitalWrite(BUZZER_PIN, LOW);
              }
              else {
                 lcd.print(" CHE DO MO ");
                 if(timeLeft < 10) lcd.print("0"); 
                 lcd.print(timeLeft);
                 lastTimeUpdate = millis();
                 digitalWrite(BUZZER_PIN, LOW); // Đảm bảo tắt còi nếu > 15s
              }
              
              lcd.setCursor(0, 1); 
              lcd.print("D:DOIMK B:DATA ");
          }
          
          // Tự động đóng: Nếu cửa đóng lại (khoảng cách nhỏ) -> Khóa ngay lập tức
          if (millis() - lastSensorRead > 200) {
              readDistance(); lastSensorRead = millis();
              if (distance < CLOSE_THRESHOLD) performLockSequence();
          }
      }
      // --- GIAI ĐOẠN 3: QUÊN ĐÓNG CỬA (Quá giờ quy định) ---
      else {
          readDistance(); 
          // Nếu cửa đã đóng thì khóa ngay
          if (distance < CLOSE_THRESHOLD) performLockSequence(); 
          // Nếu cửa vẫn mở -> Kêu inh ỏi nhắc nhở
          else {
              beepWarning(); 
              lcd.setCursor(0, 0); lcd.print("!! CANH BAO !!  ");
              lcd.setCursor(0, 1); lcd.print("QUEN DONG CUA?  ");
          }
      }
  } 
  // 3. Logic khi hệ thống ĐANG KHÓA (Locked Logic) - QUAN TRỌNG: CHECK CẠY CỬA
  else {
      // === QUÉT CẢM BIẾN KHI ĐANG KHÓA ===
      // Chức năng này phát hiện nếu cửa bị nạy ra (khoảng cách tăng lên) dù chốt servo đang khóa
      if (millis() - lastSensorRead > 200) {
          readDistance(); 
          lastSensorRead = millis();
          
          // Logic phát hiện cạy cửa: Đang khóa (IsLocked=true) mà cửa hở > 10cm
          if (distance > CLOSE_THRESHOLD) {
              isAlarming = true;
              alarmStartTime = millis();
              alarmReason = "!! CUA BI CAY !!"; // Ghi nhận lý do
              lcd.clear();
              lcd.print(alarmReason);
              return; 
          }
      }

      // Hiển thị màn hình chờ (Đồng hồ & Trạng thái)
      if (inputPassword.length() == 0 && millis() - lastTimeUpdate > 1000) {
        displayIdleScreen();
        lastTimeUpdate = millis();
      }
  }
}

// ================================================================
// PHẦN 7: KEYPAD & SENSORS (XỬ LÝ ĐẦU VÀO)
// ================================================================

void system_run_keypad() {
  char key = customKeypad.getKey(); // Đọc phím nhấn
  
  if (key) {
    beep(); // Kêu tít mỗi khi bấm phím
    
    // Nếu cửa đang mở, bấm phím bất kỳ sẽ reset bộ đếm thời gian (Gia hạn thời gian mở)
    if (!isLocked) {
        unlockTime = millis() - PHASE_1_OPEN; 
        lcd.clear(); digitalWrite(LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, LOW); // Tắt còi cảnh báo 15s nếu đang kêu
    }
    
    // Nếu đang báo động, bấm phím bất kỳ để tắt báo động
    if (isAlarming) { stopAlarm(); return; }

    switch (key) {
      case 'B': return; 
      case 'D': return;
      case 'A': return;
      case 'C': // Phím xóa
        if (isLocked) { inputPassword = ""; lcd.clear(); lcd.print("DA XOA HET!"); delay(500); lcd.clear(); }
        break;
      case '*': // Phím Backspace (Xóa 1 ký tự)
        if (isLocked && inputPassword.length() > 0) { 
            inputPassword.remove(inputPassword.length() - 1); 
            // Vẽ lại các dấu sao che mật khẩu
            lcd.setCursor(0, 1); lcd.print("Pass:           "); 
            lcd.setCursor(6, 1); for(int i=0; i<inputPassword.length(); i++) lcd.print("*");
        }
        break;
      case '#': verifyPassword(); break; // Phím Enter: Kiểm tra mật khẩu
      default:  // Các phím số (0-9)
        if (isLocked && inputPassword.length() < 10) { 
            inputPassword += key;
            // Hiển thị dấu * thay vì số thật
            lcd.setCursor(0, 1); lcd.print("Pass: ");
            for(int i=0; i<inputPassword.length(); i++) lcd.print("*");
        }
        break;
    }
  }
}

void checkVibrationSensor() {
    // Chỉ kiểm tra rung khi khóa và chưa có báo động
    if (isLocked && !isAlarming) {
        // Cảm biến rung thường trả về LOW khi có rung động mạnh
        if (digitalRead(VIBRATION_PIN) == LOW) { 
            isAlarming = true; 
            alarmStartTime = millis();
            alarmReason = "!!PHAT HIEN PHA kET";
            lcd.clear(); lcd.print(alarmReason);
        }
    }
}

// Xử lý hiệu ứng khi báo động xảy ra
void handleAlarmState() {
    // Kêu inh ỏi trong 3s giây hoặc đến khi bấm phím
    if (millis() - alarmStartTime < 3000) {
        lcd.setCursor(0, 0); lcd.print("!! CANH BAO !!  ");
        lcd.setCursor(0, 1); lcd.print(alarmReason);
        
        // Hiệu ứng còi cảnh sát (Nháy đèn + Kêu liên tục)
        digitalWrite(BUZZER_PIN, HIGH); digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(BUZZER_PIN, LOW); digitalWrite(LED_PIN, LOW);
        delay(100);
    } else {
        stopAlarm(); // Tự tắt sau 3s (hoặc 10s tùy chỉnh) để tránh ồn quá lâu
    }
}

void stopAlarm() {
    isAlarming = false;
    digitalWrite(BUZZER_PIN, LOW); digitalWrite(LED_PIN, LOW);
    lcd.clear(); lastTimeUpdate = 0; 
    
    // Lưu ý: Nếu báo động do cạy cửa, sau khi tắt, nếu cửa vẫn hở
    // thì vòng lặp sau (system_run_logic) sẽ lại kích hoạt báo động tiếp.
}

// ================================================================
// PHẦN 8: CÁC TIỆN ÍCH 
// ================================================================

void verifyPassword() {
  if (inputPassword.length() == 0) return;
  lcd.clear(); lcd.print("Checking..."); delay(200);
  
  // So sánh mật khẩu nhập vào với mật khẩu lưu trữ HOẶC mật khẩu chủ
  if (inputPassword == currentPassword || inputPassword == MASTER_PASSWORD) {
    lcd.setCursor(0, 1); lcd.print("MAT KHAU DUNG!  "); beepSuccess(); 
    logHistory(); // Lưu thời gian mở vào lịch sử
    unlockSafe(); // Thực hiện mở khóa
  } else {
    lcd.setCursor(0, 1); lcd.print("MAT KHAU SAI!   "); 
    wrongAttempts++; // Tăng số lần sai
    handleWrongAttempts(); // Kiểm tra xem sai quá 3 lần chưa
  }
  inputPassword = ""; delay(1000); lcd.clear();
}

// Chế độ đổi mật khẩu
void changePasswordMode() {
    String newPass = ""; unsigned long lastInput = millis();
    lcd.clear(); lcd.print("NHAP PASS MOI:");
    while (true) {
        if (millis() - lastInput > MENU_TIMEOUT) { lcd.clear(); break; } // Timeout
        char key = customKeypad.getKey(); 
        if(key) {
            beep(); lastInput = millis(); 
            if (key == '#') { // Xác nhận đổi
                if (newPass.length() > 0 && newPass.length() < 10) {
                    currentPassword = newPass; savePasswordToEEPROM(currentPassword); 
                    lcd.clear(); lcd.print("DOI THANH CONG!"); delay(1500); break;
                }
            } 
            else if (key == 'A' || key == 'B') { continue; }
            else if (key == 'D') break; // Thoát
            else if (key == 'C') { newPass = ""; lcd.setCursor(0, 1); lcd.print("                "); } // Xóa
            else if (key == '*') { if(newPass.length()>0) newPass.remove(newPass.length()-1); }
            else if (newPass.length() < 9) newPass += key; // Nhập số
            
            // Hiển thị pass mới (ẩn bằng dấu *)
            lcd.setCursor(0, 1); lcd.print("                ");
            lcd.setCursor(0, 1); for(int i=0; i<newPass.length(); i++) lcd.print("*");
        }
    }
    lcd.clear(); unlockTime = millis() - PHASE_1_OPEN; // Reset thời gian chờ để không bị đóng cửa ngay khi thoát menu
}

// Chế độ xem lịch sử
void viewHistoryMode() {
    int scroll = 0; unsigned long lastInput = millis(); bool viewing = true;
    lcd.clear(); lcd.print(">> LICH SU MO <<"); delay(1000);
    updateHistoryScreen(scroll);
    while (viewing) {
        if (millis() - lastInput > MENU_TIMEOUT) viewing = false;
        char key = customKeypad.getKey();
        if (key) {
             beep(); lastInput = millis();
             if (key == 'B') viewing = false; // Thoát
             else if (key == 'C' && scroll < 3) { scroll++; updateHistoryScreen(scroll); } // Cuộn xuống
             else if (key == 'A' && scroll > 0) { scroll--; updateHistoryScreen(scroll); } // Cuộn lên
        }
    }
    lcd.clear(); if (!isLocked) unlockTime = millis() - PHASE_1_OPEN; 
}

void updateHistoryScreen(int index) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print(index + 1); lcd.print("."); lcd.print(history[index]);
    if (index + 1 < 5) {
        lcd.setCursor(0, 1); lcd.print(index + 2); lcd.print("."); lcd.print(history[index + 1]);
    }
}

// Hàm lưu thời gian thực vào mảng lịch sử
void logHistory() {
    DateTime now = rtc.now();
    String timeStr = "";
    // Định dạng chuỗi: DD/MM HH:mm
    if(now.day()<10) timeStr += "0"; timeStr += String(now.day()) + "/";
    if(now.month()<10) timeStr += "0"; timeStr += String(now.month()) + " ";
    if(now.hour()<10) timeStr += "0"; timeStr += String(now.hour()) + ":";
    if(now.minute()<10) timeStr += "0"; timeStr += String(now.minute());

    // Đẩy lịch sử cũ xuống (Shift array)
    for (int i = 4; i > 0; i--) { history[i] = history[i-1]; }
    history[0] = timeStr; // Lưu cái mới nhất vào đầu
    saveHistoryToEEPROM(); // Lưu ngay vào EEPROM
}

// ================================================================
// PHẦN 9: CÁC HÀM HỖ TRỢ
// ================================================================

// Chỉ ghi EEPROM nếu giá trị thay đổi (Tăng tuổi thọ chip nhớ)
void EEPROM_SmartUpdate(int address, byte value) {
    if (EEPROM.read(address) != value) EEPROM.write(address, value);
}

void savePasswordToEEPROM(String pass) {
    EEPROM_SmartUpdate(ADDR_PASS_LEN, pass.length());
    for (int i = 0; i < pass.length(); i++) EEPROM_SmartUpdate(ADDR_PASS_VAL + i, pass[i]);
}

void saveHistoryToEEPROM() {
    int addr = ADDR_HISTORY_START;
    for(int i=0; i<5; i++) {
        String logEntry = history[i];
        for(int j=0; j < HISTORY_MSG_LEN; j++) EEPROM_SmartUpdate(addr + j, (j < logEntry.length()) ? logEntry[j] : 0);
        addr += HISTORY_MSG_LEN;
    }
}

void loadSystemData() {
    int len = EEPROM.read(ADDR_PASS_LEN);
    // Kiểm tra tính hợp lệ của dữ liệu (tránh đọc rác khi chip mới mua)
    if (len > 0 && len < 10) { 
        currentPassword = "";
        for (int i = 0; i < len; i++) currentPassword += (char)EEPROM.read(ADDR_PASS_VAL + i);
    } else {
        currentPassword = DEFAULT_PASSWORD; savePasswordToEEPROM(currentPassword);
    }
}

void loadHistoryData() {
    int addr = ADDR_HISTORY_START;
    for(int i=0; i<5; i++) {
        String temp = "";
        for(int j=0; j < HISTORY_MSG_LEN; j++) {
            char c = (char)EEPROM.read(addr + j); if (c != 0 && c != 255) temp += c; else break;
        }
        if (temp.length() > 2) history[i] = temp; addr += HISTORY_MSG_LEN;
    }
}

void unlockSafe() {
  if (!isLocked) return;
  myServo.write(90); // Quay servo góc 90 độ để mở chốt
  isLocked = false; isAlarming = false;
  digitalWrite(LED_PIN, HIGH); unlockTime = millis(); wrongAttempts = 0; 
}

void performLockSequence() {
    lcd.clear(); lcd.print("Phat hien dong!"); delay(1000); lockSafe();
}

void lockSafe() {
  myServo.write(0); // Quay servo về 0 độ để đóng chốt
  isLocked = true; beep(); 
  digitalWrite(LED_PIN, LOW); lcd.clear(); lcd.print("KET SAT BLK"); delay(2000);
}

void handleWrongAttempts() {
  for(int i=0; i<3; i++) { beep(); delay(100); } // Kêu 3 tiếng báo sai
  
  // Nếu sai quá 3 lần -> Phạt chờ 20 giây
  if (wrongAttempts >= 3) {
    lcd.clear(); lcd.print("SAI QUA 3 LAN!");
    for(int i=20; i>0; i--) { 
      lcd.setCursor(0, 1); lcd.print("Khoa: "); lcd.print(i); lcd.print("s "); 
      // Nháy đèn còi cảnh báo
      digitalWrite(BUZZER_PIN, HIGH); digitalWrite(LED_PIN, HIGH); delay(25); 
      digitalWrite(BUZZER_PIN, LOW);  digitalWrite(LED_PIN, LOW);  delay(225);
    }
    wrongAttempts = 0; lcd.clear();
  }
}

void readDistance() {
  int cm = hc.dist(); 
  // Lọc nhiễu: Nếu đọc ra 0 (lỗi) coi như không có vật cản gần (999)
  distance = (cm == 0) ? 999 : cm;
}

void displayIdleScreen() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 0); lcd.print("TIME: ");
  // Hiển thị giờ phút giây có số 0 đằng trước nếu < 10
  if(now.hour() < 10) lcd.print('0'); lcd.print(now.hour()); lcd.print(':');
  if(now.minute() < 10) lcd.print('0'); lcd.print(now.minute()); lcd.print(':');
  if(now.second() < 10) lcd.print('0'); lcd.print(now.second());
  lcd.setCursor(0, 1); lcd.print("DA KHOA");
}

void beep() { digitalWrite(BUZZER_PIN, HIGH); delay(50); digitalWrite(BUZZER_PIN, LOW); }
void beepSuccess() {
  for(int i=0; i<2; i++) { digitalWrite(BUZZER_PIN, HIGH); delay(80); digitalWrite(BUZZER_PIN, LOW); delay(40); }
}
void beepWarning() {
  if (millis() % 500 <= 100) digitalWrite(BUZZER_PIN, HIGH); else digitalWrite(BUZZER_PIN, LOW);
}

// Hàm đặc biệt để Reset giao thức I2C (Tránh lỗi màn hình hiển thị rác hoặc treo)
void clearI2CBus() {
  int sdaPin = A4; int sclPin = A5;
  pinMode(sdaPin, INPUT_PULLUP); pinMode(sclPin, INPUT_PULLUP); delay(20);
  pinMode(sclPin, OUTPUT);
  for (int i = 0; i < 9; i++) {
    digitalWrite(sclPin, LOW); delayMicroseconds(10);
    digitalWrite(sclPin, HIGH); delayMicroseconds(10);
  }
  pinMode(sdaPin, OUTPUT); digitalWrite(sdaPin, LOW); delayMicroseconds(10);
  digitalWrite(sclPin, HIGH); delayMicroseconds(10);
  digitalWrite(sdaPin, HIGH); delayMicroseconds(10);
  pinMode(sdaPin, INPUT); pinMode(sclPin, INPUT);
}
