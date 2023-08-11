/*
================================================================================================================= 
**                                           BANLINHKIEN.COM                                                  **     
=================================================================================================================
*                                ĐIỀU KHIỂN LED SÁNG TẮT BẰNG HỒNG NGOẠI
*Đấu nối: - Chân tín hiệu của hồng ngoại nối với chân D2 của Arduino                                
          - 9 LED đơn được nối lần lượt với các chân D3-D11 của Arduino
          - 1 LED nối với chân D12 
 * uint32_t: Là kiểu dữ liệu số 32 bit, giá trị không dấu từ 0 đến FFFFFFFF
 * IrReceiver.begin(_): khởi tạo bộ thu
 * IrReceiver.decode(): kiểm tra xem có tín hiệu IR đến hay không.
 * IrReceiver.resume(): cho phép nhận giá trị tiếp theo

*/
#include <IRremote.hpp>
const int Recv_Pin = 2;
int Ledpin[]={3,4,5,6,7,8,9,10,11};                       // Khai báo và khởi tạo LED với các chân
int i;
unsigned long lastTime = millis();                        // Lưu thời gian cuối cùng khi nhận tín hiệu từ Remote

void setup() {
  Serial.begin(9600);
  for(i=0; i<=9; i++)
     pinMode(Ledpin[i], OUTPUT);
  for(i=0; i<=9; i++)
    digitalWrite(Ledpin[i], LOW);
  //Khởi động bộ thu, Khi nhận tín hiệu Led chân 12 nhấp nháy
  IrReceiver.begin(Recv_Pin, true, 12);                   // True cho phép led nối với chân 8 nhấp nháy
}

void loop() {
  if (IrReceiver.decode())                                // Kiểm tra liên tục tín hiệu nhận từ Remote
  {
    
    uint32_t dataRemote = IrReceiver.decodedIRData.decodedRawData;      // Lưu dữ liệu nhận được vào biến dataRemote 
    
    if(dataRemote>0)                                     // Kiểm tra dữ liệu hợp lệ phải >0
    {
      Serial.println(dataRemote);
      
     if (millis() - lastTime > 250)                     // Do tắt mở cùng 1 nút bấm nên phải có thời gian nghĩ giữa 2 lần bấn
      {
        switch(dataRemote)
        {
            
          case 4010852096:                              // Bấm phím số 1, điều khiển Led 1 sáng tắt
            digitalWrite(Ledpin[0], !digitalRead(Ledpin[0]));
          break;
          
          case 3994140416:                              // Bấm phím số 2, điều khiển Led 2 sáng tắt
            digitalWrite(Ledpin[1], !digitalRead(Ledpin[1]));
            break;

          case 3977428736:                              // Bấm phím số 3, điều khiển Led 3 sáng tắt
            digitalWrite(Ledpin[2], !digitalRead(Ledpin[2]));
          break;
          
          case 3944005376:                              // Bấm phím số 4, điều khiển Led 4 sáng tắt
            digitalWrite(Ledpin[3], !digitalRead(Ledpin[3]));    
          break;
          
          case 4010852096:                              // Bấm phím số 5, điều khiển Led 5 sáng tắt
            digitalWrite(Ledpin[5], !digitalRead(Ledpin[5]));
          break;
          
          case 4010852096:                              // Bấm phím số 6, điều khiển Led 6 sáng tắt
            digitalWrite(Ledpin[6], !digitalRead(Ledpin[6]));
          break;
          
          case 4010852096:                              // Bấm phím số 7, điều khiển Led 7 sáng tắt
            digitalWrite(Ledpin[7], !digitalRead(Ledpin[7]));
          break;
          
          case 4010852096:                              // Bấm phím số 8, điều khiển Led 8 sáng tắt
            digitalWrite(Ledpin[8], !digitalRead(Ledpin[8]));
          break;
          
          case 4010852096:                              // Bấm phím số 9, điều khiển Led 9 sáng tắt
            digitalWrite(Ledpin[9], !digitalRead(Ledpin[9]));
          break;
          
          case 4261527296:                              // Bấm phím CH-, tất cả các đèn đều tắt
            for(i=0; i<=9; i++)
              digitalWrite(Ledpin[i], LOW); 
          break;
          
          case 4127833856:                             // Bấm phím số CH+, tất cả các đèn đều sáng
            for(i=0; i<=9; i++)
               digitalWrite(Ledpin[i], HIGH);
          break;
        }
      }
    }
    lastTime = millis();                              //Reset biến lastTime
    IrReceiver.resume();                              // Cho phép nhận giá trị tiếp theo
  }
}
