/*==========================================================================================
 *                                  BANLINHKIEN.COM                                        *
 *==========================================================================================
                       HIỂN THỊ DỮ LIỆU TRÊN MÀN HÌNH LCD 16x2.
  ==========================================================================================
  lcd.begin(columns, rows): Khởi tạo màn hình LCD với số cột và số hàng xác định.
  lcd.print(data): In dữ liệu lên màn hình LCD.
  lcd.clear() : Xóa màn hình, đưa con trỏ về vị trí 0,0
  lcd.home()  : Đưa con trỏ về vị trí 0
  lcd.noDisplay() : Không hiển thị gì trên màn hình cả
  lcd.display() : Hiển thị lên màn hình
  lcd.noBlink() : Không nhấp nháy con trỏ
  lcd.blink() : Nhấp nháy con trỏ
  lcd.print(" "): Hiển thị văn bản lên màn hình
  lcd.setCursor(x,y)  : Đưa con trỏ về vị trí cột thứ x, hàng thứ y
  lcd.init()  : Khởi tạo Màn hình Màn hình
  lcd.scrollDisplayLeft() : Cuộn màn hình bên trái
  lcd.scrollDisplayRight()  : Cuộn màn hình bên phải
  lcd.leftToRight() : Kí tự sẽ hiển thị từ phía bên trái sang phải
  lcd.rightToLeft() : Kí tự sẽ hiển thị từ phía phải sang trái
  lcd.write(x)  : Màn hình sẽ in ra kí tự thứ x trong ô nhớ từ 0-7 của CGRAM
  lcd.createChar(location,charmap[])  : Hiển thị một kí tự charmap[] bất kì
  lcd.noBacklight() : Tắt đèn nền
  lcd.backlight() : Có đèn nền
  https://maxpromer.github.io/LCD-Character-Creator/ trang web tạo ký tự đặc biệt
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
