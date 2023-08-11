 /*=================================================================================
 *                          BANLINHKIEN.COM                                        *
 *==================================================================================
                   CODE THIẾT KẾ CÁC KÝ TỰ ĐẶC BIỆT
  ==================================================================================
  Đấu nối: 
  -D3: nối chân D4 trên LCD      -D6: nối chân D7 trên LCD
  -D4: nối chân D5 trên LCD      -D9: nối chân E trên LCD
  -D5: nối chân D6 trên LCD      -D11: nối chân RS trên LCD
*/
 
#include <LiquidCrystal.h>                  //Khai báo thư viện của LCD
LiquidCrystal lcd(11,9,6,5,4,3);            //Khai báo địa chỉ nối Arduino và Màn hình 16x02
//Khai báo mảng ký tự 'tim'
byte Tim[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};
//Khai báo mảng ký tự 'Phần trăm'
byte Phantram[] = {
  B00000,
  B11000,
  B11001,
  B00010,
  B00100,
  B01000,
  B10011,
  B00011
};
//Khai báo mảng ký tự 'dấu thăng'
byte dauthang[] = {
  B00000,
  B01010,
  B01010,
  B11111,
  B01010,
  B11111,
  B01010,
  B01010
};
//Khai báo mảng ký tự 'và'
byte va[] = {
  B00000,
  B01100,
  B10010,
  B10100,
  B01000,
  B10101,
  B10010,
  B01101
};
//Khai báo mảng ký tự 'đô la'
byte dola[] = {
  B00000,
  B00100,
  B01111,
  B10100,
  B01110,
  B00101,
  B11110,
  B00100
};
//Khai báo mảng ký tự 'loa'
byte loa[] = {
  B00001,
  B00011,
  B00111,
  B11111,
  B11111,
  B00111,
  B00011,
  B00001
};
//Khai báo mảng ký tự 'nhiệt độ'
byte nhietdo[] = {
  B11000,
  B11000,
  B00111,
  B01000,
  B01000,
  B01000,
  B00111,
  B00000
};


void setup() 
{
  lcd.begin(16,2);                      //Khai báo LCD được sử dụng là 16x2
  // Tạo các ký tự đặc biệt.
  lcd.setCursor(0,1);                   //Đưa con trỏ về cột 0 hàng 1
  lcd.print("Banlinhkien.com");         //In ra màn hình dòng chữ 'Banlinhkien.com'
  lcd.createChar(0, Tim);               //Hiển thị kí tự tim
  lcd.createChar(1, Phantram);          //Hiển thị kí tự phần trăm
  lcd.createChar(2, dauthang);          //Hiển thị kí tự dấu thăng
  lcd.createChar(3, va);                //Hiển thị kí tự và
  lcd.createChar(4, dola);              //Hiển thị kí tự đô la
  lcd.createChar(5, loa);               //Hiển thị kí tự loa
  lcd.createChar(6, nhietdo);           //Hiển thị kí tự độ C

}

void loop() 
// In các ký tự ra màn hình LCD
{ 
  lcd.setCursor(0,0);                   //Đưa con trỏ về vị trí 0,0 (cột, hàng)
  lcd.write(byte(0));                   //In kí tự tim ra màn hình

  lcd.setCursor(2,0);                   //Đưa con trỏ về vị trí 2,0 (cột, hàng)
  lcd.write(byte(1));                   //In kí tự phần trăm ra màn hình

  lcd.setCursor(4,0);                   //Đưa con trỏ về vị trí 4,0 (cột, hàng)
  lcd.write(byte(2));                    //In kí tự dấu thăng ra màn hình

  lcd.setCursor(6,0);                   //Đưa con trỏ về vị trí 6,0 (cột, hàng)
  lcd.write(byte(3));                    //In kí tự và ra màn hình

  lcd.setCursor(8,0);                   //Đưa con trỏ về vị trí 8,0 (cột, hàng)
  lcd.write(byte(4));                    //In kí tự đô la ra màn hình

  lcd.setCursor(10,0);                   //Đưa con trỏ về vị trí 10,0 (cột, hàng)
  lcd.write(byte(5));                    //In kí tự loa ra màn hình

  lcd.setCursor(12,0);                   //Đưa con trỏ về vị trí 12,0 (cột, hàng)
  lcd.write(byte(6));                    //In kí tự nhiệt độ ra màn hình
}
*/
