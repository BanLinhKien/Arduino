#include <LiquidCrystal.h>              // Thư viện LCD 
#include <SPI.h>                        // Thư viện SPI cho giao tiếp với RFID
#include <MFRC522.h>                    // Thư viện RFID

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);    // Khởi tạo đối tượng LCD với các chân: RS, E, D4, D5, D6, D7

#define SS_PIN 10                       // Pin kết nối RFID
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);                   // Khởi động Serial Monitor tốc độ truyền là 9600 bps  
  lcd.begin(16, 2);                     // Khởi động LCD
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID");
  SPI.begin();                          // Khởi động giao tiếp SPI và RFID       
  rfid.PCD_Init();                      // Khởi tạo RFID
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {  // Kiểm tra nếu có thẻ RFID nào được quét
    return;                             // Không có thẻ
  }
  if (!rfid.PICC_ReadCardSerial()) {
    return;                             // Không đọc được dữ liệu thẻ
  }
  lcd.clear();                          // Xóa màn hình và hiển thị thông báo
  lcd.setCursor(0, 0);
  lcd.print("UID:");

  // Hiển thị UID của thẻ lên Serial Monitor và LCD
  Serial.print("UID Tag: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    lcd.print(rfid.uid.uidByte[i], HEX);      // Hiển thị mã UID ở dạng HEX trên LCD
    
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);   // Hiển thị mã UID ở dạng HEX trên Serial
  }
  Serial.println();                           
  delay(2000);                                // Giữ kết quả trên màn hình LCD trong 2 giây
  rfid.PICC_HaltA();                          // Dừng thẻ RFID sau khi đọc xong
}
