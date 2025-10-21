
/* =================================================================================================*
 *                                          BANLINHKIEN.COM                                         *
 *==================================================================================================*                        
 *                 	   PRJ08: DIY đồng hồ đếm ngược trong cờ vua
 * 
 * Đấu nối: 
 * Arduino UNO R3 vs LCD 16x2 I2C
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 *
 * Arduino UNO R3 vs Nút nhấn
 * Nút nhấn 1: D2 <-> Pin 1  ;   GND <-> Pin 2 
 * Nút nhấn 2: D3 <-> Pin 1  ;   GND <-> Pin 2
 *
 * Arduino UNO R3 vs LED 5mm
 * LED 1: D12 <-> Anode  ;   GND <-> Cathode 
 * LED 2: D13 <-> Anode  ;   GND <-> Cathode 
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Địa chỉ LCD có thể là 0x27 hoặc 0x3F

const int blackButtonPin = 2;        // Nút nhấn cho Black (một đầu nối GND)
const int whiteButtonPin = 3;        // Nút nhấn cho White (một đầu nối GND)
const int blackLedPin = 12;          // LED báo lượt Black
const int whiteLedPin = 13;          // LED báo lượt White

int blackButtonState = HIGH;
int lastBlackButtonState = HIGH;
unsigned long lastBlackDebounceTime = 0;

int whiteButtonState = HIGH;
int lastWhiteButtonState = HIGH;
unsigned long lastWhiteDebounceTime = 0;

unsigned long debounceDelay = 50;

// Thời gian (10 phút)
double whiteTime = 600000;
double blackTime = 600000;
double whiteLastCheck = 0;
double blackLastCheck = 0;

int whiteScore = 0;  		// Tỉ số
int blackScore = 0;

bool isWhiteTurn = true;
bool gameOver = false;
String whoLost = "";

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(blackButtonPin, INPUT_PULLUP);
  pinMode(whiteButtonPin, INPUT_PULLUP);
  pinMode(blackLedPin, OUTPUT);
  pinMode(whiteLedPin, OUTPUT);

  digitalWrite(whiteLedPin, HIGH);
  digitalWrite(blackLedPin, LOW);

  whiteLastCheck = millis();
  blackLastCheck = millis();

  showHeader();       		// In dòng tiêu đề ban đầu
  updateDisplay();    		// In thời gian ban đầu
}

void loop() {
  // Xử lý nút nhấn Black
  int blackReading = digitalRead(blackButtonPin);
  if (blackReading != lastBlackButtonState) {
    lastBlackDebounceTime = millis();
  }
  if ((millis() - lastBlackDebounceTime) > debounceDelay) {
    if (blackReading != blackButtonState) {
      blackButtonState = blackReading;
      if (blackButtonState == LOW) {
        if (gameOver) {
          resetGame();
        } else if (!isWhiteTurn && !gameOver) {
          // Chuyển lượt sang White
          isWhiteTurn = true;
          whiteLastCheck = millis();
          digitalWrite(blackLedPin, LOW);
          digitalWrite(whiteLedPin, HIGH);
        }
      }
    }
  }
  lastBlackButtonState = blackReading;

  // Xử lý nút nhấn White
  int whiteReading = digitalRead(whiteButtonPin);
  if (whiteReading != lastWhiteButtonState) {
    lastWhiteDebounceTime = millis();
  }
  if ((millis() - lastWhiteDebounceTime) > debounceDelay) {
    if (whiteReading != whiteButtonState) {
      whiteButtonState = whiteReading;
      if (whiteButtonState == LOW) {
        if (gameOver) {
          resetGame();
        } else if (isWhiteTurn && !gameOver) {
          // Chuyển lượt sang Black
          isWhiteTurn = false;
          blackLastCheck = millis();
          digitalWrite(whiteLedPin, LOW);
          digitalWrite(blackLedPin, HIGH);
        }
      }
    }
  }
  lastWhiteButtonState = whiteReading;

  // Cập nhật thời gian
  if (!gameOver) {
    if (isWhiteTurn) {
      UpdateWhiteTime();
    } else {
      UpdateBlackTime();
    }
  }
}

// ===== HIỂN THỊ DÒNG 1: "Black        White" =====
void showHeader() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Black");
  lcd.setCursor(10, 0);  	// Dịch White sang trái một cột
  lcd.print("White");
}

// ===== CẬP NHẬT DÒNG 2: thời gian + tỉ số =====
void updateDisplay() {
  lcd.setCursor(0, 1);
  printTime(blackTime);

  lcd.setCursor(6, 1);  	// Dịch tỉ số sang trái một cột
  lcd.print(blackScore);
  lcd.print("-");
  lcd.print(whiteScore);

  lcd.setCursor(10, 1);  	// Dịch thời gian White sang trái một cột
  printTime(whiteTime);
}

// ===== HÀM CẬP NHẬT THỜI GIAN TRẮNG =====
void UpdateWhiteTime() {
  whiteTime -= (millis() - whiteLastCheck);
  whiteLastCheck = millis();

  if (whiteTime <= 0 && !gameOver) {
    gameOver = true;
    whoLost = "White";
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("White LOSE!");
    return;
  }

  updateDisplay();
}

// ===== HÀM CẬP NHẬT THỜI GIAN ĐEN =====
void UpdateBlackTime() {
  blackTime -= (millis() - blackLastCheck);
  blackLastCheck = millis();

  if (blackTime <= 0 && !gameOver) {
    gameOver = true;
    whoLost = "Black";
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Black LOSE!");
    return;
  }

  updateDisplay();
}

// ===== IN ĐỊNH DẠNG mm:ss =====
void printTime(double timeLeft) {
  int minutes = floor(timeLeft / 60000);
  int seconds = floor(timeLeft / 1000) - minutes * 60;

  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);
}

// ===== RESET TRÒ CHƠI =====
void resetGame() {
  if (whoLost == "White") {
    blackScore++;
  } else if (whoLost == "Black") {
    whiteScore++;
  }

  whiteTime = 600000;
  blackTime = 600000;
  whiteLastCheck = millis();
  blackLastCheck = millis();
  isWhiteTurn = true;
  digitalWrite(whiteLedPin, HIGH);
  digitalWrite(blackLedPin, LOW);
  gameOver = false;
  whoLost = "";
  lcd.clear();
  showHeader();
  updateDisplay();
}
