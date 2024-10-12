/*====================================================================================================
 *                                          BANLINHKIEN.COM                                          *
 *====================================================================================================                                          
 *                       HƯỚNG DẪN LÀM HỆ THỐNG ĐÓNG/MỞ CỬA TỰ ĐỘNG BẰNG ARDUINO 
 * Đấu nối:
 *        Arduino      |      Servo
 *          D5               Dây cam
 *______________________________________________          
 *        Arduino      |      SRF04
 *          D2         |      Trig
 *          D3         |      Echo
 *______________________________________________        
 *        Arduino      |     LCD_I2C
 *          A4         |      SDA
 *          A5         |      SCL
 *______________________________________________        
 *        Arduino      |    Còi chíp
 *          D4         |    Chân dương
 *                  
 *__________________________________________________________________________________________________*/
 
#include <Servo.h>                        // Gọi thư viện của Servo 
#include <Wire.h>                         // Gọi thư viện giao tiếp với I2C
#include <LiquidCrystal_I2C.h>            // Gọi thư viện LCD giao tiếp với I2C

#define GOC_DONG 0                        // Góc đóng của servo (độ)
#define GOC_MO 90                         // Góc mở của servo (độ)
#define KHOANG_CACH 10                    // Khoảng cách để mở cửa (cm)
#define THOI_GIAN 1000                    // Thời gian đóng mở SERVO (ms)

LiquidCrystal_I2C lcd(0x27, 16, 2);       // Khởi tạo đối tượng LiquidCrystal_I2C, sử dụng địa chỉ 0x27 của LCD
const int trigPin = 2;                    // Chân Trig của cảm biến
const int echoPin = 3;                    // Chân Echo của cảm biến
const int speakPin = 4;                   // Chân kết nối với còi chip
const int servoPin = 5;                   // Chân kết nối với Servo Motor

int test=0;
long duration;                            // Khai báo biến đọc thời gian
int distance;                             // Khai báo biến chuyển đổi thời gian thành khoảng cách
Servo servo;                              // Khai báo dối tượng servo
byte TIM[] = {0x00,0x0A,0x1F,0x1F,0x0E,0x04,0x00,0x00};         //Khai báo mảng ký tự trái tim

void setup() {
  Wire.begin();                           // Khởi tạo giao tiếp I2C
  lcd.begin(16, 2);                       // Khởi tạo màn hình LCD với số cột 16 và số hàng 2
  lcd.backlight();                        // Bật đèn nền
  lcd.createChar(0, TIM);                 // Hiển thị ký tự trái tim

  pinMode(speakPin, OUTPUT);               // Đặt chân loa là chân đầu ra
  pinMode(trigPin, OUTPUT);                // Đặt chân trigPin là chân đầu ra
  pinMode(echoPin, INPUT);                 // Đặt chân echoPin là chân đầu vào
  servo.attach(servoPin);                  // Khởi tạo Servo Motor
  
  lcd.setCursor(15,0);                      // Đưa con trỏ về cột 15 hàng 0
  lcd.write(byte(0));                       // Hiển thị ký tự trái tim
  lcd.setCursor(0,0);                       // Đưa con trỏ về cột 0 hàng 0
  lcd.print("Banlinhkien.com");             // Hiển thị ra màn hình LCD dòng chữ 'Banlinhkien.com'

}

void loop() {
  digitalWrite(trigPin, LOW);              // Gửi xung từ chân Trig trong 10µs để kích hoạt cảm biến
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);       // Đọc thời gian trôi qua từ chân Echo
  distance = duration * 0.034 / 2;         // Chuyển đổi thời gian thành khoảng cách (cm)
  if (distance < KHOANG_CACH) {            // Nếu khoảng cách nhỏ hơn 10cm (có vật cản)
    servo.write(GOC_MO);                   // Servo mở cửa (góc 90 độ)
    lcd.setCursor(4,1);                    // Đưa con trỏ về cột 1 hàng 1
    lcd.print("Xin Chao");            // In ra màn hình dòng chữ 'Mời Quý Khách '
    if(test ==0 ){
      digitalWrite(speakPin, HIGH);        // Bật còi báo 
      delay(100);
      digitalWrite(speakPin, LOW);
      delay(100);
      digitalWrite(speakPin, HIGH);        
      delay(100);
      digitalWrite(speakPin, LOW);
    }
    delay(THOI_GIAN);                      // Servo mở trong 1s 
    test = 1;
  } else {
    servo.write(GOC_DONG);                 // Đóng cửa (góc 0 độ)
    delay(THOI_GIAN);                      // Servo mở trong 1s
    lcd.setCursor(0,1);                    //Đưa con trỏ về cột 0 hàng 1
    lcd.print("                ");         //Xóa màn hình hàng 1
    test=0;
  }
}
