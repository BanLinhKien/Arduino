/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 * Code: Giao tiếp Keypad 4x4 với Arduino - How to interface Keypad 4x4 with Arduino
 * Đấu nối: 
 + RFID:
  -D13: nối chân SCK của RFID      
  -D12: nối chân MISO của RFID
  -D11: nối chân MOSI của RFID      
  -D10: nối chân SS(DSA)của RFID  
  -D9: nối chân RST của RFID 
 + LCD:
  -D3: nối chân RS của LCD  
  -D4: nối chân ENABLE của LCD
  -D5: nối chân D4 của LCD     
  -D6: nối chân D5 của LCD 
  -D7: nối chân D6 của LCD 
  -D8: nối chân D7 của LCD   
 + Loa BUZZER:
  -A0: nối chân Anot của loa   
 + Động cơ Servo:
  -D2: nối chân tín hiệu của động cơ Servo
 * Các hàm sử dụng:
 - getRFID(): Đọc mã RFID từ thẻ và trả về dưới dạng chuỗi.
 - addParkedRFID(): Thêm mã RFID vào danh sách các thẻ đã đỗ.
 - removeParkedRFID(): Xóa mã RFID khỏi danh sách các thẻ đã đỗ.
 - buzzBuzzer(): Kích hoạt còi kêu trong một khoảng thời gian cố định.
 - startBuzzer(): Bắt đầu kêu còi và lưu thời gian bắt đầu.
 - stopBuzzer(): Dừng còi kêu và đặt thời gian bắt đầu về 0.
 - isBuzzerOn(): Kiểm tra xem còi kêu có đang hoạt độ
 */
 
//Định nghĩa các chân kết nối cho các module:
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 2
#define BUZZER_PIN A0
#define LCD_RS 3
#define LCD_ENABLE 4
#define LCD_D4 5
#define LCD_D5 6
#define LCD_D6 7
#define LCD_D7 8

//Khởi tạo các đối tượng (instances) cho các module:
MFRC522 mfrc522(SS_PIN, RST_PIN);                                       // Tạo đối tượng cho đọc thẻ RFID
Servo servo;                                                            // Tạo đối tượng cho Servo
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);  // Tạo đối tượng cho màn hình LCD

int availableSlots = 3;                                                 // Số lượng chỗ đỗ trống ban đầu
String parkedRFIDs[3];                                                  // Mảng để lưu trữ thông tin về các thẻ RFID đã đỗ

// Thêm biến để theo dõi thời gian còi kêu
unsigned long buzzerStartTime = 0;
const unsigned long buzzerDuration = 5000;                              // Thời gian kêu còi: 5 giây

void setup() 
{
  Serial.begin(9600);
  SPI.begin();                                                          // Khởi tạo bus SPI
  mfrc522.PCD_Init();                                                   // Khởi tạo đọc thẻ RFID

  servo.attach(SERVO_PIN);
  servo.write(0);                                                       // Đóng Servo ban đầu
  
  pinMode(BUZZER_PIN, OUTPUT);
  
  lcd.begin(16, 2);                                                     // Khởi tạo màn hình LCD 16x2
  lcd.print("Smart Parking");
  lcd.setCursor(0, 1);
  lcd.print("Available: " + String(availableSlots));
}

void loop() 
{
  // Kiểm tra nếu có thẻ RFID mới được đưa vào
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) 
  {
    String rfid = getRFID();                                            // Đọc mã RFID từ thẻ

    if (rfid != "") 
    {
      lcd.clear();
      lcd.print("RFID: " + rfid);

      bool isParkedCar = false;
      for (int i = 0; i < 3; i++) 
      {
        // Xử lý thẻ đã đọc
        if (rfid == parkedRFIDs[i]) 
        {
          isParkedCar = true;
          break;
        }
      }

      if (isParkedCar) 
      {
        servo.write(90); // Open the servo
        lcd.setCursor(1, 1);
        lcd.print("See You Again!");
        buzzBuzzer(1000);  
        delay(2000);
        lcd.clear();
        lcd.print("Available: " + String(++availableSlots));
        delay(1000);

        servo.write(0); // Close the servo
        removeParkedRFID(rfid);
      } 
      else if (availableSlots > 0) 
      {
        servo.write(90); // Open the servo
        lcd.setCursor(4, 1);
        lcd.print("Welcome!");
        buzzBuzzer(1000);  
        delay(2000);
        lcd.clear();
        lcd.print("Available: " + String(--availableSlots));
        delay(1000);

        servo.write(0); 
        addParkedRFID(rfid);
      } 
      else 
      {
        lcd.setCursor(3, 1);
        lcd.print("Full Slot");
        buzzBuzzer(5000);  
        delay(2000);
      }

      lcd.clear();
      lcd.print("Smart Parking");
      lcd.setCursor(0, 1);
      lcd.print("Available: " + String(availableSlots));
    }
    // Kiểm tra và điều khiển còi kêu
    if (isBuzzerOn()) 
    {
      if (millis() - buzzerStartTime >= buzzerDuration) 
      {
        stopBuzzer();
      }
    }
    mfrc522.PICC_HaltA();                                               // Dừng truyền thẻ RFID
  }
}

String getRFID() 
{
  String rfid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    rfid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    rfid.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  mfrc522.PICC_HaltA();
  return rfid;
}

void addParkedRFID(String rfid) 
{
  for (int i = 0; i < 3; i++) 
  {
    if (parkedRFIDs[i] == "") 
    {
      parkedRFIDs[i] = rfid;
      break;
    }
  }
}

void removeParkedRFID(String rfid) 
{
  for (int i = 0; i < 3; i++) 
  {
    if (parkedRFIDs[i] == rfid) 
    {
      parkedRFIDs[i] = "";
      break;
    }
  }
}

void buzzBuzzer(unsigned int duration) 
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void startBuzzer() 
{
  digitalWrite(BUZZER_PIN, HIGH);
  buzzerStartTime = millis();                                           // Lưu thời gian bắt đầu kêu còi

}

void stopBuzzer() 
{
  digitalWrite(BUZZER_PIN, LOW);
  buzzerStartTime = 0;                                                  // Đặt thời gian bắt đầu kêu còi về 0
}

bool isBuzzerOn() 
{
  return buzzerStartTime > 0;
}
