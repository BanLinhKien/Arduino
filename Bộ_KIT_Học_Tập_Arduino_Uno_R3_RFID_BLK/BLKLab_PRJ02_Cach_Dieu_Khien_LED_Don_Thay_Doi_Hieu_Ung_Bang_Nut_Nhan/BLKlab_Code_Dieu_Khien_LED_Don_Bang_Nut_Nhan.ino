 /* ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 *      ĐIỀU KHIỂN LED BẰNG NÚT Nhấn (Hướng dẫn xác định LED nào đang sáng)
 * * Đấu nối: 
  -D2: nối với chân nút nhấn      -D5: nối với chân led 3
  -D3: nối với chân led 1         -D6: nối với chân led 4
  -D4: nối với chân led 2         -D7: nối với chân led 5   
 */

const int ledPins[] = {3, 4, 5, 6, 7};                      // Khai báo mảng chứa các chân kết nối của LED với Arduino
const int buttonPin = 2;                                    // Khai báo chân kết nối của nút nhấn với Arduino

int lastButtonState = HIGH;                                 // Trạng thái nút nhấn trước đó (mặc định chưa nhấn là HIGH)
int currentButtonState;                                     // Trạng thái hiện tại của nút nhấn

void setup()
{
  Serial.begin(9600);                                       // Mở cổng Serial để hiển thị trạng thái
  for (int i = 0; i < 5; i++)
  {
    pinMode(ledPins[i], OUTPUT);                            // Cài đặt chân là OUTPUT để điều khiển LED
    digitalWrite(ledPins[i], LOW);                          // Tắt tất cả các LED ban đầu
  }
  pinMode(buttonPin, INPUT_PULLUP);                         // Cài đặt chân nút nhấn là INPUT_PULLUP
}

void loop()
{
  currentButtonState = digitalRead(buttonPin);              // Đọc trạng thái hiện tại của nút nhấn

  // Kiểm tra xem nút có vừa được nhấn xuống không (chuyển từ TRẠNG THÁI CAO xuống THẤP)
  if (lastButtonState == HIGH && currentButtonState == LOW) 
  {
    delay(50);                                              // Chờ 50ms để chống dội phím (debounce)
    
    if (digitalRead(buttonPin) == LOW)                      // Nếu sau khi chờ mà nút vẫn đang nhấn
    {
      // --- GIAI ĐOẠN 1: BẬT ĐÈN ---
      for (int i = 0; i < 5; i++)                           // Dùng for để bật lần lượt các LED
      {
        digitalWrite(ledPins[i], HIGH);                     // Bật sáng đèn LED thứ i
        Serial.print("Den LED dang sang la den so: ");      // Hiển thị thông báo trên Serial
        Serial.println(i + 1);                              // In ra số thứ tự đèn đang sáng
        delay(500);                                         // Chờ 0.5 giây rồi bật đèn tiếp theo
      }

      // --- GIAI ĐOẠN 2: TẮT ĐÈN ---
      for (int i = 0; i < 5; i++)                           // Dùng for để tắt lần lượt các LED sau khi đã sáng hết
      {
        digitalWrite(ledPins[i], LOW);                      // Tắt đèn LED thứ i
        Serial.print("Den LED dang tat la den so: ");       // Hiển thị thông báo trên Serial
        Serial.println(i + 1);                              // In ra số thứ tự đèn đang tắt
        delay(500);                                         // Chờ 0.5 giây rồi tắt đèn tiếp theo
      }
    }
  }

  lastButtonState = currentButtonState;                     // Lưu lại trạng thái nút nhấn cho vòng lặp tiếp theo
}