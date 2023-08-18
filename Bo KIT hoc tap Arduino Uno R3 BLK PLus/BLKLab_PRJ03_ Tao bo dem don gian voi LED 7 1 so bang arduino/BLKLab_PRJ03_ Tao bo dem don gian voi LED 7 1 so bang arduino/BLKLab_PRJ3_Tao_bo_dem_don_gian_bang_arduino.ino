/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 *              Hướng Dẫn Cách Hiển Thị Số Đếm 0-9 Trên LED 7 Thanhn 1 Số
 * Đấu nối: 
  -D6: nối chân A      -D10: nối chân D
  -D7: nối chân B      -D11: nối chân E
  -D8: nối chân DP     -D12: nối chân G
  -D9: nối chân C      -D13: nối chân F
 */
 //Khai báo cho các chân
 
const int APin=6;                //Khai báo và khởi tạo APin với chân số 6
const int BPin=7;                //Khai báo và khởi tạo BPin với chân số 7
const int DPPin=8;               //Khai báo và khởi tạo DPPin với chân số 8
const int CPin=9;                //Khai báo và khởi tạo CPin với chân số 9
const int DPin=10;               //Khai báo và khởi tạo DPin với chân số 10
const int EPin=11;               //Khai báo và khởi tạo EPin với chân số 11
const int GPin=12;               //Khai báo và khởi tạo GPin với chân số 12
const int FPin=13;               //Khai báo và khởi tạo FPin với chân số 13
int settime=1000;                //Khai báo biến settime được gián giá trị bằng 1000

void setup() {
  pinMode(APin,OUTPUT);          //Đặt chân 6 làm đầu ra
  pinMode(BPin,OUTPUT);          //Đặt chân 7 làm đầu ra
  pinMode(CPin,OUTPUT);          //Đặt chân 9 làm đầu ra
  pinMode(DPin,OUTPUT);          //Đặt chân 10 làm đầu ra
  pinMode(EPin,OUTPUT);          //Đặt chân 11 làm đầu ra
  pinMode(GPin,OUTPUT);          //Đặt chân 12 làm đầu ra
  pinMode(FPin,OUTPUT);          //Đặt chân 13 làm đầu ra
  pinMode(DPPin,OUTPUT);         //Đặt chân 8 làm đầu ra
}

void loop() {
  display0(); delay(settime);      //Gọi hàm hiển thị số 0 và hiển thị trong khoảng thời gian settime
  display1(); delay(settime);     //Gọi hàm hiển thị số 1 và hiển thị trong khoảng thời gian settime
  display2(); delay(settime);     //Gọi hàm hiển thị số 2 và hiển thị trong khoảng thời gian settime
  display3(); delay(settime);     //Gọi hàm hiển thị số 3 và hiển thị trong khoảng thời gian settime
  display4(); delay(settime);     //Gọi hàm hiển thị số 4 và hiển thị trong khoảng thời gian settime
  display5(); delay(settime);     //Gọi hàm hiển thị số 5 và hiển thị trong khoảng thời gian settime
  display6(); delay(settime);     //Gọi hàm hiển thị số 6 và hiển thị trong khoảng thời gian settime
  display7(); delay(settime);     //Gọi hàm hiển thị số 7 và hiển thị trong khoảng thời gian settime
  display8(); delay(settime);     //Gọi hàm hiển thị số 8 và hiển thị trong khoảng thời gian settime
  display9(); delay(settime);     //Gọi hàm hiển thị số 9 và hiển thị trong khoảng thời gian settime
  allOn(); delay(settime);        //Gọi hàm không hiển thị số trong khoảng thời gian settime
}
//Hiển thị tất cả led

void allOn()
{
  digitalWrite(APin,HIGH);
  digitalWrite(BPin,HIGH);
  digitalWrite(CPin,HIGH);
  digitalWrite(DPin,HIGH);
  digitalWrite(EPin,HIGH);
  digitalWrite(FPin,HIGH);
  digitalWrite(GPin,HIGH);
}
void allOff()
{
  digitalWrite(APin,LOW);
  digitalWrite(BPin,LOW);
  digitalWrite(CPin,LOW);
  digitalWrite(DPin,LOW);
  digitalWrite(EPin,LOW);
  digitalWrite(FPin,LOW);
  digitalWrite(GPin,LOW);
}

//Hiển thị số 0

void display0()
{
  allOff();
  digitalWrite(GPin,HIGH);
}
//Hiển thị số 1

void display1()
{
  allOff();
  digitalWrite(APin,HIGH);
  digitalWrite(FPin,HIGH);
  digitalWrite(GPin,HIGH);
  digitalWrite(DPin,HIGH);
  digitalWrite(EPin,HIGH);
}
//Hiển thị số 2

void display2()
{
  allOff();
  digitalWrite(FPin,HIGH);
  digitalWrite(CPin,HIGH);
}

//Hiển thị số 3

void display3()
{
  allOff();
  digitalWrite(FPin,HIGH);
  digitalWrite(EPin,HIGH);
}

//Hiển thị số 4

void display4()
{
  allOff();
  digitalWrite(APin,HIGH);
  digitalWrite(EPin,HIGH);
  digitalWrite(DPin,HIGH);
}

//Hiển thị số 5

void display5()
{
  allOff();
  digitalWrite(BPin,HIGH);
  digitalWrite(EPin,HIGH);
}

//Hiển thị số 6

void display6()
{
  allOff();
  digitalWrite(BPin,HIGH);
}

//Hiển thị số 7

void display7()
{
  allOff();
  digitalWrite(FPin,HIGH);
  digitalWrite(GPin,HIGH);
  digitalWrite(EPin,HIGH);
  digitalWrite(DPin,HIGH);
}

//Hiển thị số 8

void display8()
{
  allOff();
}

//Hiển thị số 9

void display9()
{
  allOff();
  digitalWrite(EPin,HIGH);
}

//Hiển thị dấu chấm

void displayDP()
{
  allOff();
  digitalWrite(DPPin,HIGH);
}
