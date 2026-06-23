/*==========================================================================================
 *                                  BANLINHKIEN.COM                                        *
 *==========================================================================================
                       HIỂN THỊ DỮ LIỆU TRÊN MÀN HÌNH LCD 16x2.
  ==========================================================================================
  Đấu nối:
  D3: Kết nối với chân DB7
  D4: Kết nối với chân DB6
  D5: Kết nối với chân DB5
  D6: Kết nối với chân DB4
  D9: Kết nối với chân EN
  D11: Kết nối với chân RS
  ========================================================================================== */

#include <LiquidCrystal.h>                  //Gọi thư viện của LCD
LiquidCrystal lcd(11,9,6,5,4,3);            //Khai báo địa chỉ nối Arduino và Màn hình 16x02

  //Code hiển thị dữ liệu ra màn hình

void setup()
{
  lcd.begin(16,2);                          //Khởi tạo màn hình LCD được sử dụng là 16x2
  lcd.setCursor(1,0);                       //Đưa con trỏ về cột 0 hàng 1
  lcd.print("Banlinhkien.vn");              //In ra màn hình dòng chữ 'Banlinhkien.com'
  lcd.setCursor(4,1);                       //Đưa con trỏ về cột 1 hàng 4
  lcd.print("Arduino");                     //In ra màn hình dòng chữ 'Arduino'
}
void loop()
{
  /*
  //Cuộn chữ sang trái
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
  lcd.scrollDisplayLeft();                  // Cuộn 1 vị trí sang trái
  delay(150);
  }
  delay(100);

*/
}
