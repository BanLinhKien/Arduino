/*=========================================================================================
 *                                  BANLINHKIEN.COM                                       *
 *=========================================================================================
 *                           ĐIỀU KHIỂN BÓNG ĐÈN AC SỬ DỤNG IR                            *
 *=========================================================================================
*/
#include <IRremote.hpp>

const int Recv_Pin = 2;
int Ledpin = 3;
int i;
unsigned long lastTime = millis();        // Lưu thời gian cuối cùng khi nhận tín hiệu từ Remote

void setup() {
  
  Serial.begin(9600);
  pinMode(Ledpin, OUTPUT);
  digitalWrite(Ledpin, LOW); 
  //Khởi động bộ thu, Khi nhận tín hiệu Led chân 8 nhấp nháy
  IrReceiver.begin(Recv_Pin, true, 8);          // True cho phép led nối với chân 8 nhấp nháy
}

void loop() {
  if (IrReceiver.decode())                                                // Kiểm tra liên tục tín hiệu nhận từ Remote
  {
    
    uint32_t dataRemote = IrReceiver.decodedIRData.decodedRawData;        // Lưu dữ liệu nhận được vào biến dataRemote 
    
    if(dataRemote>0)                                                      // Kiểm tra dữ liệu hợp lệ phải >0
    {
      Serial.println(dataRemote);
      
     if (millis() - lastTime > 250)                                       // Do tắt mở cùng 1 nút bấm nên phải có thời gian nghĩ giữa 2 lần bấn
      {
        switch(dataRemote)
        {
            
          case 4010852096:                                                // Bấm phiếm số 1, điều khiển Led 1 sáng tắt
            digitalWrite(Ledpin, !digitalRead(Ledpin));
          break;
          
          case 3994140416:                                                // Bấm phiếm số 2, điều khiển Led 2 sáng tắt
            digitalWrite(Ledpin, LOW);
            delay(500);
            digitalWrite(Ledpin, HIGH);
            break;
        }
      }
    }
    lastTime = millis();                                                  //Reset biến lastTime
    IrReceiver.resume();                                                  // Cho phép nhận giá trị tiếp theo
  }
}
