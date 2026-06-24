/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 *                  Kiểm soát nhiệt độ lò ấp trứng bằng Arduino
 * Đấu nối: 
 + LCD:
  -D12: nối với chân RS của LCD  
  -D11: nối với chân ENABLE của LCD
  -D10: nối với chân D4 của LCD     
  -D9: nối với chân D5 của LCD 
  -D8: nối với chân D6 của LCD 
  -D7: nối với chân D7 của LCD   
 + Phím bấm:
  -D4: nối với chân của phím bấm SET   
  -D5: nối với chân của phím bấm UP 
  -D6: nối với chân của phím bấm DOWN     
 + Cảm biến nhiệt độ, độ ẩm DHT22:
  -D2: nối với chân của DHT22   
 + Modul Relay:
  -D2: nối với chân Modul Relays
 */

#include <DHT.h>                                                                                            // Gọi thư viện DHT (cảm biến nhiệt độ và độ ẩm)
#include <LiquidCrystal.h>                                                                                  // Gọi thư viện LiquidCrystal (để điều khiển màn hình LCD)
#include <EEPROM.h>                                                                                         // Gọi thư viện EEPROM (để lưu trữ dữ liệu trong bộ nhớ EEPROM)

#define DHTPIN 2                                                                                            // Định nghĩa chân số 2 là chân dữ liệu cho cảm biến DHT
#define RELAYPIN 3                                                                                          // Định nghĩa chân số 3 là chân kết nối với relay
#define SET_BUTTON_PIN 4                                                                                    // Định nghĩa chân số 4 là chân cho nút cài đặt
#define UP_BUTTON_PIN 5                                                                                     // Định nghĩa chân số 5 là chân cho nút tăng nhiệt độ
#define DOWN_BUTTON_PIN 6                                                                                   // Định nghĩa chân số 6 là chân cho nút giảm nhiệt độ

#define RELAY_ON  HIGH                                                                                      // Định nghĩa trạng thái bật của relay
#define RELAY_OFF LOW                                                                                       // Định nghĩa trạng thái tắt của relay

DHT dht(DHTPIN, DHT21);                                                                                     // Tạo một đối tượng DHT để đọc dữ liệu từ cảm biến
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);                                                                     // Tạo một đối tượng LiquidCrystal để điều khiển màn hình LCD

const int MODE_DEFAULT = 0;                                                                                 // Định nghĩa hằng số cho chế độ mặc định
const int MODE_SET_TEMP = 1;                                                                                // Định nghĩa hằng số cho chế độ cài đặt nhiệt độ
const int MODE_SUCCESS = 2;                                                                                 // Định nghĩa hằng số cho chế độ cài đặt thành công

float temperatureSet = 37.0;                                                                                // Nhiệt độ được đặt mặc định
float currentTemp = 0.0;                                                                                    // Biến để lưu trữ nhiệt độ hiện tại
int mode = MODE_DEFAULT;                                                                                    // Chế độ hoạt động ban đầu

unsigned long previousMillis = 0;                                                                           // Biến để theo dõi thời gian trôi qua
const long interval = 1000;                                                                                 // Khoảng thời gian giữa các đọc dữ liệu cảm biến

void setup() 
{
  Serial.begin(9600);                                                                                       // Khởi tạo giao tiếp Serial với tốc độ 9600 bps
  dht.begin();                                                                                              // Khởi tạo cảm biến DHT
  pinMode(RELAYPIN, OUTPUT);                                                                                // Đặt chân RELAYPIN là OUTPUT
  pinMode(SET_BUTTON_PIN, INPUT_PULLUP);                                                                    // Đặt chân SET_BUTTON_PIN là INPUT_PULLUP (nút cài đặt với điện trở kéo lên)
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);                                                                     // Đặt chân UP_BUTTON_PIN là INPUT_PULLUP (nút tăng nhiệt độ với điện trở kéo lên)
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);                                                                   // Đặt chân DOWN_BUTTON_PIN là INPUT_PULLUP (nút giảm nhiệt độ với điện trở kéo lên)
  lcd.begin(16, 2);                                                                                         // Khởi tạo màn hình LCD với kích thước 16x2

  lcd.setCursor(0, 0);                                                                                      // Đặt con trỏ vị trí (0,0) trên màn hình LCD
  lcd.print("Banlinhkien.com");                                                                             // In văn bản lên màn hình
  lcd.setCursor(3, 1);                                                                                      // Đặt con trỏ vị trí (3,1) trên màn hình LCD
  lcd.print("Xin Chao");                                                                                    // In văn bản lên màn hình
  delay(2000);                                                                                              // Tạm dừng 2 giây
  lcd.clear();                                                                                              // Xóa màn hình LCD

  temperatureSet = constrain(EEPROM.read(0), 20, 50);                                                       // Đọc nhiệt độ được lưu trữ trong EEPROM và giới hạn nó trong khoảng từ 20 đến 50
  updateLCD();                                                                                              // Cập nhật màn hình LCD với nhiệt độ được đọc từ EEPROM
}

void loop() 
{
  handleButtons();                                                                                          // Xử lý các nút bấm

  unsigned long currentMillis = millis();                                                                   // Lấy thời gian hiện tại

  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;                                                                         // Lưu lại thời gian hiện tại

    currentTemp = dht.readTemperature();                                                                    // Đọc nhiệt độ từ cảm biến DHT21

    if (mode != MODE_SET_TEMP) 
    {
      updateLCD();                                                                                          // Cập nhật màn hình LCD nếu không ở chế độ cài đặt nhiệt độ
    }

    controlRelay();                                                                                         // Kiểm soát relay dựa trên nhiệt độ
  }
}

void handleButtons() 
{
  int setButtonState = digitalRead(SET_BUTTON_PIN);                                                         // Đọc trạng thái của nút cài đặt
  int upButtonState = digitalRead(UP_BUTTON_PIN);                                                           // Đọc trạng thái của nút tăng nhiệt độ
  int downButtonState = digitalRead(DOWN_BUTTON_PIN);                                                       // Đọc trạng thái của nút giảm nhiệt độ

  if (setButtonState == LOW) 
  {
    mode = (mode + 1) % 3;                                                                                  // Chuyển qua chế độ khác
    updateLCD();                                                                                            // Cập nhật màn hình LCD với chế độ hoạt động mới

    if (mode == MODE_SUCCESS) 
    {
      lcd.setCursor(4, 0);
      lcd.print("Cai Dat");                                                                                 
      lcd.setCursor(3, 1);
      lcd.print("Thanh Cong");                                                                              
      delay(1000);                                                                                          
      mode = MODE_DEFAULT;                                                                                  // Chuyển về chế độ mặc định sau khi cài đặt thành công
    }
    saveSettingsToEEPROM();                                                                                 // Lưu cài đặt vào EEPROM
    delay(200);                                                                                             // Tạm dừng 200ms để tránh nhấn nút nhiều lần
  }

  switch (mode) 
  {
    case MODE_SET_TEMP:
      if (upButtonState == LOW && temperatureSet < 50) 
      {
        temperatureSet += 0.25;                                                                             // Tăng nhiệt độ được đặt lên 0.25 độ Celsius
        if (temperatureSet > 50) 
        {
          temperatureSet = 50;                                                                              // Giới hạn nhiệt độ được đặt tối đa là 50 độ
        }
        updateLCD();                                                                                        // Cập nhật màn hình LCD
        saveSettingsToEEPROM();                                                                             // Lưu cài đặt vào EEPROM
        delay(200);                                                                                         // Tạm dừng 200ms để tránh nhấn nút nhiều lần
      }
      if (downButtonState == LOW && temperatureSet > 20) 
      {
        temperatureSet -= 0.25;                                                                             // Giảm nhiệt độ được đặt xuống 0.25 độ Celsius
        if (temperatureSet < 20) 
        {
          temperatureSet = 20;                                                                              // Giới hạn nhiệt độ được đặt tối thiểu là 20 độ
        }
        updateLCD();                                                                                        // Cập nhật màn hình LCD
        saveSettingsToEEPROM();                                                                             // Lưu cài đặt vào EEPROM
        delay(200);                                                                                         // Tạm dừng 200ms để tránh nhấn nút nhiều lần
      }
      break;
  }
}

void controlRelay() 
{
  const float hysteresis = 2;                                                                               // Độ lệch nhiệt độ để tránh việc relay bật/tắt liên tục

  if (currentTemp < (temperatureSet - hysteresis)) 
  { 
    digitalWrite(RELAYPIN, RELAY_ON);                                                                       // Bật relay nếu nhiệt độ hiện tại thấp hơn nhiệt độ được đặt
  } 
  else if (currentTemp > temperatureSet)
  {
    digitalWrite(RELAYPIN, RELAY_OFF);                                                                      // Tắt relay nếu nhiệt độ hiện tại cao hơn nhiệt độ được đặt
  }
}

void updateLCD() 
{
  lcd.clear();                                                                                              // Xóa màn hình LCD

  switch (mode) 
  {
    case MODE_DEFAULT:
      lcd.setCursor(2, 0);
      lcd.print("Lo Ap Trung");                                                                             // Hiển thị "Lo Áp Trung"
      lcd.setCursor(4, 1);
      lcd.print(currentTemp);                                                                               // Hiển thị nhiệt độ hiện tại
      lcd.write(223);                                                                                       // Hiển thị ký hiệu độ Celsius (°)
      lcd.print("C");
      break;
    case MODE_SET_TEMP:
      lcd.setCursor(0, 0);
      lcd.print("Cai Dat Nhiet Do");                                                                        // Hiển thị "Cài Đặt Nhiệt Độ"
      lcd.setCursor(4, 1);
      lcd.print(temperatureSet);                                                                            // Hiển thị nhiệt độ được đặt
      lcd.write(223);                                                                                       // Hiển thị ký hiệu độ (°)
      lcd.print("C");
      break;
  }
}

void saveSettingsToEEPROM() 
{
  EEPROM.write(0, static_cast<uint8_t>(temperatureSet));                                                    // Lưu nhiệt độ được đặt vào EEPROM
}
