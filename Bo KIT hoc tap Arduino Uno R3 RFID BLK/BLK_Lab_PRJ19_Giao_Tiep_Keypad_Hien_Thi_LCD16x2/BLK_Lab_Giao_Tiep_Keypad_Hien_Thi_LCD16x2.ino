/*                                                                                 *
 * ================================================================================*
 *                            BANLINHKIEN.COM                                      * 
 * ================================================================================*                           
 *                                                                                 *
 *             Hướng Dẫn Giao tiếp với Keypad hiển thị lên LCD1602                 *
 * Đấu nối:                                                                        *
 *-D8: nối chân D4 trên LCD      -D11: nối chân D7 trên LCD                        *
 *-D9: nối chân D5 trên LCD      -D13: nối chân E trên LCD                         *
 *-D10: nối chân D6 trên LCD     -D12: nối chân RS trên LCD                        *
 *                                                                                 *
 *-D2: nối chân số 1 trên Keypad      -A0: nối chân số 5 trên Keypad               *
 *-D3: nối chân số 2 trên Keypad      -A1: nối chân số 6 trên Keypad               *
 *-D4: nối chân số 3 trên Keypad      -A2: nối chân số 7 trên Keypad               *
 *-D5: nối chân số 4 trên Keypad      -A3: nối chân số 8 trên Keypad               *
 *=================================================================================*/

#include <LiquidCrystal.h>  // Thư viện hiển thị LCD 
#include <Keypad.h>         // Thư viện Keypad

// Khởi tạo đối tượng LCD: LiquidCrystal(RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 13, 8, 9, 10, 11);

// Cấu hình Keypad 4x4
const byte ROWS = 4;        // 4 hàng
const byte COLS = 4;        // 4 cột
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A3, A2, A1, A0};    // Kết nối với chân hàng
byte colPins[COLS] = {5, 4, 3, 2};        // Kết nối với chân cột

// Khởi tạo đối tượng Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int holdDelay = 700;  // Thời gian trễ nhấn nút nhằm tránh nhiễu
int n = 3; 
int state = 0;        // Nếu state = 0 không nhấn, state = 1 nhấn thời gian ngắn, state = 2 nhấn giữ lâu
char key = 0; 

void setup() {
  Serial.begin(9600);           // bật serial, baudrate 9600
  lcd.begin(16, 2);             // Khởi động LCD với 16 cột và 2 hàng
  lcd.print("Banlinhkiem.com");
  delay(2000);                  // Chờ 2 giây trước khi hiển thị tiếp
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press a key:");
}

void loop() {
  char temp = keypad.getKey();  // Đọc phím từ Keypad
  
  if ((int)keypad.getState() ==  PRESSED) {
    if (temp != 0) {
      key = temp;
    }
  }
  if ((int)keypad.getState() ==  HOLD) {
    state++;
    state = constrain(state, 1, n-1);
    delay(holdDelay);
  }
 
  if ((int)keypad.getState() ==  RELEASED) {
    key += state;
    state = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Key Pressed:");
    lcd.setCursor(0, 1);
    lcd.print(key);           // Hiển thị phím lên LCD
    Serial.println(key);
    delay(1000);              // Chờ 1 giây trước khi hiển thị tiếp
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press a key:");
  }
}
