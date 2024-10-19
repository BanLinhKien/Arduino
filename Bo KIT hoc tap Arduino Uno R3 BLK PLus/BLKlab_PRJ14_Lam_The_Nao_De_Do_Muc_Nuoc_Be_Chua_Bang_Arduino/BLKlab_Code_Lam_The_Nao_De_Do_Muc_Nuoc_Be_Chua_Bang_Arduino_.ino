/*=============================================================================================================*
 *                                            BANLINHKIEN.COM                                                  *
 *=============================================================================================================*                                            
 *                  HƯỚNG DẪN ĐO MỰC NƯỚC BỂ CHỨA BẰNG ARDUINO (RELAY BẬT MÁY BƠM NƯỚC)
 * Đấu nối:
 *         Arduino          |           SRF04
             D2             |           Trig
             D3             |           Echo
 *         Arduino          |         LCD_I2C
             A4             |           SCL
             A5             |           SDA
 *         Arduino          |          Relay
             D5             |           IN
 *         Arduino          |        Còi chíp
             D4             |       Chân dương
 *_____________________________________________________________________________________________________________*/
#include <Wire.h>                           // Gọi thư viện giao tiếp với I2C
#include <LiquidCrystal_I2C.h>              // Gọi thư viện LCD giao tiếp với I2C

#define LCD_ADDRESS 0x27                    // Khai báo địa chỉ I2C của mô-đun LCD
#define LCD_COLUMNS 16                      // 16 cột
#define LCD_ROWS 2                          // 2 hàng
#define KHOANG_CACH 30                      // Khoảng cách để bật máy bơm
const int trigPin = 2;                      // Khai báo chân số 2 kết nối cảm biến siêu âm
const int echoPin = 3;                      // Khai báo chân số 3 kết nối cảm biến siêu âm
const int speakPin = 4;                     // Khai báo chân kết nối với còi báo
const int relayPin = 5;                     // Khai báo chân kết nối với relay
long distance = 0;                          // Khai báo biến lưu trữ khoảng cách đo được từ cảm biến siêu âm
long duration;                              // Khai báo biến đọc thời gian
int test=0;

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);        // Khởi tạo đối tượng LCD

void setup() {
  Serial.begin(9600);                       // Kích hoạt cổng Serial để gửi thông báo đến máy tính
  pinMode(trigPin, OUTPUT);                 // Khai báo chân Trig là OUTPUT 
  pinMode(echoPin, INPUT);                  // Khai báo chân Echo INPUT
  pinMode(speakPin, OUTPUT);                // Đặt chân loa là chân đầu ra
  pinMode(relayPin, OUTPUT);                // Đặt chân relay là chân đầu ra
  lcd.begin(LCD_COLUMNS, LCD_ROWS);         // Khởi tạo LCD
  lcd.backlight();                          // Bật đèn nền LCD
  lcd.setCursor(0,0);
  lcd.print("Banlinhkien.com");
  delay(2000);
}

void loop() {
  digitalWrite(trigPin, LOW);               // Gửi xung Trig trong 10ms để kích hoạt cảm biến
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);        // Đọc thời gian trả về của xung Echo
  distance = duration * 0.034 / 2;          // Chuyển đổi thời gian thành khoảng cách (cm)
  Serial.print("Khoang cach: ");            // Hiển thị kết quả lên Serial Monitor
  Serial.print(distance);
  Serial.println(" cm");
  lcd.clear();
    lcd.setCursor(0, 0);                    // Đặt con trỏ về vị trí hàng 0 cột 0
    lcd.print("Khoang cach: ");             // Hiển thị ra màn hình LCD
    lcd.setCursor(0, 1);                    // Đặt con trỏ về vị trí cột 0 hàng 1
    lcd.print("      ");                    // Hiển thị ra màn hình LCD
    lcd.setCursor(0, 1);
    lcd.print(distance);
    lcd.print("cm");
    
  if (distance > KHOANG_CACH) {             // Nếu khoảng cách nhỏ hơn KHOANG CACH (nước cạn)
    digitalWrite(relayPin, HIGH);           // Bật relay điều khiển máy bơm
    if(test ==0 ){
      lcd.clear();
      lcd.setCursor(2,1);                     // Đưa con trỏ về cột 2 hàng 1
      lcd.print("Bat May Bom");               // In ra màn hình LCD
      digitalWrite(speakPin, HIGH);         // Bật còi báo 
      delay(300);
      digitalWrite(speakPin, LOW);
      delay(300);
      digitalWrite(speakPin, HIGH);        
      delay(300);
      digitalWrite(speakPin, LOW);
    }
    test = 1;
  } else  { 
    
    test=0;
  }
  if ( distance < 5){
     digitalWrite(relayPin, LOW);          // Tắt relay
     lcd.clear();
     lcd.setCursor(5,1);                   // Đưa con trỏ về cột 1 hàng 1
     lcd.print("Tat May Bom");             // In ra màn hình LCD
  }
  
  delay(1000);                              // Đợi 1 giây trước khi đo lại
}
