/* =================================================================================================*
 *                                          BANLINHKIEN.COM                                         *
 *==================================================================================================*                        
 *                    PRJ03: DIY bộ điều khiển thiết bị điện từ xa bằng remote hồng ngoại
 * 
 * Đấu nối: 
 * Arduino UNO R3 vs LCD 16x2 I2C
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 * Arduino UNO R3 vs Module DS1307
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 * Arduino UNO R3 vs Module Thu Hồng Ngoại 1838T MH-R38
 * 5V <-> + (chân giữa)  ;   GND <-> - (ký hiệu trừ)  ;   D2 <-> S (ký hiệu S signal)  
 * Arduino UNO R3 vs Module Relay 5V
 * 5V <-> VCC  ;   GND <-> GND  ;   D3 <-> IN  
 */

#include <Wire.h> 
#include <IRremote.hpp>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <EEPROM.h>

#define IR_PIN 2                // Chân kết nối bộ thu IR
#define RELAY_PIN 3             // Chân điều khiển Relay
#define BUTTON_PIN 4            // Chân kết nối nút nhấn vật lý

#define MAX_SCHEDULES 5         // Tối đa 5 khung giờ
#define EEPROM_START_ADDR 0
#define EEPROM_RELAY_ADDR (EEPROM_START_ADDR + 1 + MAX_SCHEDULES * sizeof(Schedule))

// MÃ IR ĐÃ CẬP NHẬT TỪ BẠN - VUI LÒNG KIỂM TRA LẠI CHÍNH XÁC NHẤT CÓ THỂ!
#define IR_ON 3125149440UL      // Phím 1: Bật relay (thủ công)
#define IR_OFF 3141861120UL     // Phím 2: Tắt relay (thủ công)
#define IR_MODE 3158572800UL    // Phím 3: Chuyển chế độ (Thủ công/Tự động)
#define IR_ADD 3091726080UL     // Phím 4: Thêm khung giờ / Vào Menu Khung giờ
#define IR_DELETE 4061003520UL  // Phím 5: Xóa khung giờ
#define IR_OK 3927310080UL      // Phím OK: Xác nhận / Chọn khung giờ
#define IR_ESC 4161273600UL     // Phím 6: Quay lại
#define IR_0 3910598400UL       // Phím 0
#define IR_1 4077715200UL       // Phím 1 (nhập số)
#define IR_2 3877175040UL       // Phím 2 (nhập số)
#define IR_3 2707357440UL       // Phím 3 (nhập số)
#define IR_4 4144561920UL       // Phím 4 (nhập số)
#define IR_5 3810328320UL       // Phím 5 (nhập số)
#define IR_6 2774204160UL       // Phím 6 (nhập số)
#define IR_7 3175284480UL       // Phím 7 (nhập số)
#define IR_8 2907897600UL       // Phím 8 (nhập số)
#define IR_9 3041591040UL       // Phím 9 (nhập số)
#define IR_UP 3208707840UL      // Mã phím lên (UP)
#define IR_DOWN 3860463360UL    // Mã phím xuống (DOWN)

// Các trạng thái của ứng dụng để quản lý màn hình và luồng điều khiển
enum AppState {
  STATE_MAIN_SCREEN,            // Màn hình chính (hiển thị thời gian, chế độ, relay)
  STATE_MENU_SCHEDULES,         // Màn hình danh sách khung giờ (duyệt và chọn)
  STATE_EDIT_SCHEDULE_ADD,      // Đang thêm khung giờ mới
  STATE_EDIT_SCHEDULE_MODIFY    // Đang sửa khung giờ đã có
};

LiquidCrystal_I2C lcd(0x27, 16, 2); // Địa chỉ I2C của LCD (Kiểm tra lại nếu LCD của bạn có địa chỉ khác)
RTC_DS3231 rtc;
unsigned long lastIRTime = 0;          // Thời gian nhận tín hiệu IR cuối
unsigned long lastButtonPressTime = 0; // Thời gian nhấn nút cuối
const long debounceDelay = 50;         // Thời gian debounce cho nút nhấn

// Cấu trúc lưu khung giờ
struct Schedule {
  uint8_t onHour;
  uint8_t onMinute;
  uint8_t offHour;
  uint8_t offMinute;
  bool active;
};

// Biến toàn cục
Schedule schedules[MAX_SCHEDULES];
uint8_t scheduleCount = 0;
bool isManualMode = true;                     // true: thủ công, false: tự động
bool relayState = false;                      // false: OFF, true: ON

AppState currentAppState = STATE_MAIN_SCREEN; // Trạng thái hiện tại của ứng dụng
uint8_t selectedScheduleIndex = 0;            // Chỉ số khung giờ đang được chọn/hiển thị trong menu

uint8_t inputStep = 0;                        // 0: không nhập, 1-2: giờ bật, 3-4: phút bật, 5-6: giờ tắt, 7-8: phút tắt
uint8_t tempOnHour, tempOnMinute, tempOffHour, tempOffMinute; // Biến tạm lưu giá trị khi nhập

bool rtc_initialized = false;                 // Biến cờ kiểm tra RTC đã được khởi tạo thành công chưa

// Khai báo sớm các hàm để có thể gọi lẫn nhau
void updateLCD();
void handleIR(uint32_t value);
void handleScheduleInput(uint32_t value);
void handleButtonPress();                     // Hàm xử lý nút nhấn vật lý
uint8_t mapIRToNumber(uint32_t value);
void checkSchedules(DateTime now);
void saveToEEPROM();

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);          // Kích hoạt điện trở kéo lên bên trong cho nút nhấn

  // Đọc trạng thái relay từ EEPROM và áp dụng
  EEPROM.get(EEPROM_RELAY_ADDR, relayState);
  digitalWrite(RELAY_PIN, relayState ? HIGH : LOW); // Relay mức cao (HIGH để bật relay)
  
  lcd.begin(16, 2);                           // Khởi tạo LCD 16x2
  lcd.backlight();
  IrReceiver.begin(IR_PIN, false);            // Khởi động IR
  
  Serial.println("Khoi tao RTC...");
  if (!rtc.begin()) {
    // Nếu RTC lỗi ngay từ đầu
    lcd.clear();
    lcd.print("RTC Error!");
    lcd.setCursor(0,1);
    lcd.print("Kiem tra ket noi");            // Thông báo lỗi RTC và hướng dẫn kiểm tra kết nối
    Serial.println("RTC Error! Please check RTC module connection.");
    rtc_initialized = false;                  // Đặt cờ là false để biết RTC không hoạt động
  } else {
    // RTC khởi tạo thành công
    // Luôn kiểm tra nếu RTC mất nguồn hoặc thời gian quá cũ (dưới năm 2023)
    // Thì sẽ tự động cập nhật từ thời gian biên dịch của máy tính
    if (rtc.lostPower() || rtc.now().year() < 2023) { 
      lcd.clear();
      lcd.print("RTC lost power!");
      lcd.setCursor(0,1);
      lcd.print("Set from PC...");
      delay(1000);
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Đặt lại thời gian từ máy tính khi biên dịch
      Serial.println("RTC time updated from compile time.");
    }
    rtc_initialized = true;               // Đặt cờ là true
  }

  // Đọc dữ liệu khung giờ từ EEPROM
  EEPROM.get(EEPROM_START_ADDR, scheduleCount);
  if (scheduleCount > MAX_SCHEDULES) scheduleCount = 0; // Đảm bảo số lượng không vượt quá giới hạn
  for (int i = 0; i < scheduleCount; i++) {
    EEPROM.get(EEPROM_START_ADDR + 1 + i * sizeof(Schedule), schedules[i]);
  }

  // Debug: In khung giờ đã lưu ra Serial Monitor
  Serial.println("Khung gio da luu:");
  for (int i = 0; i < scheduleCount; i++) {
    Serial.print("K"); Serial.print(i + 1);
    Serial.print(": Bat "); Serial.print(schedules[i].onHour); Serial.print(":"); Serial.print(schedules[i].onMinute);
    Serial.print(" Tat "); Serial.print(schedules[i].offHour); Serial.print(":"); Serial.print(schedules[i].offMinute);
    Serial.print(" Active: "); Serial.println(schedules[i].active ? "Yes" : "No");
  }
  Serial.print("Relay State from EEPROM: "); Serial.println(relayState ? "ON" : "OFF");

  updateLCD(); // Cập nhật màn hình ban đầu
}

void loop() {
  // Xử lý tín hiệu IR từ remote
  if (IrReceiver.decode()) {
    uint32_t dataRemote = IrReceiver.decodedIRData.decodedRawData;
    if (dataRemote > 0 && millis() - lastIRTime > 250) { // Chống lặp tín hiệu (debounce)
      Serial.print("IR Code: "); Serial.print(dataRemote);
      Serial.print(", AppState: "); Serial.print(currentAppState);
      Serial.print(", inputStep: "); Serial.println(inputStep); // Debug thông tin trạng thái
      handleIR(dataRemote); // Gọi hàm xử lý tín hiệu IR
      lastIRTime = millis();
    }
    IrReceiver.resume(); // Tiếp tục nhận tín hiệu IR
  }

  // Xử lý nút nhấn vật lý
  if (digitalRead(BUTTON_PIN) == LOW && millis() - lastButtonPressTime > debounceDelay) {
    handleButtonPress();
    lastButtonPressTime = millis();
  }

  // --- PHẦN XỬ LÝ NHẬP THỜI GIAN TỪ SERIAL ĐÃ BỊ LOẠI BỎ ---
  // if (Serial.available()) {
  //   setRTCTimeFromSerial();
  // }

  // Chế độ tự động: Kiểm tra khung giờ (chỉ thực hiện khi ở màn hình chính và chế độ tự động)
  // Chỉ chạy khi RTC đã được khởi tạo thành công
  if (currentAppState == STATE_MAIN_SCREEN && !isManualMode && rtc_initialized) {
    DateTime now = rtc.now();
    checkSchedules(now);
  }

  // Cập nhật LCD để hiển thị thời gian thực (chỉ ở màn hình chính)
  // Chỉ cập nhật khi RTC đã được khởi tạo HOẶC khi đang ở màn hình khác không phải màn hình chính (để đảm bảo updateLCD vẫn được gọi khi thay đổi trạng thái)
  static unsigned long lastLCDUpdate = 0;
  if ((currentAppState == STATE_MAIN_SCREEN && millis() - lastLCDUpdate > 1000 && rtc_initialized) || 
      (currentAppState != STATE_MAIN_SCREEN && millis() - lastLCDUpdate > 1000) ) { 
    updateLCD();
    lastLCDUpdate = millis();
  }
  // Nếu RTC lỗi và đang ở màn hình chính, chúng ta muốn thông báo "RTC Error!" cố định trên LCD
  if (currentAppState == STATE_MAIN_SCREEN && !rtc_initialized && millis() - lastLCDUpdate > 5000) {
      // Cập nhật lại màn hình RTC Error mỗi 5 giây để đảm bảo hiển thị
      lcd.clear();
      lcd.print("RTC Error!");
      lcd.setCursor(0,1);
      lcd.print("Kiem tra ket noi"); // Thay đổi thông báo
      lastLCDUpdate = millis();
  }


  delay(10); // Tăng phản hồi và ổn định hệ thống
}

// Hàm cập nhật hiển thị trên màn hình LCD dựa vào trạng thái ứng dụng
void updateLCD() {
  lcd.clear(); // Xóa màn hình cũ
  switch (currentAppState) {
    case STATE_MAIN_SCREEN: { // Màn hình chính
      DateTime now = rtc.now();
      lcd.setCursor(0, 0);
      lcd.print(isManualMode ? "Thu cong " : "Tu dong  "); // Chế độ hiện tại
      if (rtc_initialized) { // Chỉ hiển thị thời gian nếu RTC hoạt động
        lcd.print(now.hour() < 10 ? "0" : ""); lcd.print(now.hour());
        lcd.print(":");
        lcd.print(now.minute() < 10 ? "0" : ""); lcd.print(now.minute());
      } else {
        lcd.print("RTC ERR!"); // Hiển thị lỗi nếu RTC chưa sẵn sàng
      }
      
      lcd.setCursor(0, 1);
      lcd.print("Relay: ");
      lcd.print(relayState ? "ON " : "OFF"); // Trạng thái Relay
      lcd.print(" KH: ");
      lcd.print(scheduleCount); // Số lượng khung giờ đã cài
      break;
    }
    case STATE_MENU_SCHEDULES: { // Màn hình danh sách khung giờ
      lcd.setCursor(0, 0);
      lcd.print("--- Khung Gio ---");
      lcd.setCursor(0, 1);
      if (scheduleCount == 0) {
        lcd.print("Chua co KH. ADD!"); // Thông báo nếu chưa có khung giờ nào
      } else {
        // Hiển thị khung giờ đang được chọn
        lcd.print("K"); lcd.print(selectedScheduleIndex + 1); lcd.print(": ");
        lcd.print(schedules[selectedScheduleIndex].onHour < 10 ? "0" : ""); lcd.print(schedules[selectedScheduleIndex].onHour);
        lcd.print(":");
        lcd.print(schedules[selectedScheduleIndex].onMinute < 10 ? "0" : ""); lcd.print(schedules[selectedScheduleIndex].onMinute);
        lcd.print("-");
        lcd.print(schedules[selectedScheduleIndex].offHour < 10 ? "0" : ""); lcd.print(schedules[selectedScheduleIndex].offHour);
        lcd.print(":");
        lcd.print(schedules[selectedScheduleIndex].offMinute < 10 ? "0" : ""); lcd.print(schedules[selectedScheduleIndex].offMinute);
        lcd.print(schedules[selectedScheduleIndex].active ? " ON" : "OFF"); // Trạng thái Active của khung giờ
      }
      break;
    }
    case STATE_EDIT_SCHEDULE_ADD: // Màn hình thêm mới khung giờ
    case STATE_EDIT_SCHEDULE_MODIFY: { // Màn hình sửa khung giờ
      lcd.setCursor(0, 0);
      lcd.print(currentAppState == STATE_EDIT_SCHEDULE_ADD ? "Them K.H" : "Sua K.H");
      if (currentAppState == STATE_EDIT_SCHEDULE_MODIFY) {
         lcd.print(" "); lcd.print(selectedScheduleIndex + 1); // Hiển thị số thứ tự khung giờ đang sửa
      }
      lcd.print(":");
      lcd.setCursor(0, 1);
      
      // Hiển thị thông tin nhập liệu dựa trên inputStep
      switch (inputStep) {
        case 1: 
        case 2: // Nhập Giờ bật (GB)
          lcd.print("GB: ");
          lcd.print(tempOnHour < 10 && inputStep == 1 ? "0" : ""); // Hiển thị 0 nếu mới nhập 1 chữ số
          lcd.print(tempOnHour);
          lcd.print(":");
          lcd.print(tempOnMinute < 10 ? "0" : ""); lcd.print(tempOnMinute); 
          break;
        case 3: 
        case 4: // Nhập Phút bật (PB)
          lcd.print("PB: ");
          lcd.print(tempOnHour < 10 ? "0" : ""); lcd.print(tempOnHour);
          lcd.print(":");
          lcd.print(tempOnMinute < 10 && inputStep == 3 ? "0" : ""); 
          lcd.print(tempOnMinute);
          break;
        case 5: 
        case 6: // Nhập Giờ tắt (GT)
          lcd.print("GT: ");
          lcd.print(tempOffHour < 10 && inputStep == 5 ? "0" : ""); 
          lcd.print(tempOffHour);
          lcd.print(":");
          lcd.print(tempOffMinute < 10 ? "0" : ""); lcd.print(tempOffMinute); 
          break;
        case 7: 
        case 8: // Nhập Phút tắt (PT)
          lcd.print("PT: ");
          lcd.print(tempOffHour < 10 ? "0" : ""); lcd.print(tempOffHour);
          lcd.print(":");
          lcd.print(tempOffMinute < 10 && inputStep == 7 ? "0" : ""); 
          lcd.print(tempOffMinute);
          break;
        case 9: // Đã nhập xong, chờ nhấn OK
          lcd.print("Xac nhan OK?");
          lcd.setCursor(0, 1);
          lcd.print(tempOnHour < 10 ? "0" : ""); lcd.print(tempOnHour);
          lcd.print(":");
          lcd.print(tempOnMinute < 10 ? "0" : ""); lcd.print(tempOnMinute);
          lcd.print("-");
          lcd.print(tempOffHour < 10 ? "0" : ""); lcd.print(tempOffHour);
          lcd.print(":");
          lcd.print(tempOffMinute < 10 ? "0" : ""); lcd.print(tempOffMinute);
          break;
        default: // Trạng thái ban đầu khi mới vào màn hình thêm/sửa
          lcd.print("Nhap gio bat");
          break;
      }
      break;
    }
  }
}

// Xử lý tín hiệu IR dựa trên trạng thái ứng dụng
void handleIR(uint32_t value) {
  switch (currentAppState) {
    case STATE_MAIN_SCREEN: // Đang ở màn hình chính
      switch (value) {
        case IR_ON: // Bật relay thủ công (chỉ hoạt động ở chế độ thủ công)
          if (isManualMode) {
            relayState = true;
            digitalWrite(RELAY_PIN, HIGH);
            EEPROM.put(EEPROM_RELAY_ADDR, relayState); // Lưu trạng thái
            updateLCD();
          }
          break;
        case IR_OFF: // Tắt relay thủ công (chỉ hoạt động ở chế độ thủ công)
          if (isManualMode) {
            relayState = false;
            digitalWrite(RELAY_PIN, LOW);
            EEPROM.put(EEPROM_RELAY_ADDR, relayState); // Lưu trạng thái
            updateLCD();
          }
          break;
        case IR_MODE: // Chuyển đổi chế độ (Thủ công/Tự động)
          isManualMode = !isManualMode;
          updateLCD();
          break;
        case IR_ADD: // Nút ADD/Menu để vào danh sách khung giờ
          currentAppState = STATE_MENU_SCHEDULES;
          selectedScheduleIndex = (scheduleCount > 0) ? 0 : 0; // Reset index hoặc giữ 0 nếu không có KH
          updateLCD();
          break;
        // Các phím số, OK, ESC không có tác dụng ở màn hình chính
      }
      break;

    case STATE_MENU_SCHEDULES: // Đang ở màn hình danh sách khung giờ
      switch (value) {
        case IR_UP: // Cuộn lên
          if (scheduleCount > 0) {
            selectedScheduleIndex = (selectedScheduleIndex == 0) ? (scheduleCount - 1) : (selectedScheduleIndex - 1);
            updateLCD();
          }
          break;
        case IR_DOWN: // Cuộn xuống
          if (scheduleCount > 0) {
            selectedScheduleIndex = (selectedScheduleIndex == scheduleCount - 1) ? 0 : (selectedScheduleIndex + 1);
            updateLCD();
          }
          break;
        case IR_OK: // Chọn khung giờ để chỉnh sửa
          if (scheduleCount > 0) {
            currentAppState = STATE_EDIT_SCHEDULE_MODIFY;
            inputStep = 1; // Bắt đầu nhập giờ bật
            // Tải thông tin của khung giờ đang chọn vào biến tạm để sửa
            tempOnHour = schedules[selectedScheduleIndex].onHour;
            tempOnMinute = schedules[selectedScheduleIndex].onMinute;
            tempOffHour = schedules[selectedScheduleIndex].offHour;
            tempOffMinute = schedules[selectedScheduleIndex].offMinute;
            updateLCD();
          } else {
            lcd.clear(); lcd.print("Chua co KH!"); delay(1000); updateLCD();
          }
          break;
        case IR_ADD: // Thêm khung giờ mới từ menu
          if (scheduleCount < MAX_SCHEDULES) {
            currentAppState = STATE_EDIT_SCHEDULE_ADD;
            inputStep = 1; // Bắt đầu nhập giờ bật
            tempOnHour = 0; tempOnMinute = 0; tempOffHour = 0; tempOffMinute = 0; // Reset giá trị tạm thời
            updateLCD();
          } else {
            lcd.clear(); lcd.print("Het bo nho!"); delay(1000); updateLCD();
          }
          break;
        case IR_DELETE: // Xóa khung giờ đang chọn
          if (scheduleCount > 0) {
            // Dịch chuyển các phần tử phía sau lên để lấp đầy chỗ trống
            for (int i = selectedScheduleIndex; i < scheduleCount - 1; i++) {
              schedules[i] = schedules[i + 1];
            }
            scheduleCount--; // Giảm số lượng khung giờ
            // Đảm bảo selectedScheduleIndex không vượt quá giới hạn sau khi xóa
            if (selectedScheduleIndex >= scheduleCount && scheduleCount > 0) {
              selectedScheduleIndex = scheduleCount - 1;
            } else if (scheduleCount == 0) {
              selectedScheduleIndex = 0; // Nếu không còn khung giờ nào
            }
            saveToEEPROM(); // Lưu vào EEPROM
            lcd.clear(); lcd.print("Da xoa KH!"); delay(1000);
            updateLCD();
          } else {
            lcd.clear(); lcd.print("Khong co KH!"); delay(1000); updateLCD();
          }
          break;
        case IR_ESC: // Quay lại màn hình chính
          currentAppState = STATE_MAIN_SCREEN;
          updateLCD();
          break;
      }
      break;

    case STATE_EDIT_SCHEDULE_ADD: // Đang thêm khung giờ
    case STATE_EDIT_SCHEDULE_MODIFY: // Đang sửa khung giờ
      handleScheduleInput(value); // Xử lý các phím số, OK, ESC trong chế độ nhập
      break;
  }
}

// Xử lý nút nhấn vật lý
void handleButtonPress() {
  Serial.println("Nut nhan vat ly duoc nhan!");
  
  if (currentAppState == STATE_MAIN_SCREEN) { // Chỉ xử lý khi đang ở màn hình chính
      // Luôn bật/tắt relay và chuyển sang chế độ thủ công khi nhấn nút vật lý
      isManualMode = true; // Chuyển sang chế độ thủ công
      relayState = !relayState; // Đảo trạng thái relay
      digitalWrite(RELAY_PIN, relayState ? HIGH : LOW); // Áp dụng trạng thái
      EEPROM.put(EEPROM_RELAY_ADDR, relayState); // Lưu trạng thái vào EEPROM
      
      lcd.clear();
      lcd.print(relayState ? "Relay ON!" : "Relay OFF!"); // Thông báo trạng thái mới
      lcd.setCursor(0,1);
      lcd.print("Thu cong active");
      delay(1000); // Hiển thị thông báo ngắn gọn
      updateLCD(); // Cập nhật hiển thị màn hình chính
      Serial.print("Relay State (Button): "); Serial.println(relayState ? "ON" : "OFF");
  }
}


// Xử lý nhập khung giờ (cho cả thêm mới và chỉnh sửa)
void handleScheduleInput(uint32_t value) {
  if (value == IR_ESC) { // Nút Quay lại (Hủy bỏ nhập)
    Serial.println("Escaped input");
    currentAppState = STATE_MENU_SCHEDULES; // Quay lại màn hình menu
    inputStep = 0; // Reset bước nhập
    lcd.clear(); lcd.print("Huy nhap gio!"); delay(1000); updateLCD();
    return;
  }

  uint8_t num = mapIRToNumber(value);
  if (num == 255) { // Không phải phím số
    if (value == IR_OK) {
      if (inputStep == 9) { // Đã nhập đủ 8 chữ số và nhấn OK
        // Kiểm tra tính hợp lệ cuối cùng trước khi lưu
        if (tempOnHour <= 23 && tempOnMinute <= 59 && tempOffHour <= 23 && tempOffMinute <= 59) {
          if (currentAppState == STATE_EDIT_SCHEDULE_ADD) {
            schedules[scheduleCount].onHour = tempOnHour;
            schedules[scheduleCount].onMinute = tempOnMinute;
            schedules[scheduleCount].offHour = tempOffHour;
            schedules[scheduleCount].offMinute = tempOffMinute;
            schedules[scheduleCount].active = true; // Đặt là active khi thêm mới
            scheduleCount++; // Tăng số lượng khung giờ
            selectedScheduleIndex = scheduleCount - 1; // Chọn khung giờ vừa thêm
          } else { // STATE_EDIT_SCHEDULE_MODIFY
            schedules[selectedScheduleIndex].onHour = tempOnHour;
            schedules[selectedScheduleIndex].onMinute = tempOnMinute;
            schedules[selectedScheduleIndex].offHour = tempOffHour;
            schedules[selectedScheduleIndex].offMinute = tempOffMinute;
            // Trạng thái active không đổi khi sửa
          }
          saveToEEPROM(); // Lưu vào EEPROM
          Serial.print("Luu/Sua khung gio K"); Serial.print(selectedScheduleIndex + 1);
          Serial.print(": Bat "); Serial.print(tempOnHour); Serial.print(":"); Serial.print(tempOnMinute);
          Serial.print(" Tat "); Serial.print(tempOffHour); Serial.print(":"); Serial.println(tempOffMinute);
          
          currentAppState = STATE_MENU_SCHEDULES; // Quay lại màn hình menu
          inputStep = 0;
          lcd.clear(); lcd.print("Luu/Sua OK!"); delay(1000); updateLCD();
        } else {
          lcd.clear(); lcd.print("Gio khong hop le!"); delay(1000);
          updateLCD(); // Cập nhật lại màn hình nhập để người dùng sửa
        }
      } else {
          lcd.clear(); lcd.print("Nhap du so!"); delay(1000); updateLCD();
      }
    }
    return; // Bỏ qua các mã IR khác không phải số hoặc OK
  }

  // Nếu là phím số, xử lý nhập liệu
  bool validInput = true;
  switch (inputStep) {
    case 1: // Giờ bật, hàng chục
      if (num <= 2) { tempOnHour = num * 10; inputStep++; } 
      else { validInput = false; lcd.clear(); lcd.print("Gio sai! (0-2)"); delay(1000); }
      break;
    case 2: // Giờ bật, hàng đơn vị
      if (tempOnHour / 10 == 2 && num > 3) { // Giới hạn 20-23
        validInput = false; lcd.clear(); lcd.print("Gio sai! (0-23)"); delay(1000);
      } else { tempOnHour += num; inputStep++; }
      break;
    case 3: // Phút bật, hàng chục
      if (num <= 5) { tempOnMinute = num * 10; inputStep++; } 
      else { validInput = false; lcd.clear(); lcd.print("Phut sai! (0-5)"); delay(1000); }
      break;
    case 4: // Phút bật, hàng đơn vị
      tempOnMinute += num; inputStep++;
      break;
    case 5: // Giờ tắt, hàng chục
      if (num <= 2) { tempOffHour = num * 10; inputStep++; } 
      else { validInput = false; lcd.clear(); lcd.print("Gio sai! (0-2)"); delay(1000); }
      break;
    case 6: // Giờ tắt, hàng đơn vị
      if (tempOffHour / 10 == 2 && num > 3) {
        validInput = false; lcd.clear(); lcd.print("Gio sai! (0-23)"); delay(1000);
      } else { tempOffHour += num; inputStep++; }
      break;
    case 7: // Phút tắt, hàng chục
      if (num <= 5) { tempOffMinute = num * 10; inputStep++; } 
      else { validInput = false; lcd.clear(); lcd.print("Phut sai! (0-5)"); delay(1000); }
      break;
    case 8: // Phút tắt, hàng đơn vị
      tempOffMinute += num; inputStep++;
      break;
  }
  updateLCD(); // Luôn cập nhật LCD sau mỗi lần nhập số
}

// Ánh xạ mã IR thành số
uint8_t mapIRToNumber(uint32_t value) {
  switch (value) {
    case IR_0: return 0;
    case IR_1: return 1;
    case IR_2: return 2;
    case IR_3: return 3;
    case IR_4: return 4;
    case IR_5: return 5;
    case IR_6: return 6;
    case IR_7: return 7;
    case IR_8: return 8;
    case IR_9: return 9;
    default: return 255; // Giá trị không hợp lệ
  }
}

// Kiểm tra khung giờ để bật/tắt relay tự động
void checkSchedules(DateTime now) {
  bool shouldBeOn = false;
  for (int i = 0; i < scheduleCount; i++) {
    if (schedules[i].active) { // Chỉ kiểm tra các khung giờ đang hoạt động
      uint32_t nowMinutes = now.hour() * 60 + now.minute();
      uint32_t onMinutes = schedules[i].onHour * 60 + schedules[i].onMinute;
      uint32_t offMinutes = schedules[i].offHour * 60 + schedules[i].offMinute;

      if (onMinutes < offMinutes) { // Khung giờ trong cùng một ngày (ví dụ: 08:00 - 17:00)
        if (nowMinutes >= onMinutes && nowMinutes < offMinutes) {
          shouldBeOn = true;
          break;
        }
      } else { // Khung giờ qua đêm (ví dụ: 22:00 - 06:00)
        if (nowMinutes >= onMinutes || nowMinutes < offMinutes) {
          shouldBeOn = true;
          break;
        }
      }
    }
  }

  if (shouldBeOn != relayState) { // Nếu trạng thái relay cần thay đổi
    relayState = shouldBeOn;
    digitalWrite(RELAY_PIN, relayState ? HIGH : LOW); // Thay đổi trạng thái relay
    EEPROM.put(EEPROM_RELAY_ADDR, relayState); // Lưu trạng thái mới vào EEPROM
    // updateLCD(); // Không gọi updateLCD ở đây để tránh gián đoạn các màn hình khác
  }
}

// Lưu tất cả dữ liệu khung giờ và trạng thái relay vào EEPROM
void saveToEEPROM() {
  EEPROM.put(EEPROM_START_ADDR, scheduleCount); // Lưu số lượng khung giờ
  for (int i = 0; i < scheduleCount; i++) {
    EEPROM.put(EEPROM_START_ADDR + 1 + i * sizeof(Schedule), schedules[i]); // Lưu từng khung giờ
  }
}


