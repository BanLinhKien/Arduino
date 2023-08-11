/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 * Code: Sử dụng phím bấm điều khiển thay đổi hiệu ứng led
 * Đấu nối: 
 * -D2: nối với chân phím bấm      -D5: nối với chân led 3
 * -D3: nối với chân led 1         -D6: nối với chân led 4
 * -D4: nối với chân led 2         -D7: nối với chân led 5   
 */

const int ledPins[] = {3, 4, 5, 6, 7};                                          // Mảng chứa các chân của LED kết nối với Arduino
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);                       // Số lượng LED, được tính từ kích thước của mảng ledPins

const int buttonPin = 2;                                                        // Chân nút nhấn kết nối với chân 2 trên Arduino
volatile int buttonState = HIGH;                                                // Trạng thái hiện tại của nút nhấn (ban đầu HIGH - không nhấn)
volatile int lastButtonState = HIGH;                                            // Trạng thái trước đó của nút nhấn

int mode = 0;                                                                   // Biến quản lý hiệu ứng hiện tại của LED

void setup()
{
  for (int i = 0; i < numLeds; i++)
  {
    pinMode(ledPins[i], OUTPUT);                                                 // Thiết lập chân của LED là OUTPUT
    digitalWrite(ledPins[i], LOW);                                               // Tắt tất cả các LED ban đầu
  }

  pinMode(buttonPin, INPUT_PULLUP);                                             // Thiết lập chân nút nhấn là INPUT_PULLUP
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, CHANGE);   // Kết nối hàm xử lý ngắt khi trạng thái nút nhấn thay đổi
}

void loop()
{
  // Hàm loop trống, tất cả công việc được thực hiện trong các hàm con và xử lý ngắt
}

void buttonInterrupt()
{
  buttonState = digitalRead(buttonPin);                                         // Đọc trạng thái nút nhấn

  if (buttonState == LOW && lastButtonState == HIGH)
  {
    mode = (mode + 1) % 3;                                                      // Thay đổi hiệu ứng LED khi nút nhấn được nhấn
    activateEffect();
    delay(200);                                                                 // Chờ 200ms để tránh nhấn liên tục
  }
  else if (buttonState == HIGH && lastButtonState == LOW)
  {
    turnOffLeds();                                                              // Tắt tất cả các LED khi nút nhấn được thả ra
  }

  lastButtonState = buttonState;                                                // Lưu trạng thái nút nhấn để so sánh lần sau
}

void activateEffect()
{
  // Hàm này kích hoạt hiệu ứng LED dựa trên giá trị của biến "mode"

  if (mode == 0) 
  {
    for (int i = 0; i < numLeds; i++) 
    {
      digitalWrite(ledPins[i], HIGH);                                           // Bật tất cả các LED
    }
  } 
  else if (mode == 1) 
  {
    for (int i = 0; i < numLeds; i++) 
    {
      digitalWrite(ledPins[i], i % 2 == 0 ? HIGH : LOW);                        // Bật lần lượt các LED lẻ
    }
  } 
  else if (mode == 2) 
  {
    for (int i = 0; i < numLeds; i++) 
    {
      digitalWrite(ledPins[i], i % 2 == 0 ? LOW : HIGH);                        // Bật lần lượt các LED chẵn
    }
  }
}

void turnOffLeds()
{
  for (int i = 0; i < numLeds; i++) 
  {
    digitalWrite(ledPins[i], LOW);                                              // Tắt tất cả các LED
  }
}
