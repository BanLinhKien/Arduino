  /********************************************************************************
 *                            BANLINHKIEN.COM                                   *
 ********************************************************************************                            
 *               HƯỚNG DẪN HỆ THỐNG PHÁT HIỆN LỬA VÀ KHÍ GAS 
 *                 ĐIỀU KHIỂN ĐÓNG MỞ CỦA VÀ BẬT QUẠT HÚT           
 ********************************************************************************/

#include <Servo.h>
#include <SimpleKalmanFilter.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
SimpleKalmanFilter bo_loc(2, 2, 0.001);
const int rs = 12, en = 9, d4 = 8, d5 = 7, d6 = A6, d7 = A7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Chân RS, Enable, D4, D5, D6, D7

// Dinh nghia các chân kết nối
#define mq2Pin A0
#define mhirPin A1
#define buzzerPin 2        // còi báo
#define servo1Pin 6        // servo1 đóng mở cửa
#define servo2Pin 5        // servo2 đóng mở cửa
#define relay1Pin 3        // quạt
#define relay2Pin 4        // bơm
// Khai báo các Nut nhan
int buttonPin = A5;             // nút nhấn 
int len = A4;     int gtlen;    // Lên là trừ
int xuong = A3;   int gtxuong;  // Xuống là cộng
int menu = A2;    int gtmenu; 
int macdinh = 1;

// Cac bien su dung
int congtru_tong = 0; int congtru_menu1 = 0; int congtru_menu2 = 0;
int demmenu1 = 0; int demmenu2 = 0;
int demtong = 0;  

int gtcai;                                          // Gia tri hien thi LCD
int x = 0, y = 0, z = 0;                            // Biến đếm số lần nút được nhấn
int smokeThreshold;                                 // Giá trị ngưỡng cho mức khí độc
int smoke, mhir; 
int manual, autoo;
Servo servo1;                                       // Khởi tạo đối tượng động cơ servo
Servo servo2; 

void manhinh()      // Màn hình chính
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HE THONG AN TOAN"); 
  lcd.setCursor(0, 1);
  lcd.print("GAS: ");
  lcd.setCursor(5, 1);
  lcd.print(analogRead(A0));
}
void canhbao()      // Màn hình cảnh báo
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    CANH BAO    ");
}

void menu_tong()    // Hiển thị menu
{
  if (congtru_tong == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">QUAY LAI");
    lcd.setCursor(0, 1);
    lcd.print(" CHE DO");//Chedo  
  }
  else if (congtru_tong == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" QUAY LAI");
    lcd.setCursor(0, 1);
    lcd.print(">CHE DO");    
  }
  else if (congtru_tong == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">CAI NGUONG");//cai nguong  
  }
}
 
void menu_1()  
{
  if (congtru_menu1 == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">QUAY LAI");
    lcd.setCursor(0, 1);
    lcd.print(" TU DONG");  //Auto
  }
  else if (congtru_menu1 == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" QUAY LAI");
    lcd.setCursor(0, 1);
    lcd.print(">TU DONG");    
  }
  else if (congtru_menu1 == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">DIEU KHIEN");//Manual
  }     
}

void writeThreshold(int threshold) {                    // Ghi ngưỡng cảnh báo vào EEPROM
  EEPROM.write(10, threshold / 100);                    // Ghi chữ số hàng trăm
  EEPROM.write(11, (threshold % 100) / 10);             // Ghi chữ số hàng chục
  EEPROM.write(12, threshold % 10);                     // Ghi chữ số hàng đơn vị
}
int readGasThreshold() {                                // Đọc ngưỡng cảnh báo từ EEPROM
  int threshold = 100 * EEPROM.read(10) + 10 * EEPROM.read(11) + EEPROM.read(12);
  Serial.print("Nguong canh bao: ");
  Serial.println(threshold);
  return threshold;
}

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(mq2Pin, INPUT);                         // Khởi tạo chân kết nối với MQ2
  pinMode(mhirPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(len, INPUT);
  pinMode(xuong, INPUT);
  pinMode(menu, INPUT);
  manhinh();
  Serial.println("Connected!");  
  digitalWrite(buzzerPin, HIGH);                   // Bật còi
  delay(1500);
  digitalWrite(buzzerPin, LOW);
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(180);                              // Đưa động cơ servo về vị trí ban đầu
  servo2.write(90);
  autoo = 1;
  manual = 0;
}

void loop() 
{
  gtcai = readGasThreshold();
  gtlen = digitalRead(len);  
  gtxuong = digitalRead(xuong);  
  gtmenu = digitalRead(menu);
  
  smokeThreshold = readGasThreshold();            // Đọc giá trị ngưỡng
  smoke = analogRead(mq2Pin);                     // Đọc giá trị từ cảm biến MQ2
  mhir = digitalRead(mhirPin);
  smoke = bo_loc.updateEstimate(smoke);
  Serial.print("Smoke: ");                        // In giá trị lên Serial Monitor để theo dõi
  Serial.println(smoke);

  if (demtong == 0) {
    if (autoo == 0 & manual == 1) {   // chế độ MANUAL (dieu khien)
      manhinh();
      if (digitalRead(len) == 0) {           // Nếu nút lên được nhấn
        x++;
        delay(50);
      }
      if (digitalRead(xuong) == 0) {         // Nút xuống được nhấn
        y++;
        delay(50);
      }
      if (digitalRead(buttonPin) == 0) {       // Nút on/off được nhấn
        z++;
        delay(50);
      }
      if (y % 2 == 1){            // Đóng mở cửa
        door_open();
      } else {
        door_close();
      }
      if (z % 2 == 1){            // Bật tắt quạt
        relay1_on();
        buz_on();
      } else {
        relay1_off();
        buz_off();
      }      
      if (x % 2 == 1){            // Bật tắt bơm
        relay2_on();
      } else {
        relay2_off();
      }
    }
    if (autoo == 1 & manual == 0) {                           // Chế độ AUTO (tu dong)
      if (smoke > smokeThreshold && mhir == 0) {              // Kiểm tra khí gas vượt quá ngưỡng và có lửa
        door_open();
        relay1_on();
        relay2_on();
        buz_on();
        lcd.setCursor(0, 1);
        lcd.print(" RO RI KHI GAS ");
        delay(200);
      }
      else if (smoke > smokeThreshold && mhir == 1) {         // Có Gas vượt ngưỡng, không có lửa
        door_open();
        relay1_on();
        relay2_off();
        buz_on();
        canhbao();        // Màn hình cảnh báo
        lcd.setCursor(0, 1);
        lcd.print(" RO RI KHI GAS ");
        delay(200);
      }
      else if (smoke < smokeThreshold - 10 && mhir == 0) {    // Có lửa, Gas bé hơn ngưỡng
        door_open();
        relay1_off();
        relay2_on();
        buz_on();
        canhbao();        // Màn hình cảnh báo
        lcd.setCursor(0, 1);
        lcd.print("CO LUA NGUY HIEM");
        delay(200);
      }
      else if (smoke < smokeThreshold - 10 && mhir == 1) {    // Không lửa, Gas bé hơn ngưỡng
        manhinh();
        door_close();
        relay1_off();
        relay2_off();
        buz_off();
      }
      if (digitalRead(buttonPin) == 0) {       // Nút on/off được nhấn
        autoo = 0;
        manual = 1;
        Serial.println("Ve che do Manual");
      }
    }
    delay(100);
  }

  if (gtlen != macdinh){                        // NÚT LÊN
   if (gtlen == 0){                             // Khi nhấn nút lên
    if (demtong == 1){                          // Len o menu tong 
      if (congtru_tong >= 2){ 
        congtru_tong = 0;  
      }
      else{ 
        congtru_tong++;  
      }   
      menu_tong();
    }
    else if (demtong == 2 && congtru_tong == 1){  // menu 1
      if (congtru_menu1 >= 2){ 
        congtru_menu1 = 0;  
      }
      else{ 
        congtru_menu1++;  
      }   
      menu_1();
    }
    else if (demtong == 2 && congtru_tong == 2) {   // menu 2
      if (gtcai <= 0){ 
        gtcai = 0;  
      }
      else{ 
        gtcai = gtcai - 10;  
        lcd.setCursor(9,1);
        lcd.print(gtcai);
        writeThreshold(gtcai);
      }
    }  
    delay(200);
  }
  macdinh = gtlen;
  }
  
  if (gtxuong != macdinh){                   // NÚT XUỐNG
   if (gtxuong == 0){                        //Khi nhấn nút xuống
    if (demtong == 1){                       // menu tong
      if (congtru_tong <= 0){ 
        congtru_tong = 2;  
      }
      else{ 
        congtru_tong--;  
      }
      menu_tong();
    }
    else if (demtong == 2 && congtru_tong == 1){
      if (congtru_menu1 <= 0){ 
        congtru_menu1 = 2;  
      }
      else{ 
        congtru_menu1--;  
      }
      menu_1();
    }
    else if (demtong == 2 && congtru_tong == 2){
        gtcai = gtcai + 10;  
        lcd.setCursor(9,1);
        lcd.print(gtcai);
        writeThreshold(gtcai);
    }  
    delay(200);
  }
  macdinh = gtxuong;
  }
  
  if (gtmenu != macdinh){                           // NÚT MENU  
    if (gtmenu == 0){         //Khi nhấn nút
      demtong ++;
    if (demtong == 1){        //Ở menu tổng 
      menu_tong(); 
    }
    else if (demtong == 2 && congtru_tong == 0){    //Nhấn BACK từ Menu tổng về màn hình
        demtong = 0;
        manhinh(); 
    }
    else if (demtong == 2 && congtru_tong == 1) {   // Ở menu 1
      menu_1();
      demmenu1++;
    }
    else if (demtong == 3 && demmenu1 == 1 && congtru_menu1 == 1) {    //Từ chọn_menu1 về menu 
      demtong = 0;
      congtru_menu1 = 0;
      demmenu1 = 0;
      congtru_tong = 0;
      manhinh();
      autoo = 1;
      manual = 0;
    } 
    else if (demtong == 3 && demmenu1 == 1 && congtru_menu1 == 2) {    //Từ chọn_menu2 về menu 
      demtong = 0;
      congtru_menu1 = 0;
      demmenu1 = 0;
      congtru_tong = 0;
      manhinh();
      autoo = 0;
      manual = 1;
    }           
    else if (demtong == 3 && demmenu1 == 1 && congtru_menu1 == 0 && congtru_tong == 1) { 
      congtru_tong = 0;
      demtong = 1;
      demmenu1 = 0;
      menu_tong();
    }
    else if (demtong == 2 && congtru_tong == 2) //Ở menu 2
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("   CAI NGUONG ");
      lcd.setCursor(0,1);
      lcd.print("GIA TRI: "); 
      lcd.setCursor(9,1);
      lcd.print(gtcai);
    }
    else if (demtong == 3 && congtru_tong == 2) 
    {
      congtru_tong = 0;
      demtong = 0;
      manhinh();
    }        
    delay(200);
  }
  macdinh = gtmenu;
  }
  
} //Kết loop

void relay1_on() {     // Bật quạt                       
  digitalWrite(relay1Pin, HIGH);
}
void relay1_off() {   // Tắt quạt
  digitalWrite(relay1Pin, LOW);
}
void relay2_on() {    // Bật bơm
  digitalWrite(relay2Pin, HIGH);
}
void relay2_off() {   // Tắt bơm
  digitalWrite(relay2Pin, LOW);
}
void door_open() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(90);                             // Quay động cơ servo 1 về giữa
  servo2.write(180);                            // Quay động cơ servo 2 về giữa
  delay(50);
  servo1.detach();
  servo2.detach();
}
void door_close() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(180);                            // Động cơ servo 1 ở vị trí ban đầu
  servo2.write(90);                             // Động cơ servo 2 ở vị trí ban đầu
  delay(50);
  servo1.detach();
  servo2.detach();
}
void buz_on() {
  digitalWrite(buzzerPin, HIGH);                // Bật còi
}
void buz_off() {
  digitalWrite(buzzerPin, LOW);                // Tắt còi
}


/*=================================================================================================*/
