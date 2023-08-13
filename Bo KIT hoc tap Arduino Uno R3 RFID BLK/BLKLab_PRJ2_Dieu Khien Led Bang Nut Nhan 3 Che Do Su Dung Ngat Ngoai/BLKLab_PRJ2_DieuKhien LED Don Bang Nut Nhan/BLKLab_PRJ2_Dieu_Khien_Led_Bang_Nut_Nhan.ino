/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 *            Hướng dẫn Điều khiển Led bằng nút nhấn sử dụng ngắt ngoài
 * Đấu nối: 
  -D2: nối với chân phím bấm      -D5: nối với chân led 3
  -D3: nối với chân led 1         -D6: nối với chân led 4
  -D4: nối với chân led 2         -D7: nối với chân led 5   
 */

const int ledPins[] = {3, 4, 5, 6, 7};                                         // Khai báo mảng chứa các chân kết nối của lED với Arduino
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

const int buttonPin = 2;                                                       // Khai báo chân kết nối của nút nhấn với Arduino
volatile int buttonState = HIGH;                                               // Trạng thái hiện tại của nút nhấn
volatile int lastButtonState = HIGH;                                           // Trạng thái trước đó của nút nhấn

int mode = 0;                                                                  // Chế độ hiệu ứng

void setup()
{
  for (int i = 0; i < numLeds; i++)
  {
    pinMode(ledPins[i], OUTPUT);                                               // Cài đặt chân là OUTPUT để điều khiển LED
    digitalWrite(ledPins[i], LOW);                                             // Tắt tất cả các LED ban đầu
  }

  pinMode(buttonPin, INPUT_PULLUP);                                            // Cài đặt chân nút nhấn là INPUT_PULLUP
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, CHANGE);  // Gắn ngắt cho nút nhấn
}

void loop()
{
}

void buttonInterrupt()
{
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && lastButtonState == HIGH)
  {
    mode = (mode + 1) % 3;                                                      // Chuyển đổi giữa 3 chế độ hiệu ứng
    activateEffect();
    delay(200);                                                                 // Tránh đọc tín hiệu của nút nhấn liên tục khi giữ nút
  }
  else if (buttonState == HIGH && lastButtonState == LOW)
  {
    turnOffLeds();                                                              // Tắt tất cả LED khi nhả nút
  }

  lastButtonState = buttonState;
}

void activateEffect()
{
  if (mode == 0) 
  {
    for (int i = 0; i < numLeds; i++) 
    {
      digitalWrite(ledPins[i], HIGH);                                           // Bật tất cả LED
    }
  } 
  else if (mode == 1) 
  {
    for (int i = 0; i < numLeds; i++) 
    {
      digitalWrite(ledPins[i], i % 2 == 0 ? HIGH : LOW);                        // Bật LED chẵn, tắt LED lẻ
    }
  } 
  else if (mode == 2) 
  {
    for (int i = 0; i < numLeds; i++) 
    {
      digitalWrite(ledPins[i], i % 2 == 0 ? LOW : HIGH);                        // Tắt LED chẵn, bật LED lẻ
    }
  }
}

void turnOffLeds()
{
  for (int i = 0; i < numLeds; i++) 
  {
    digitalWrite(ledPins[i], LOW);                                              // Tắt tất cả LED
  }
}
