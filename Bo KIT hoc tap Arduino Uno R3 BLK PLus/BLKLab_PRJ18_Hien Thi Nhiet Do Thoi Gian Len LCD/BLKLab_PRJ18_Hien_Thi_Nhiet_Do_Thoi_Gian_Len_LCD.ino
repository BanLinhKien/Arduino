/*
 * ===========================================================================================================================*
 *                                                        BANLINHKIEN.COM
 * ===========================================================================================================================*                           
 *                                          Hiển thị nhiệt độ, thời gian lên LCD
 * Đấu nối: 
 + LCD:
  -D12: nối với chân RS của LCD  
  -D11: nối với chân ENABLE của LCD
  -D10: nối với chân D4 của LCD     
  -D9: nối với chân D5 của LCD 
  -D8: nối với chân D6 của LCD 
  -D7: nối với chân D7 của LCD   
 + Cảm biến nhiệt độ, độ ẩm DHT21:
  -D2: nối với chân OUT của DHT21   
 + Modul thời gian thực DS1307:
  -SCL: nối với chân SCL của DS1307
  -SDA: nối với chân SCL của SDA
 */

#include <Wire.h>                                           // Thư viện Wire cho việc giao tiếp I2C
#include <LiquidCrystal.h>                                  // Thư viện LiquidCrystal cho việc điều khiển màn hình LCD
#include <DHT.h>                                            // Thư viện DHT cho cảm biến nhiệt độ và độ ẩm
#include <RTClib.h>                                         // Thư viện RTClib cho module thời gian thực (Real-Time Clock)

#define DHTPIN 2                                            // Định nghĩa chân dữ liệu kết nối cảm biến DHT21
#define DHTTYPE DHT21                                       // Định nghĩa loại cảm biến DHT21
DHT dht(DHTPIN, DHTTYPE);                                   // Khởi tạo đối tượng cảm biến DHT

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);                     // Khởi tạo đối tượng cho màn hình LCD với các chân kết nối

RTC_DS1307 rtc;                                             // Khởi tạo đối tượng cho module thời gian thực DS1307

void setup() 
{
  lcd.begin(16, 2);                                         // Khởi tạo màn hình LCD với kích thước 16x2 (16 ký tự, 2 dòng)
  dht.begin();                                              // Khởi tạo cảm biến DHT
  Wire.begin();                                             // Khởi tạo giao tiếp I2C
  rtc.begin();                                              // Khởi tạo module thời gian thực

  lcd.setCursor(0, 0);                                      // Đặt con trỏ màn hình LCD ở hàng 0, cột 0
  lcd.print("Banlinhkien.com");                             // In chuỗi "Banlinhkien.com" lên màn hình LCD
  delay(2000);                                              // Delay 2 giây để hiển thị thông báo

  if (!rtc.isrunning())                                     // Kiểm tra xem module thời gian thực có đang chạy hay không
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));         // Nếu không, sử dụng thời gian hệ thống hiện tại để cài đặt thời gian cho module
  }
}

void loop() 
{
  float temperature = dht.readTemperature();                // Đọc nhiệt độ từ cảm biến DHT21
  DateTime now = rtc.now();                                 // Đọc thời gian từ module DS1307
  lcd.clear();                                              // Xóa nội dung cũ trên màn hình LCD

  lcd.setCursor(0, 0);                                      // Đặt con trỏ màn hình LCD ở hàng 0, cột 0
  lcd.print("Nhiet do:");                                   // In chuỗi "Nhiet do:" lên màn hình LCD
  lcd.print(temperature);                                   // In giá trị nhiệt độ lên màn hình LCD
  lcd.write(223);                                           // In ký hiệu độ Celsius (°C)
  lcd.print("C");                                           //In ra kí tự C lên màn hình LCD

  lcd.setCursor(4, 1);                                      // Đặt con trỏ màn hình LCD ở hàng 1, cột 4
  lcd.print(now.hour(), DEC);                               // In giờ lên màn hình LCD
  lcd.print(":");                                           // In dấu hai chấm
  lcd.print(now.minute(), DEC);                             // In phút lên màn hình LCD
  lcd.print(":");                                           // In dấu hai chấm
  lcd.print(now.second(), DEC);                             // In giây lên màn hình LCD

  delay(1000);                                              // Delay 1 giây trước khi lặp lại quá trình
}
