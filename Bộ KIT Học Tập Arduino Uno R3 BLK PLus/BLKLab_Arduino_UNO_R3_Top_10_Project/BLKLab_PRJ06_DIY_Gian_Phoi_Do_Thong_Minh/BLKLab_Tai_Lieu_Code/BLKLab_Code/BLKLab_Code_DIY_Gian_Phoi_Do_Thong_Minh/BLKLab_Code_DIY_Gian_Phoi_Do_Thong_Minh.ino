
/* =================================================================================================*
 *                                          BANLINHKIEN.COM                                         *
 *==================================================================================================*                        
 *                                PRJ06: DIY Giàn Phơi Đồ Thông Minh
 * 
 * Đấu nối: 
 * Arduino UNO R3 vs LCD 16x2 I2C
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 *
 * Arduino UNO R3 vs Module Điều khiển Động Cơ L298
 * 5V <-> VCC  ;   GND <-> GND
 * D2 <-> IN4  ;   D3 <-> IN3  ;   D4 <-> IN2  ;   D5 <-> IN1  
 *
 * Arduino UNO R3 vs Module DHT11
 * 5V <-> VCC  ;   GND <-> GND  ;   D8 <-> Out
 * 
 * Arduino UNO R3 vs Module Thu Hồng Ngoại 1838T MH-R38 
 * 5V <-> VCC  ;   GND <-> GND  ;   D9 <-> S
 * 
 * Arduino UNO R3 vs Module Cảm Biến Mưa
 * 5V <-> VCC  ;   GND <-> GND  ;   D7 <-> D0
 * 
 * Arduino UNO R3 vs Module Cảm Biến Ánh Sáng MS-CDS05      
 * 5V <-> VCC  ;   GND <-> GND  ;   A0 <-> A0
 */
 
#include <EEPROM.h>                     // Thư viện EEPROM
#include <Stepper.h>                    // Thư viện điều khiển động cơ bước
#include <IRremote.h>                   // Thư viện xử lý tín hiệu hồng ngoại
#include <DHT.h>                        // Thư viện cảm biến DHT11
#include <LiquidCrystal_I2C.h>          // Thư viện màn hình LCD I2C
#include <Wire.h>                       // Thư viện I2C

// Định nghĩa hằng số
const int STEPS_PER_REV = 2048;         
const int EXTEND_REVOLUTIONS = 5;       
const int RETRACT_REVOLUTIONS = 5;      
const int HUMIDITY_THRESHOLD = 80;      // Ngưỡng độ ẩm (%)

// Định nghĩa chân kết nối
const int rainPin = 7;                  
const int extendButtonPin = 11;         
const int retractButtonPin = 10;        
const int irPin = 9;                    
const int motorPin1 = 5;                
const int motorPin2 = 4;                
const int motorPin3 = 3;                
const int motorPin4 = 2;                
const int ldrPin = A0;                   // Chân cảm biến ánh sáng (D0, digital)
const int dhtPin = 8;                    // Chân cảm biến DHT11

// Mã hồng ngoại
const uint32_t EXTEND_CODE = 4127850240;  
const uint32_t RETRACT_CODE = 4161273600; 

// Khởi tạo đối tượng
Stepper myStepper(STEPS_PER_REV, motorPin1, motorPin3, motorPin2, motorPin4);
DHT dht(dhtPin, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);     // Địa chỉ I2C mặc định: 0x27, LCD 16x2

// Biến trạng thái
bool extended = false;                  
bool retracted = true;                  
float temperature = 0.0;
float humidity = 0.0;
bool isBright = false;

// Địa chỉ lưu trữ trong EEPROM
const int EEPROM_ADDRESS = 0;

void setup() {
  pinMode(rainPin, INPUT);              
  pinMode(extendButtonPin, INPUT_PULLUP);
  pinMode(retractButtonPin, INPUT_PULLUP);
  pinMode(ldrPin, INPUT);
  myStepper.setSpeed(19);               
  IrReceiver.begin(irPin, true, 13);    
  Serial.begin(9600);                   
  dht.begin();
  lcd.begin(16,2);
  lcd.backlight();
  
  // Đọc trạng thái từ EEPROM khi khởi động
  byte state = EEPROM.read(EEPROM_ADDRESS);
  if (state == 1) {
    extended = true;
    retracted = false;
  } else {
    extended = false;
    retracted = true;
  }
  
  Serial.println("Hệ thống sẵn sàng!");
  Serial.println("Trạng thái ban đầu: " + String(extended ? "Extended" : "Retracted"));
  updateLCD();
}

void loop() {
  // Đọc cảm biến
  bool isRaining = digitalRead(rainPin) == LOW;
  isBright = digitalRead(ldrPin) == LOW; // Đảo ngược: LOW là sáng, HIGH là tối
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  // Kiểm tra lỗi cảm biến DHT
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Lỗi đọc cảm biến DHT11!");
    return;
  }
  
  // Cập nhật LCD
  updateLCD();
  
  // Xử lý logic theo điều kiện
  // 1. Trời tối hoặc mưa hoặc độ ẩm cao: Thu vào
  if ((!isBright || isRaining || humidity > HUMIDITY_THRESHOLD) && extended) {
    retract();
    return;
  }
  
  // 2. Trời sáng, không mưa, độ ẩm thấp: Phơi ra
  if (isBright && !isRaining && humidity <= HUMIDITY_THRESHOLD && !extended) {
    extend();
    return;
  }
  
  // 3. Trời tối, không mưa: Giữ trạng thái thu
  if (!isBright && !isRaining && extended) {
    retract();
    return;
  }
  
  // 4. Điều khiển nút nhấn (chỉ hoạt động khi trời sáng và không mưa)
  if (isBright && !isRaining) {
    static bool lastExtendButton = HIGH;
    bool currentExtendButton = digitalRead(extendButtonPin);
    if (currentExtendButton == LOW && lastExtendButton == HIGH) {
      delay(200);
      if (extended) { // Trời sáng nhấn nút: Thu vào
        retract();
      }
    }
    lastExtendButton = currentExtendButton;

    static bool lastRetractButton = HIGH;
    bool currentRetractButton = digitalRead(retractButtonPin);
    if (currentRetractButton == LOW && lastRetractButton == HIGH) {
      delay(200);
      if (extended) {
        retract();
      }
    }
    lastRetractButton = currentRetractButton;
  }
  
  // 5. Điều khiển hồng ngoại (chỉ cho phép phơi khi trời sáng và không mưa)
  if (IrReceiver.decode()) {
    uint32_t dataRemote = IrReceiver.decodedIRData.decodedRawData;
    Serial.println("Tín hiệu IR: " + String(dataRemote));
    
    if (isBright && !isRaining && dataRemote == EXTEND_CODE && !extended) {
      extend();
    } else if (dataRemote == RETRACT_CODE && extended) {
      retract();
    }
    
    delay(500);
    IrReceiver.resume();
  }
}

void extend() {
  if (!extended) {
    Serial.println("Phơi ra...");
    myStepper.step(STEPS_PER_REV * EXTEND_REVOLUTIONS);
    extended = true;
    retracted = false;
    EEPROM.update(EEPROM_ADDRESS, 1);
    updateLCD();
  }
}

void retract() {
  if (!retracted) {
    Serial.println("Thu vào...");
    myStepper.step(-STEPS_PER_REV * RETRACT_REVOLUTIONS);
    retracted = true;
    extended = false;
    EEPROM.update(EEPROM_ADDRESS, 0);
    updateLCD();
  }
}

void updateLCD() {
  lcd.clear();
  // Dòng 1: Nhiệt độ và độ ẩm
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.write(223);
  lcd.print("C H:");
  lcd.print(humidity, 1);
  lcd.print("%");
  
  // Dòng 2: Trạng thái cảm biến và giàn phơi
  lcd.setCursor(0, 1);
  lcd.print(digitalRead(rainPin) == LOW ? "Mua|" : "Nang|");
  lcd.print(isBright ? "Sang|" : "Toi|");
  lcd.print(extended ? "Phoi" : "Thu");
  delay(500);
}
