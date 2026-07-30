/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 *                  Hương Dẫn Làm Máy Tính Cầm Tay Bằng Arduino
 * Đấu nối: 
 + Keypad:
  -D5: nối với chân hàng 1 của keypad      -A3: nối với chân cột 1 của keypad
  -D4: nối với chân hàng 2 của keypad      -A2: nối với chân cột 2 của keypad 
  -D3: nối với chân hàng 3 của keypad      -A1: nối với chân cột 3 của keypad
  -D2: nối với chân hàng 4 của keypad      -A0: nối với chân cột 4 của keypad
 + LCD
  -D12: nối với chân RS của LCD
  -D11: nối với chân E của LCD      
  -D10: nối với chân D4 của LCD
  -D9: nối với chân D5 của LCD
  -D8: nối với chân D6 của LCD
  -D7: nối với chân D7 của LCD
  
 */

#include <LiquidCrystal.h>
#include <Keypad.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);                     // Khai báo và khởi tạo đối tượng LiquidCrystal để điều khiển màn hình LCD

/* Keypad setup */

const byte KEYPAD_ROWS = 4;                                 // Số hàng của bàn phím
const byte KEYPAD_COLS = 4;                                 // Số cột của bàn phím
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};                   // Các chân hàng kết nối với Arduino
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};               // Các chân cột kết nối với Arduino
char keys[KEYPAD_ROWS][KEYPAD_COLS] =                       // Ma trận chứa các ký tự tương ứng với các phím của bàn phím
{
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'.', '0', '=', '/'}
};

// Khởi tạo đối tượng "keypad" để xử lý việc nhận phím nhấn từ bàn phím

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

uint64_t value = 0;                                         // Biến "value" kiểu uint64_t để lưu trữ giá trị số nhập vào

void showSpalshScreen()                                     // Hàm hiển thị màn hình chào khi chương trình bắt đầu chạy
{
  lcd.setCursor(2, 0);
  lcd.print("Banlinhkien");
  lcd.setCursor(3, 1);
  String message = "Calculator";
  for (byte i = 0; i < message.length(); i++) 
  {
    lcd.print(message[i]);
    delay(500);
  }
  delay(500);
}

void updateCursor()                                           // Hàm cập nhật trạng thái con trỏ trên màn hình
{
  if (millis() / 250 % 2 == 0 ) 
  {
    lcd.cursor();
  } else 
  {
    lcd.noCursor();
  }
}

void setup() 
{
  Serial.begin(115200);
  lcd.begin(16, 2);

  showSpalshScreen();
  lcd.clear();
  lcd.cursor();

  lcd.setCursor(1, 0);
}

char operation = 0;
String memory = "";
String current = "";
uint64_t currentDecimal;
bool decimalPoint = false;

double calculate(char operation, double left, double right)   // Hàm sử dụng để thực hiện các phép tính cộng, trừ, nhân và chia
{
  switch (operation) 
  {
    case '+': return left + right;
    case '-': return left - right;
    case '*': return left * right;
    case '/': return left / right;
  }
}

void processInput(char key)                                   // Hàm xử lý việc nhập ký tự từ bàn phím
{
  if ('-' == key && current == "") 
  {
    current = "-";
    lcd.print("-");
    return;
  }

  switch (key) 
  {
    case '+':
    case '-':
    case '*':
    case '/':
      if (!operation) 
      {
        memory = current;
        current = "";
      }
      operation = key;
      lcd.setCursor(0, 1);
      lcd.print(key);
      lcd.setCursor(current.length() + 1, 1);
      return;

    case '=':
      float leftNum = memory.toDouble();
      float rightNum = current.toDouble();
      memory = String(calculate(operation, leftNum, rightNum));
      current = "";
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(memory);
      lcd.setCursor(0, 1);
      lcd.print(operation);
      return;
  }

  if ('.' == key && current.indexOf('.') >= 0) 
  {
    return;
  }

  if ('.' != key && current == "0") 
  {
    current = String(key);
  } else if (key) 
  {
    current += String(key);
  }

  lcd.print(key);
}

void loop() 
{
  updateCursor();                                             // Cập nhật trạng thái con trỏ

  char key = keypad.getKey();
  if (key) 
  {
    processInput(key);                                        // Xử lý việc nhấn phím và cập nhật màn hình LCD
  }
}
