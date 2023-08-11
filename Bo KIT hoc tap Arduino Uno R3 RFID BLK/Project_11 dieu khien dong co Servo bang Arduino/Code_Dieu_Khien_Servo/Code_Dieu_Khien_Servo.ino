/*=================================================================================================
 *                                     BANLINHKIEN.COM                                            *
 ==================================================================================================
                           ĐIỀU KHIỂN ĐỘNG CƠ SERVO BẰNG BIẾN TRỞ
 ==================================================================================================
 Đấu nối:
*  Servo        Arduino
    Đỏ            5V
   Nâu            GND
   Cam            A0
* Chân tín hiệu của biến trở nối với chân D6 của Arduino
*
- Servo myServo: Tạo biến myServo của loại Servo
- myServo.attached(): Trả về true nếu servo được gắn vào pin
- myServo.detach(): Tách myServo bằng một pin đính kèm
- myServo.write(angle): Ghi một góc từ 0 đến 180 cho servo gắn với myServo
- myServo.read(): Trả về một số nguyên chứa góc hiện tại của servo trong khoảng từ 0 - 180
- myServo.attach(PIN_NUMBER): myServo được liên kết với pin tại vị trí PIN_NUMBER

===================================================================================================
*/

#include <Servo.h>                    // Thêm thư viện Servo.h vào chương trình.

Servo myservo;                        // Tạo biến myServo của Động cơ Servo   
int servo = 6;                        // Khởi tạo và gán tín hiệu điều khiển Servo chân 6 trêm Arduino
int bientro = A0;                     // Khởi tạo và gán chân tín hiệu biến trở là A0 trên Arduino
int gtbientro;                        // Lưu giá trị đọc từ biến trở
int vtservo;                          // Lưu giá trị đọc từ biến trở chuyển đổi sang góc 0-180 độ

void setup() 
{
  myservo.attach(servo);              //myServo được liên kết với pin tại vị trí servo = 6(Chân 6))
  Serial.begin(9600);
}

void loop() 
{
  gtbientro = analogRead (bientro);   //Đọc giá trị của biến trở
  vtservo = map (gtbientro,0,1023,0,180);
  myservo.write(vtservo);
  Serial.print("Goc Servo: "); 
  Serial.println(vtservo);
  delay(300);

}
