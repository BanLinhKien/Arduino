/********************************************************************************
 *                            BANLINHKIEN.COM                                   *
 ********************************************************************************                            
 *               HƯỚNG DẪN HỆ THỐNG PHÁT HIỆN LỬA VÀ KHÍ GAS 
 *                 ĐIỀU KHIỂN ĐÓNG MỞ CỦA VÀ BẬT QUẠT HÚT           
 ********************************************************************************/

#include <Servo.h>
#include <SimpleKalmanFilter.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
SoftwareSerial sim(10, 11); // tx 10 rx 11
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

int congtru_tong = 0; int congtru_menu1 = 0; int congtru_menu2 = 0;
int demmenu1 = 0; int demmenu2 = 0;
int demtong = 0;  

// Cac bien su dung
int gtcai;                                          // Gia tri hien thi LCD
int x = 0, y = 0, z = 0;                            // Biến đếm số lần nút được nhấn
int smokeThreshold;                                 // Giá trị ngưỡng cho mức khí độc
int smoke, mhir, t, a; 
int manual, autoo;
String phoneNumber;                                 // Số gọi đến
String sms;
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

void writePhoneNumber(String phoneNumber) {         // Ghi số điện thoại vào EEPROM
  for (int i = 0; i < 5; i++) {                     
    int startPos = i * 2;
    String pair = phoneNumber.substring(startPos, startPos + 2);
    int number = pair.toInt();
    EEPROM.write(i, number);
  }
  Serial.print("So dien thoai: ");
  Serial.println(phoneNumber);
}

String readPhoneNumber() {                          // Đọc số điện thoại từ EEPROM
  String phoneNumber = "";
  for (int i = 0; i < 5; i++) {
    int number1 = EEPROM.read(i);
    if (number1 < 10){
      phoneNumber += "0" + String(number1);
    }
    else {
      phoneNumber += String(number1);
    }
  }
  Serial.println(phoneNumber);
  return phoneNumber;
}

// Tách chuỗi
String nameSim;
String paraSim;
void setPhoneNumber_Threshold(String sms) {
    //Serial.println(sms);
    char  str_sms[50];
    sms.toCharArray(str_sms, sms.length()+1);
    char * p;
    p = strtok(str_sms, "_");       //Tách chuỗi con lần đầu tiên
    nameSim = p;
    Serial.println(nameSim);
    //Bằng cách sử dụng hàm strok cho tới khi kết quả NULL được trả về.
    while(p != NULL) {
        p = strtok(NULL, "_");
        if(p != NULL) {
           //  Serial.println(paraSim);
           paraSim = p;
        }
    }
    Serial.println(paraSim);
    Serial.println(paraSim.length());    
    if (nameSim == "S") {
      Serial.println("DD " + paraSim.length());
      if (paraSim.length() < 10 || paraSim.length() > 11) {
//        phanhoi2(0);
      } else {
        writePhoneNumber(paraSim);
        Serial.println("SDT: "+paraSim);
        digitalWrite(buzzerPin, HIGH);                // Bật còi
        delay(1000);
        digitalWrite(buzzerPin, LOW);
        writePhoneNumber(paraSim);
        phoneNumber = readPhoneNumber();
        phanhoi(9);
        delay(1000);
      }
    } 
    else if (nameSim == "N" ) {
       //Serial.println("paraSim"+paraSim);
      int threshold = paraSim.toInt();
      //Serial.println("nn"+threshold);
      writeThreshold(threshold);
      smokeThreshold = threshold;
      Serial.println("Nguong: "+threshold);
      digitalWrite(buzzerPin, HIGH);                // Bật còi
      delay(1000);
      digitalWrite(buzzerPin, LOW);
      writeThreshold(threshold);
      phanhoi(10);
      delay(1000);
    }
}

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  sim.begin(9600);
  pinMode(mq2Pin, INPUT);                         // Khởi tạo chân kết nối với MQ2
  pinMode(mhirPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(len, INPUT);
  pinMode(xuong, INPUT);
  pinMode(menu, INPUT);
  sim.println("AT");
  manhinh();
  /*while(sim.readStringUntil('\n') != "OK"){
    sim.println("AT");
    Serial.println("Connecting..."); // kết nối
  }*/
  while(!sim.available()){
    sim.println("AT");
    delay(1000); 
    Serial.println("Connecting..."); // kết nối
  }
  Serial.println("Connected!");  
  digitalWrite(buzzerPin, HIGH);                   // Bật còi
  delay(1500);
  digitalWrite(buzzerPin, LOW);
  sim.println("AT+IPR=9600");                      // khởi động chức năng SMS
  delay(1000);
  sim.println("AT+CMGF=1");                       // khởi động chức năng SMS
  delay(1000);  
  sim.println("AT+CNMI=1,2,0,0,0"); 
  delay(1000);
  sim.println("AT+CMGL=\"REC UNREAD\"");          // đọc tin nhắn tới
  delay(1000);
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
  phoneNumber = readPhoneNumber();                // Đọc số điện thoại
  smoke = analogRead(mq2Pin);                     // Đọc giá trị từ cảm biến MQ2
  mhir = digitalRead(mhirPin);
  smoke = bo_loc.updateEstimate(smoke);
  Serial.print("Smoke: ");                        // In giá trị lên Serial Monitor để theo dõi
  Serial.println(smoke);
  delay(100);

  if (demtong == 0) {
    if (autoo == 0 & manual == 1) {
      manhinh();
      if (digitalRead(len) == 0) {
        x++;
        delay(50);
      }
      if (digitalRead(xuong) == 0) {
        y++;
        delay(50);
      }
      if (digitalRead(buttonPin) == 0) {
        z++;
        delay(50);
      }
      if (y % 2 == 1){
        door_open();
      } else {
        door_close();
      }
      if (z % 2 == 1){
        relay1_on();
        buz_on();
      } else {
        relay1_off();
        buz_off();
      }      
      if (x % 2 == 1){
        relay2_on();
      } else {
        relay2_off();
      }
    }
    if (autoo == 1 & manual == 0) {
      if (smoke > smokeThreshold && mhir == 00) {                   // Kiểm tra khí gas vượt quá ngưỡng
        door_open();
        relay1_on();
        relay2_on();
        buz_on();
        canhbao();        // Màn hình cảnh báo
        lcd.setCursor(0, 1);
        lcd.print(" RO RI KHI GAS ");
        delay(200);
        if (t<=2) { 
          callNumber1();        
          SendMessage(2);  // chuong trinh con nhắn tin 
          delay(500);
          t++; 
          for ( int i=0; i<100; i++){
            smoke = analogRead(mq2Pin); 
            smoke = bo_loc.updateEstimate(smoke);
          }
        }
      }
      else if (smoke > smokeThreshold && mhir == 1) {
        door_open();
        relay1_on();
        relay2_off();
        buz_on();
        canhbao();        // Màn hình cảnh báo
        lcd.setCursor(0, 1);
        lcd.print(" RO RI KHI GAS ");
        delay(200);
        if (t<=2) { 
          callNumber1();
          SendMessage(1);                             // chuong trinh con nhắn tin  
          delay(500);
          /*if (digitalRead(buttonPin) == 1) {
            delay(20000);
          } else {
            sim.print("ATH"); 
            sim.println(";"); 
          }*/
          t++; 
          for ( int i=0; i<100; i++){
            smoke = analogRead(mq2Pin); 
            smoke = bo_loc.updateEstimate(smoke);
          }
        }
      }
      else if (smoke < smokeThreshold - 10 && mhir == 0) {
        door_open();
        relay1_off();
        relay2_on();
        buz_on();
        canhbao();        // Màn hình cảnh báo
        lcd.setCursor(0, 1);
        lcd.print("CO LUA NGUY HIEM");
        delay(200);
        if (t<=2) { 
          callNumber1();
          SendMessage(0);   // chuong trinh con nhắn tin  
          delay(500);                   
          t++; 
          for ( int i=0; i<100; i++){
            smoke = analogRead(mq2Pin); 
            smoke = bo_loc.updateEstimate(smoke);
          }
        }
      }
      else if (smoke < smokeThreshold - 10 && mhir == 1) {
        manhinh();
        door_close();
        relay1_off();
        relay2_off();
        buz_off();
        t = 0;
      }
      if (digitalRead(buttonPin) == 0) {       // Nút on/off được nhấn
        autoo = 0;
        manual = 1;
        Serial.println("Ve che do Manual");
      }
    }
    check_SMS(); 
    delay(200);
    
  }

  if (gtlen != macdinh){                        // NÚT LÊN
   if (gtlen == 0){                             // Khi nhấn nút lên
    if (demtong == 1){                          // Nut len ơ menu chinh  
      if (congtru_tong >= 2){ 
        congtru_tong = 0;  
      }
      else{ 
        congtru_tong++;  
      }   
      menu_tong();
    }
    else if (demtong == 2 && congtru_tong == 1){  // Lên ở menu 1
      if (congtru_menu1 >= 2){ 
        congtru_menu1 = 0;  
      }
      else{ 
        congtru_menu1++;  
      }   
      menu_1();
    }
    else if (demtong == 2 && congtru_tong == 2) {   // Lên ở menu 2
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
    delay(150);
  }
  macdinh = gtlen;
  }
  
  if (gtxuong != macdinh){                   // NÚT XUỐNG
   if (gtxuong == 0){                        //Khi nhấn nút xuống
    if (demtong == 1){                       // Xuống ở menu chính
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
    delay(150);
  }
  macdinh = gtxuong;
  }
  
  if (gtmenu != macdinh){                           // NÚT MENU  
    if (gtmenu == 0){                               // Nút menu được nhấn
      demtong ++;
    if (demtong == 1){                              //Ở menu tổng 
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
    else if (demtong == 3 && demmenu1 == 1 && congtru_menu1 == 2) {    //Từ chọn_menu1 về menu 
      demtong = 0;
      congtru_menu1 = 0;
      demmenu1 = 0;
      congtru_tong = 0;
      manhinh();
      autoo = 0;
      manual = 1;
    }           
    else if (demtong == 3 && demmenu1 == 1 && congtru_menu1 == 0 && congtru_tong == 1) { //Nhấn BACK ở menu 1
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
    else if (demtong == 3 && congtru_tong == 2) //Nhấn BACK ở menu 2
    {
      congtru_tong = 0;
      demtong = 0;
      manhinh();
    }        
    delay(150);
  }
  macdinh = gtmenu;
  }
  
} 
//Kết thúc hàm loop

/*================Các hàm điều khiển đầu OUT===================*/

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
/*------------------------------------------------------------------------*/

/*====================Các hàm gọi, gửi tin nhắn qua SIM===================*/

String smsSD1;
String smsSD2;
void check_SMS() {
  if (sim.available()) {
    sms = sim.readStringUntil('\n');
    //setPhoneNumber_Threshold(sms);                              // Đọc và cập nhật số điện thoại và ngưỡng cảnh báo nếu có
    Serial.println("SMS: " + sms);
    if (sms == "TU DONG\r") {
      autoo = 1;
      manual = 0;
      phanhoi(0);
      Serial.println("Che do tu dong");
      delay(200);
    }
    else if (sms == "DIEU KHIEN\r") {
      autoo = 0;
      manual = 1;
      phanhoi(1);
      Serial.println("Che do dieu khien");
      delay(200);
    }
    else if (sms == "MO CUA\r") {
      door_open();
      y++;
      delay(200);
      phanhoi(2);
      Serial.println("Gui tin mo cua");
    }
    else if (sms == "DONG CUA\r") {
      door_close();
      y++;
      delay(200);
      phanhoi(3);
      Serial.println("Gui tin dong cua");
    }
    else if (sms == "BAT QUAT HUT\r") {
      relay1_on();
      z++;
      delay(200);
      phanhoi(4);
      Serial.println("Gui tin bat quat");
    }
    else if (sms == "TAT QUAT HUT\r") {
      relay1_off();
      z++;
      delay(200);
      phanhoi(5);
      Serial.println("Gui tin tat quat");
    }
    else if (sms == "BAT MAY BOM\r") {
      relay2_on();
      x++;
      delay(200);
      phanhoi(6);
      Serial.println("Gui tin bat bom");
    }
    else if (sms == "TAT MAY BOM\r") {
      relay2_off();
      x++;
      delay(200);
      phanhoi(7);
      Serial.println("Gui tin tat bom");
    }
    else if (sms == "GAS\r") {
      phanhoi(8);
      Serial.println("Gui khi Gas");
    }
    else if (sms == "SDT\r") {
      phanhoi(9);
      Serial.println("Gui so dien thoai");
    }
    else if (sms == "CHE DO\r") {
      phanhoi(11);
      Serial.println("Gui che do hien tai");
    }
    else if (sms == "SO DU\r") {
      sim.println("ATD*101#;"); 
      delay(1000);
      for (int i=0; i<9; i++) {
        smsSD1 = sim.readStringUntil('\n');
        if (smsSD1 != "OK\r") {
          smsSD2 = smsSD1;
        }
        delay(200);
      }
      sim.println("AT+CMGF=1");
      delay(1000);
      sim.println("AT+CMGS=\"" + phoneNumber + "\"\r");         // Gửi tin nhắn đến số điện thoại
      delay(1000);
      sim.println("smsSD2");
      Serial.println("Check so du");
    }
    delay(50);
    setPhoneNumber_Threshold(sms);                              // Đọc và cập nhật số điện thoại và ngưỡng cảnh báo nếu có
  }
}

void SendMessage (int tipe)                                     // chuong trình con nhăn tin 
{
  Serial.println ("Gui tin nhan den dien thoai");
  sim.println("AT+CMGF=1");    
  delay(1500);
  sim.println("AT+CMGS=\"" + phoneNumber + "\"\r");             // Gui tin nhăn đen so dien thoai
  delay(1000);
  switch (tipe) 
  {
    case 0:
      sim.println("He Thong Phat Hien Co Lua !!!");
      delay(100);
      sim.println((char)26);
      delay(500);
      break;
    case 1:
      sim.println("He Thong Phat Hien Khi Gas Vuot Nguong !!!");
      delay(100);
      sim.println((char)26);
      delay(500);
      break;
    case 2:
      sim.println("He Thong Canh Bao Ro Ri Khi Gas, Co Chay. NGUY HIEM !!!");
      delay(100);
      sim.println((char)26);
      delay(500);
      break;   
  }
}

void callNumber1() {                                          // Chuong trinh con gọi dien
  sim.print (F("ATD"));
  sim.print (phoneNumber);
  sim.print (F(";\r\n"));
  Serial.println("Thuc hien cuoc goi");
  Serial.println("SMS: " + sim.readStringUntil('\n'));
}

String smokePre;
void phanhoi(int tipe) {
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + phoneNumber + "\"\r");           // Gửi tin nhắn đến số điện thoại
  switch (tipe) 
  {
    case 0:
      sim.println("Che do TU DONG");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break;
    case 1:
      sim.println("Che do DIEU KHIEN");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break;
    case 2:
      sim.println("Mo Cua");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break; 
    case 3:
      sim.println("Dong Cua");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break;  
    case 4:
      sim.println("Bat Quat Hut");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break;
    case 5:
      sim.println("Tat Quat Hut");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break;
    case 6:
      sim.println("Bat May Bom");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break; 
    case 7:
      sim.println("Tat May Bom");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break;  
    case 8:
      smokePre = String (smoke);
      sim.println("Gas: \"" + smokePre + "\"\r");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break;
    case 9:
      phoneNumber = readPhoneNumber();
      sim.println("SDT: \"" + phoneNumber + "\"\r");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break;
    case 10:
      smokePre = String (readGasThreshold());
      sim.println("Nguong Canh Bao: \"" + smokePre + "\"\r");
      delay(100);
      sim.println((char)26);
      delay(1000);
      break;
    case 11:
      if (autoo == 1 && manual == 0){
        sim.println("Che do TU DONG");
        Serial.println("Che do Auto");
        delay(100);
        sim.println((char)26);
        delay(1000);
      }
      else if (autoo == 0 && manual == 1){
        sim.println("Che do DIEU KHIEN");
        Serial.println("Che do Manual");
        delay(100);
        sim.println((char)26);
        delay(1000);
      }
      break;
  }
}

/*============================================================================================*/
