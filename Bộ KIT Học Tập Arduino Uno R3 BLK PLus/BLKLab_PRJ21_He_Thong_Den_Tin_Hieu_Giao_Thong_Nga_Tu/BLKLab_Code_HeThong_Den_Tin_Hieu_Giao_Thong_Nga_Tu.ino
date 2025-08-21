/*
 * =======================================================================================================================================*
 *                                                              BANLINHKIEN.COM
 * =======================================================================================================================================*                           
 *                                                  Hướng dẫn điều khiển đèn giao thông ngã tư
 * Đấu nối: 
 + Modul Led 7 thanh:
  -D2: chân Arduino điều khiển LED1 (hàng nghìn) của 7SEG                                 -D8: chân Arduino kết nối với thanh E của 7SEG             
  -D3: chân Arduino điều khiển LED2 (hàng trăm) của 7SEG                                  -D9: chân Arduino kết nối với thanh D của 7SEG
  -D4: chân Arduino điều khiển LED3 (hàng chục) của 7SEG                                  -D10: chân Arduino kết nối với thanh C của 7SEG
  -D5: chân Arduino điều khiển LED4 (hàng đơn vị) của 7SEG                                -D11: chân Arduino kết nối với thanh B của 7SEG
  -D6: chân Arduino kết nối với thanh G của 7SEG                                          -D12: chân Arduino kết nối với thanh A của 7SEG
  -D7: chân Arduino kết nối với thanh F của 7SEG
  
 + Led đơn:
  -A0: Chân Arduino điều khiển LED xanh 1                                                 -A4: Chân Arduino điều khiển LED xanh 2 
  -A2: Chân Arduino điều khiển LED vàng 1                                                 -A5: Chân Arduino điều khiển LED vàng 2
  -A3: Chân Arduino điều khiển LED đỏ 1                                                   -A6: Chân Arduino điều khiển LED đỏ 2
 */

const int aPin = 12;                                                                      // chân Arduino kết nối với thanh A của 7SEG
const int bPin = 11;                                                                      // chân Arduino kết nối với thanh B của 7SEG
const int cPin = 10;                                                                      // chân Arduino kết nối với thanh C của 7SEG
const int dPin = 9;                                                                       // chân Arduino kết nối với thanh D của 7SEG
const int ePin = 8;                                                                       // chân Arduino kết nối với thanh E của 7SEG
const int fPin = 7;                                                                       // chân Arduino kết nối với thanh F của 7SEG
const int gPin = 6;                                                                       // chân Arduino kết nối với thanh G của 7SEG

const int led1Pin = 2;                                                                    // chân Arduino điều khiển LED1 (hàng nghìn) của 7SEG
const int led2Pin = 3;                                                                    // chân Arduino điều khiển LED2 (hàng trăm) của 7SEG
const int led3Pin = 4;                                                                    // chân Arduino điều khiển LED3 (hàng chục) của 7SEG
const int led4Pin = 5;                                                                    // chân Arduino điều khiển LED4 (hàng đơn vị) của 7SEG

const int green1Pin = A0;                                                                 // Chân Arduino điều khiển LED xanh 1
const int yellow1Pin = A1;                                                                // Chân Arduino điều khiển LED vàng 1
const int red1LedPin = A2;                                                                // Chân Arduino điều khiển LED đỏ 1
const int green2Pin = A3;                                                                 // Chân Arduino điều khiển LED xanh 2
const int yellow2Pin = A4;                                                                // Chân Arduino điều khiển LED vàng 2
const int red2LedPin = A5;                                                                // Chân Arduino điều khiển LED đỏ 2

unsigned char led7Hex[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};   // Mã LED 7 thanh kiểu anot chung

void LED7_writeByte(unsigned char b) {                                                    // Lấy bit thứ 0 (A), nếu bằng 1 thì đưa ra mức cao
  if (((b >> 0) & 0x01) == 1) {
    digitalWrite(aPin, HIGH);
  } 
  else {                                                                                  // Ngược lại (0) thì đưa ra mức thấp
    digitalWrite(aPin, LOW);
  }

  if (((b >> 1) & 0x01) == 1) {
    digitalWrite(bPin, HIGH);
  } 
  else {
    digitalWrite(bPin, LOW);
  }

  if (((b >> 2) & 0x01) == 1) {
    digitalWrite(cPin, HIGH);
  } 
  else {
    digitalWrite(cPin, LOW);
  }

  if (((b >> 3) & 0x01) == 1) {
    digitalWrite(dPin, HIGH);
  } 
  else {
    digitalWrite(dPin, LOW);
  }

  if (((b >> 4) & 0x01) == 1) {
    digitalWrite(ePin, HIGH);
  } 
  else {
    digitalWrite(ePin, LOW);
  }

  if (((b >> 5) & 0x01) == 1) {
    digitalWrite(fPin, HIGH);
  } 
  else {
    digitalWrite(fPin, LOW);
  }

  if (((b >> 6) & 0x01) == 1) {
    digitalWrite(gPin, HIGH);
  } 
  else {
    digitalWrite(gPin, LOW);
  }
}

void setup() {
  // Cấu hình các chân kết nối với thanh LED 7 thanh là đầu ra
  pinMode(aPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(cPin, OUTPUT);
  pinMode(dPin, OUTPUT);
  pinMode(ePin, OUTPUT);
  pinMode(fPin, OUTPUT);
  pinMode(gPin, OUTPUT);

  // Cấu hình các chân điều khiển LED là đầu ra
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);

  pinMode(green1Pin, OUTPUT);
  pinMode(yellow1Pin, OUTPUT);
  pinMode(red1LedPin, OUTPUT);
  pinMode(green2Pin, OUTPUT);
  pinMode(yellow2Pin, OUTPUT);
  pinMode(red2LedPin, OUTPUT);
}

void hienthi7SEG(int so) 
{
  for (int i = 0; i < 50; i++)                                                            // Quét LED 50 lần để tạo khoảng trễ xấp xỉ 50*5 = 250ms
  { 
    digitalWrite(led1Pin, LOW);
    LED7_writeByte(led7Hex[so / 1000]);                                                   // Hiển thị hàng nghìn
    delay(5);
    digitalWrite(led1Pin, HIGH);

    digitalWrite(led2Pin, LOW);
    LED7_writeByte(led7Hex[(so / 100) % 10]);                                             // Hiển thị hàng trăm
    delay(5);
    digitalWrite(led2Pin, HIGH);

    digitalWrite(led3Pin, LOW);
    LED7_writeByte(led7Hex[(so / 10) % 10]);                                              // Hiển thị hàng chục
    delay(5);
    digitalWrite(led3Pin, HIGH);

    digitalWrite(led4Pin, LOW);
    LED7_writeByte(led7Hex[so % 10]);                                                     // Hiển thị hàng đơn vị
    delay(5);
    digitalWrite(led4Pin, HIGH);
  }
}

 void loop() 
 {
  for (int num = 30; num >= 0; num--) 
  {
    digitalWrite(green1Pin, HIGH);
    digitalWrite(yellow1Pin, LOW);
    digitalWrite(red1LedPin, LOW);
    digitalWrite(green2Pin, LOW);
    digitalWrite(yellow2Pin, LOW);
    digitalWrite(red2LedPin, HIGH);
    hienthi7SEG(num);                                                                     // Gọi hàm hiển thị số trên LED 7 đoạn
  }

  for (int num = 3; num >= 0; num--) 
  {
    digitalWrite(green1Pin, LOW);
    digitalWrite(yellow1Pin, HIGH);
    digitalWrite(red1LedPin, LOW);
    digitalWrite(green2Pin, LOW);
    digitalWrite(yellow2Pin, LOW);
    digitalWrite(red2LedPin, HIGH);
    hienthi7SEG(num);                                                                     // Gọi hàm hiển thị số trên LED 7 đoạn
  }

  for (int num = 35; num >= 0; num--) 
  {
    digitalWrite(green1Pin, LOW);
    digitalWrite(yellow1Pin, LOW);
    digitalWrite(red1LedPin, HIGH);
    digitalWrite(green2Pin, HIGH);
    digitalWrite(yellow2Pin, LOW);
    digitalWrite(red2LedPin, LOW);
    hienthi7SEG(num);                                                                     // Gọi hàm hiển thị số trên LED 7 đoạn
  }

  for (int num = 3; num >= 0; num--) 
  {
    digitalWrite(green1Pin, LOW);
    digitalWrite(yellow1Pin, LOW);
    digitalWrite(red1LedPin, HIGH);
    digitalWrite(green2Pin, LOW);
    digitalWrite(yellow2Pin, HIGH);
    digitalWrite(red2LedPin, LOW);
    hienthi7SEG(num);                                                                     // Gọi hàm hiển thị số trên LED 7 đoạn
  }
}
