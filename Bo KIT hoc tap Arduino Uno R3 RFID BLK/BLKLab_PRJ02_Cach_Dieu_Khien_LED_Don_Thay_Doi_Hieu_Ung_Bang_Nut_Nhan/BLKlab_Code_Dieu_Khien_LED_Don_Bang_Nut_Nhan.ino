/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 *      ĐIỀU KHIỂN LED BẰNG NÚT NHẤN (Hướng dẫn xác định LED nào đang sáng)
 *      
 * Đấu nối: 
  -D2: nối với chân nút nhấn      -D5: nối với chân led 3
  -D3: nối với chân led 1         -D6: nối với chân led 4
  -D4: nối với chân led 2         -D7: nối với chân led 5   
 */

const int ledPins[] = {3, 4, 5, 6, 7};                      // Khai báo mảng chứa các chân kết nối của lED với Arduino
const int buttonPin = 2;                                    // Khai báo chân kết nối của nút nhấn với Arduino
int buttonState = 0;                                        // Trạng thái hiện tại của nút nhấn
int dem=0;

void setup()
{
  Serial.begin(9600);                                       // Mở cổng Serial để Hiển thị đèn LED sáng hiện tại
  for (int i = 0; i < 5; i++)
  {
    pinMode(ledPins[i], OUTPUT);                            // Cài đặt chân là OUTPUT để điều khiển LED
    digitalWrite(ledPins[i], LOW);                          // Tắt tất cả các LED ban đầu
  }
  pinMode(buttonPin, INPUT_PULLUP);                         // Cài đặt chân nút nhấn là INPUT_PULLUP
}

void loop()
{
  buttonState = digitalRead(buttonPin);                    // Đọc trạng thái nút nhấn
  if (buttonState == LOW)                                  // Nếu nút nhấn được nhấn xuống
  {
    for (int i = 0; i < 5; i++)                            // Dùng for để bật lần lượt các LED
    {
      digitalWrite(ledPins[i], HIGH);                      // Hiển thị LED với các giá trị i tương ứng
      dem=i+1;                                             // Biến 'dem' là số thứ tự đèn LED đang sáng hiện tại
      Serial.print("Den LED đang sáng la den so: ");       // Hiển thị trên cổng Serial đèn LED hiện tại đang sáng
      Serial.println(dem);                                 // Sau 1 giây sẽ bật 1 LED, LED được bật cuối cùng là LED số 5
      delay(1000); 
    }
  }
  else                                                    // Ngược lại nếu nút nhấn được nhả ra
  {
    for (int i = 0; i < 5; i++)                           // Dùng for để tắt các LED
    {
      digitalWrite(ledPins[i], LOW);                      // LED được tắt cuối cùng là LED số 5                    
      delay(1000);
    } 
  }
  
}
