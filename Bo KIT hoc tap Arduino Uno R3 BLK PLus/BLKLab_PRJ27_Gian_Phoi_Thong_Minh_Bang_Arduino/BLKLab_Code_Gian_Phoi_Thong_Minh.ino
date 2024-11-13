/********************************************************************************
 *                            BANLINHKIEN.COM                                   *
 ********************************************************************************
 *                Hướng dẫn Giàn phơi thông minh bằng Arduino                   *
 *******************************************************************************/
#include <Servo.h>

#define RAIN  2
#define SERVO 9
Servo myservo;  // tạo đối tượng servo để điều khiển servo

int pos = 0;    // tạo biến góc của servo

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  
  pinMode(2, INPUT_PULLUP);
  myservo.write(0);
}

void loop() {
  if (digitalRead(2) == LOW){
      Serial.println("Co MUA!!!");
      for (pos = 0; pos <= 180; pos += 1) { // góc quay từ 0 đến 180 độ
        myservo.write(pos);              
        delay(20);                      
      }
  }
  else{
      Serial.println("Khong MUA");
      for (pos = 180; pos >= 0; pos -= 1) { // góc quay từ 180 đến 0 độ
        myservo.write(pos);              
        delay(20);                       
      }
  }
}
