
/* =================================================================================================*
 *                                          BANLINHKIEN.COM                                         *
 *==================================================================================================*                        
 *                 	   PRJ05: DIY hệ thống đóng/mở của tự động bằng Arduino
 * 
 * Đấu nối: 
 * Arduino UNO R3 vs LCD 16x2 I2C
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 *
 * Arduino UNO R3 vs Động cơ Servo SG90
 * Servo 1: 5V <-> Đỏ  ;   GND <-> Nâu  ;   D5 <-> Vàng  
 * Servo 2: 5V <-> Đỏ  ;   GND <-> Nâu  ;   D9 <-> Vàng  
 *
 * Arduino UNO R3 vs Module JQ6500
 * 5V <-> VCC  ;   GND <-> GND  ;   D6 <-> TX  ;   D7 <-> RX
 * 
 * Arduino UNO R3 vs Cảm biến siêu âm SRF04
 * 5V <-> VCC  ;   GND <-> GND  ;   D2 <-> TRIG ;   D3 <-> ECHO
 */

#include <Servo.h>                        // Gọi thư viện của Servo 
#include <Wire.h>                         // Gọi thư viện giao tiếp với I2C
#include <LiquidCrystal_I2C.h>            // Gọi thư viện LCD giao tiếp với I2C
#include <SoftwareSerial.h>               // Gọi thư viện SoftwareSerial cho JQ6500

#define GOC_DONG 0                        // Góc đóng của servo 1 (độ)
#define GOC_MO 90                         // Góc mở của servo 1, góc đóng của servo 2 (độ)
#define KHOANG_CACH 10                    // Khoảng cách để mở cửa (cm)
#define THOI_GIAN 1000                    // Thời gian đóng mở SERVO (ms)

LiquidCrystal_I2C lcd(0x27, 16, 2);       // Khởi tạo đối tượng LiquidCrystal_I2C, sử dụng địa chỉ 0x27 của LCD
const int trigPin = 2;                    // Chân Trig của cảm biến
const int echoPin = 3;                    // Chân Echo của cảm biến
const int speakPin = 4;                   // Chân kết nối với còi chip
const int servoPin1 = 5;                  // Chân kết nối với Servo Motor 1
const int servoPin2 = 9;                  // Chân kết nối với Servo Motor 2
const int mp3RxPin = 6;                   // Chân RX của JQ6500 (kết nối với TX của Arduino)
const int mp3TxPin = 7;                   // Chân TX của JQ6500 (kết nối với RX của Arduino)

int test = 0;
long duration;                            // Khai báo biến đọc thời gian
int distance;                             // Khai báo biến chuyển đổi thời gian thành khoảng cách
Servo servo1;                             // Khai báo đối tượng servo 1
Servo servo2;                             // Khai báo đối tượng servo 2
SoftwareSerial mp3Serial(mp3RxPin, mp3TxPin); // Khởi tạo SoftwareSerial cho JQ6500
byte TIM[] = {0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00}; // Khai báo mảng ký tự trái tim

// Hàm gửi lệnh đến JQ6500
void sendJQ6500Command(byte command, byte param1, byte param2) {
  byte frame[8] = {0x7E, 0xFF, 0x06, command, 0x00, param1, param2, 0xEF};
  for (int i = 0; i < 8; i++) {
    mp3Serial.write(frame[i]);
  }
}

void setup() {
  Wire.begin();                           // Khởi tạo giao tiếp I2C
  lcd.begin(16, 2);                       // Khởi tạo màn hình LCD với số cột 16 và số hàng 2
  lcd.backlight();                        // Bật đèn nền
  lcd.createChar(0, TIM);                 // Hiển thị ký tự trái tim

  pinMode(speakPin, OUTPUT);              // Đặt chân loa là chân đầu ra
  pinMode(trigPin, OUTPUT);               // Đặt chân trigPin là chân đầu ra
  pinMode(echoPin, INPUT);                // Đặt chân echoPin là chân đầu vào
  servo1.attach(servoPin1);               // Khởi tạo Servo Motor 1
  servo2.attach(servoPin2);               // Khởi tạo Servo Motor 2
  
  mp3Serial.begin(9600);                  // Khởi tạo giao tiếp Serial cho JQ6500
  delay(500);                             // Chờ module khởi động
  sendJQ6500Command(0x09, 0x00, 0x02);    // Chọn nguồn TF card
  delay(200);
  sendJQ6500Command(0x06, 0x00, 20);      // Đặt âm lượng (20/30)
  delay(200);
  sendJQ6500Command(0x08, 0x00, 0x01);    // Không lặp lại bài hát
  
  lcd.setCursor(15, 0);                   // Đưa con trỏ về cột 15 hàng 0
  lcd.write(byte(0));                    // Hiển thị ký tự trái tim
  lcd.setCursor(0, 0);                    // Đưa con trỏ về cột 0 hàng 0
  lcd.print("Banlinhkien.com");           // Hiển thị ra màn hình LCD dòng chữ 'Banlinhkien.com'
}

void loop() {
  digitalWrite(trigPin, LOW);             // Gửi xung từ chân Trig trong 10µs để kích hoạt cảm biến
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);      // Đọc thời gian trôi qua từ chân Echo
  distance = duration * 0.034 / 2;        // Chuyển đổi thời gian thành khoảng cách (cm)
  
  if (distance < KHOANG_CACH) {           // Nếu khoảng cách nhỏ hơn 10cm (có vật cản)
    servo1.write(GOC_MO);                 // Servo 1 mở cửa (góc 90 độ)
    servo2.write(GOC_DONG);               // Servo 2 mở cửa (góc 0 độ, ngược với servo 1)
    lcd.setCursor(4, 1);                  // Đưa con trỏ về cột 4 hàng 1
    lcd.print("Xin Chao");                // In ra màn hình dòng chữ 'Xin Chao'
    
    if (test == 0) {
      digitalWrite(speakPin, HIGH);       // Bật còi báo 
      delay(100);
      digitalWrite(speakPin, LOW);
      delay(100);
      digitalWrite(speakPin, HIGH);       
      delay(100);
      digitalWrite(speakPin, LOW);
      
      sendJQ6500Command(0x03, 0x00, 0x01); // Phát file MP3 số 1 (001.mp3)
    }
    delay(THOI_GIAN);                     // Servo mở trong 1s 
    test = 1;
  } else {
    servo1.write(GOC_DONG);               // Servo 1 đóng cửa (góc 0 độ)
    servo2.write(GOC_MO);                 // Servo 2 đóng cửa (góc 90 độ, ngược với servo 1)
    delay(THOI_GIAN);                     // Servo mở trong 1s
    lcd.setCursor(0, 1);                  // Đưa con trỏ về cột 0 hàng 1
    lcd.print("                ");        // Xóa màn hình hàng 1
    sendJQ6500Command(0x0D, 0x00, 0x00); // Dừng phát nhạc
    test = 0;
  }
}