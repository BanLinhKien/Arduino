/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 * Code: Điều khiển LED bằng nút bấm sử dụng ngắt ngoài - How to control led by push button
 * Đấu nối: 
  -D2: Khai báo chân số 2 là chân kết nối nút bấm
  -D13: Khai báo chân số 13 là chân kết nối đèn LED
 */

const int buttonPin = 2; 
const int ledPin = 13;    

volatile bool buttonState = LOW;              // Trạng thái hiện tại của nút bấm
volatile bool lastButtonState = LOW;          // Trạng thái trước đó của nút bấm
volatile unsigned long lastDebounceTime = 0;  // Thời điểm chống nhiễu gần nhất
const unsigned long debounceDelay = 50;       // Thời gian chờ để tránh đội phím (chống nhiễu)

void setup() 
{
  pinMode(ledPin, OUTPUT);                    // Đặt chân đèn LED là chế độ OUTPUT
  pinMode(buttonPin, INPUT_PULLUP);           // Đặt chân nút bấm là chế độ INPUT_PULLUP (nút mắc nối Pull-Up)
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, CHANGE);  // Kết nối ngắt nút bấm với hàm xử lý ngắt
}

void loop() 
{
}

void buttonInterrupt() 
{
  unsigned long currentMillis = millis();                 // Lấy thời gian hiện tại
  if (currentMillis - lastDebounceTime >= debounceDelay)  // Kiểm tra nếu đủ thời gian chờ để tránh đội phím
  {
    lastDebounceTime = currentMillis;                     // Cập nhật thời điểm chống nhiễu gần nhất
    buttonState = digitalRead(buttonPin);                 // Đọc trạng thái của nút bấm

    if (buttonState != lastButtonState)                   // Kiểm tra nếu trạng thái nút thay đổi so với trạng thái trước đó
    {
      lastButtonState = buttonState;                      // Cập nhật trạng thái trước đó của nút
      if (buttonState == LOW)                             // Kiểm tra nếu nút được nhấn (trạng thái LOW khi nút được nhấn)
      {
        // Thực hiện thao tác bật/tắt đèn LED bằng cách đảo trạng thái hiện tại của LED
        digitalWrite(ledPin, !digitalRead(ledPin));
      }
    }
  }
}

  
  
/*
- Hàm buttonInterrupt() được gọi mỗi khi có sự thay đổi trạng thái của chân nút nhấn.
- Đầu tiên, nó lấy thời gian hiện tại bằng cách sử dụng hàm millis() và lưu vào biến currentMillis.
- Sau đó, nó kiểm tra xem thời gian kể từ lần nhấn trước đó đã đủ lớn hơn hoặc bằng debounceDelay hay chưa.
- Nếu thời gian đã đủ lớn (đã trôi qua thời gian chống chập chờn), nó cập nhật lastDebounceTime bằng currentMillis để đảm bảo đếm thời gian cho lần nhấn tiếp theo.
- Sau đó, nó đọc trạng thái hiện tại của nút nhấn bằng digitalRead() và lưu vào biến buttonState.
- Tiếp theo, nó kiểm tra xem trạng thái hiện tại buttonState có khác với trạng thái trước đó lastButtonState hay không.
- Nếu có sự thay đổi trạng thái của nút nhấn, nó cập nhật lastButtonState với buttonState.
- Nếu buttonState bằng LOW (nút được nhấn), thì nó thực hiện thao tác bật/tắt LED bằng cách sử dụng digitalWrite() với tham số đảo ngược !digitalRead(ledPin).
*/
