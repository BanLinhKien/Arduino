/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 *              
 *              Hướng Dẫn Hiển Thị Sóng Âm Thanh Lên LCD1602
 * Đấu nối: 
  -D3: nối chân D4 trên LCD      -D6: nối chân D7 trên LCD
  -D4: nối chân D5 trên LCD      -D9: nối chân E trên LCD
  -D5: nối chân D6 trên LCD      -D11: nối chân RS trên LCD
 */
#include <LiquidCrystal.h>
 //Khai báo các chân Arduino nối với Màn hình 16x2
LiquidCrystal lcd(11,9,6,5,4,3);        //RS, E, D4, D5, D6, D7
//Khởi tạo các biến
int i=0;
int j=0;
int randomnum;
int k=0;


//Khởi tạo các độ cao trong một ô 5x8
byte wave1[] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};       //Tất cả các hàng sáng

byte wave2[] = {0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};       // 2 hàng trên tắt

byte wave3[] = {0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F};       // 4 hàng trên tắt

byte wave4[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F};       // 6 hàng tắt

byte wave5[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};       // 8 hàng tắt

void setup() {                        //Hàm khởi tạo
  Serial.begin(9600);                 //Tạo serial truyền và nhận dữ liệu với tốc độ baud 9600 bit mỗi giây
  lcd.begin(16,2);                    //Khởi tạo màn hình LCD được sử dụng là 16x2
  lcd.createChar(4, wave1);           //Hiển thị wave1 ở vị trí thứ 4
  lcd.createChar(3, wave2);           //Hiển thị wave2 ở vị trí thứ 3
  lcd.createChar(2, wave3);           //Hiển thị wave3 ở vị trí thứ 2
  lcd.createChar(1, wave4);           //Hiển thị wave4 ở vị trí thứ 1
  lcd.createChar(0, wave5);           //Hiển thị wave5 ở vị trí thứ 0
  lcd.clear();                        //Xóa màn hình, đưa con trỏ về vị trí 0,0
  lcd.setCursor(1,0);                 //Đưa con trỏ về vị trí 1,0 (cột, hàng)
  lcd.print("Banlinhkien.com");       //Hiển thị dòng chữ Banlinhkien.com
  delay(2000);                        // Hiển thị trong 2s
  lcd.clear();                        //Xóa màn hinhg
}

void loop() {                             //Hàm lặp
  randomnum=random(0,8);                  // Gán biến randomnum bằng 1 số ngẫu nhiên từ 0-7
  Serial.print ("Giá trị randomnum= ");   // In dòng chữ 'Giá trị randomnum' ra cổng nối tiếp.
  Serial.println (randomnum);             // In giá trị của biến randomnum ra cổng nối tiếp.
  
  for (i=randomnum; i>0; i--)             // Trong vòng for, gán i=randomnum và cho giảm dần đến khi i>0
  {
    int z=i;                              //Gán z=i để lưu lại giá trị của i sau quá trình hiển thị
    Serial.print ("Giá trị i= ");         // In dòng chữ 'Giá trị i=' ra cổng nối tiếp.
    Serial.println (i);                   // In giá trị của biến i ra cổng nối tiếp.
    
    // Hiển thị ở cột thứ 7
    test();                               //Gọi hàm test để xác định j và k
    lcd.setCursor(7,0);                   // Đặt con trỏ ở cột thứ 7 hàng 0
    lcd.write(j);                         // In ra wave? ở vị trí j?
    lcd.setCursor(7,1);                   // Đặt con trỏ ở cột thứ 7 hàng 1
    lcd.write(k);                         // In ra wave? ở vị trí k?
    i--;                                  //Giảm i đi 1 đơn vị để có 1 bộ i và k mới
    
    // Hiển thị ở cột thứ 6 và 8    
    test();
    lcd.setCursor(8,0);
    lcd.write(j);
    lcd.setCursor(8,1);
    lcd.write(k);
    lcd.setCursor(6,0);
    lcd.write(j);
    lcd.setCursor(6,1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 5 và 9    
    test();
    lcd.setCursor(9,0);
    lcd.write(j);
    lcd.setCursor(9,1);
    lcd.write(k);
    lcd.setCursor(5,0);
    lcd.write(j);
    lcd.setCursor(5,1);
    lcd.write(k);
    i--;
    
    // Hiển thị ở cột thứ 4 và 10
    test();
    lcd.setCursor(10,0);
    lcd.write(j);
    lcd.setCursor(10,1);
    lcd.write(k);
    lcd.setCursor(4,0);
    lcd.write(j);
    lcd.setCursor(4,1);
    lcd.write(k);
    i--;
    
    // Hiển thị ở cột thứ 3 và 11
    test();
    lcd.setCursor(11,0);
    lcd.write(j);
    lcd.setCursor(11,1);
    lcd.write(k);
    lcd.setCursor(3,0);
    lcd.write(j);
    lcd.setCursor(3,1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 2 và 12
    test();
    lcd.setCursor(12,0);
    lcd.write(j);
    lcd.setCursor(12,1);
    lcd.write(k);
    lcd.setCursor(2,0);
    lcd.write(j);
    lcd.setCursor(2,1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 1 và 13
    test();
    lcd.setCursor(13,0);
    lcd.write(j);
    lcd.setCursor(13,1);
    lcd.write(k);
    lcd.setCursor(1,0);
    lcd.write(j);
    lcd.setCursor(1,1);
    lcd.write(k);
    i--;

    // Hiển thị ở cột thứ 0 và 14
    test();
    lcd.setCursor(14,0);
    lcd.write(j);
    lcd.setCursor(14,1);
    lcd.write(k);
    lcd.setCursor(0,0);
    lcd.write(j);
    lcd.setCursor(0,1);
    lcd.write(k);
      
    delay(100); // Hiển thị 0,1s
    lcd.clear();//Xóa màn hình
     
    i=z; //Đặt lại i=z
  }
}

void test()            //Hàm kiểm tra giá trị của i để đưa ra một cặp j và k khác nhau
{

  if (i ==8)
   {
    j= 4;
    k=4;
  } //Nếu i=8 ta có i=4, k=4
   if (i ==7)
  {
    j=3;
    k=4;
  } //Nếu i=7 ta có i=3, k=4
   if (i ==6)
  {
    j=2;
    k=4;
  } //Nếu i=6 ta có i=2, k=4
     if (i ==5)
  {
    j=1;
    k=4;
  } //Nếu i=5 ta có i=1, k=4
     if (i ==4)
  {
    j=0;
    k=4;
  } //Nếu i=4 ta có i=0, k=4
     if (i ==3)
  {
    j=0;
    k=3;
  } //Nếu i=3 ta có i=0, k=3
     if (i ==2)
  {
    j=0;
    k=2;
  } //Nếu i=2 ta có i=0, k=2
     if (i ==1)
  {
    j=0;
    k=1;
  } //Nếu i=1 ta có i=0, k=1
     if (i ==0)
  {
    j=0;
    k=0;
  } //Nếu i=0 ta có i=0, k=0
}
