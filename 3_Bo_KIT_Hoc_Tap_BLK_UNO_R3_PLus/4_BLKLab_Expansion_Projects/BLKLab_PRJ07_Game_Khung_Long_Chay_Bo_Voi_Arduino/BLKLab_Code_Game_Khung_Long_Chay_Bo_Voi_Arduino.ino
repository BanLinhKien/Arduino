#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

// Khởi tạo LCD với địa chỉ I2C (thường là 0x27 hoặc 0x3F), kích thước 16 cột, 2 dòng
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Khai báo sơ đồ chân kết nối các linh kiện ngoại vi
const int BUTTON_PIN = 6; // Chân đọc tín hiệu nút nhấn (Sử dụng Pull-up nội bộ)
const int BUZZER_PIN = 7; // Chân phát xung điều khiển còi chíp (Passive Buzzer)

// Các hằng số thiết lập thông số cho trò chơi
const int DINO_COLUMN = 1;             // Cột cố định của nhân vật Khủng long trên LCD
const unsigned long BASE_SPEED = 250;  // Thời gian chờ mặc định giữa các bước dịch vật cản (ms)

// ====================================================================
// ĐỊNH NGHĨA HÌNH ẢNH TỰ CHẾ (CUSTOM CHARACTER)
// ====================================================================
// Thiết lập ma trận 5x8 pixel để tạo hình cho nhân vật trong bộ nhớ CGRAM.
// Mã nhị phân Bxxxxxxx với bit 1 là điểm ảnh sáng, bit 0 là điểm ảnh tối.

// Tạo hình nhân vật Khủng long Dino (Đăng ký vào ô nhớ số 0)
byte dinoChar[8] = {
  B00111,
  B00101,
  B00111,
  B10110,
  B11111,
  B01110,
  B01010,
  B00000
};

// Tạo hình Cây xương rồng làm vật cản (Đăng ký vào ô nhớ số 1)
byte cactusChar[8] = {
  B00100,
  B00101,
  B10101,
  B10101,
  B10111,
  B01100,
  B00100,
  B00100
};

// ====================================================================
// QUẢN LÝ TRẠNG THÁI (STATE MACHINE) & BIẾN TOÀN CỤC
// ====================================================================

// Định nghĩa các trạng thái hoạt động chính của vòng lặp Game
enum GameState { START, PLAYING, GAME_OVER };
GameState currentState = START;

// Nhóm biến quản lý hành động Nhảy bất đồng bộ (Không gây nghẽn mạch)
bool isJumping = false;
unsigned long jumpStartTime = 0;
const unsigned long JUMP_DURATION = 650; // Tổng thời gian nhân vật ở trên không (ms)

// Nhóm biến điều khiển vận hành trò chơi
int cactusPosition = 15;        // Vị trí cột hiện tại của cây xương rồng (từ 15 về 0)
int lastCactusPos = 15;         // Lưu vị trí cột ở chu kỳ trước để thực hiện xóa bóng ma
unsigned long lastMoveTime = 0; // Mốc thời gian cập nhật vị trí xương rồng lần cuối
unsigned long currentSpeed = BASE_SPEED; // Tốc độ di chuyển hiện tại (càng nhỏ càng nhanh)
int score = 0;                  // Biến lưu trữ điểm số người chơi đạt được

// ====================================================================
// CÁC HÀM CHỨC NĂNG CƠ SỞ (DRIVER FUNCTIONS)
// ====================================================================

// Hàm phát âm thanh qua còi chíp với tần số và thời lượng xác định
void playTone(int frequency, int duration) {
  tone(BUZZER_PIN, frequency, duration);
}

void setup() {
  // Cấu hình chân nút nhấn là INPUT_PULLUP để không cần nối thêm điện trở bên ngoài
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Khởi tạo màn hình LCD, bật đèn nền (Hàm chuẩn của LiquidCrystal_I2C)
  lcd.init();
  lcd.backlight();
  
  // Tải dữ liệu hình ảnh tự chế vào bộ nhớ đệm của LCD
  lcd.createChar(0, dinoChar);
  lcd.createChar(1, cactusChar);
  
  // Hiển thị giao diện chào mừng ban đầu
  showStartScreen();
}

void loop() {
  unsigned long currentTime = millis(); // Cập nhật thời gian hệ thống liên tục tính từ lúc bật nguồn
  bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW); // Nút nhấn tích cực mức THẤP (đang được bấm)

  // Phân luồng xử lý dựa trên máy trạng thái hiện tại
  switch (currentState) {
    case START:
      if (buttonPressed) {
        playTone(1000, 100);
        delay(250); // Chống dính nút nhấn do nhiễu cơ học khi bắt đầu vào game
        resetGame();
      }
      break;

    case PLAYING:
      // 1. XỬ LÝ LOGIC NHẢY (CƠ CHẾ PHI TUẦN TỰ - NON-BLOCKING)
      if (buttonPressed && !isJumping) {
        isJumping = true;
        jumpStartTime = currentTime; // Ghi nhận thời điểm bắt đầu nhảy lên
        playTone(600, 50); // Phát tiếng píp ngắn báo hiệu nhảy
      }

      // Tự động chuyển trạng thái rơi xuống đất khi hết thời gian trên không
      if (isJumping && (currentTime - jumpStartTime >= JUMP_DURATION)) {
        isJumping = false;
      }

      // 2. XỬ LÝ TỰ ĐỘNG DỊCH CHUYỂN VẬT CẢN XƯƠNG RỒNG
      if (currentTime - lastMoveTime >= currentSpeed) {
        lastMoveTime = currentTime; // Đóng dấu thời gian chu kỳ mới
        cactusPosition--;           // Dịch xương rồng sang trái 1 cột
        
        // Khi cây xương rồng chạy thoát ra khỏi rìa bên trái màn hình (Cột < 0)
        if (cactusPosition < 0) {
          cactusPosition = 15;      // Đưa xương rồng quay trở lại rìa bên phải (Cột 15)
          score++;                  // Ghi nhận 1 điểm
          playTone(1200, 30);       // Phát âm thanh báo hiệu ăn điểm thành công
          
          // Thuật toán tăng độ khó: Cứ sau mỗi 3 điểm, giảm thời gian chờ đi 25ms (Tối đa nhanh tới 100ms)
          if (score % 3 == 0 && currentSpeed > 100) {
            currentSpeed -= 25; 
          }
        }

        // 3. THUẬT TOÁN PHÁT HIỆN VA CHẠM (COLLISION DETECTION)
        // Nếu vị trí xương rồng trùng cột Khủng long VÀ Khủng long đang ở mặt đất (Không nhảy)
        if (cactusPosition == DINO_COLUMN && !isJumping) {
          currentState = GAME_OVER; // Chuyển sang trạng thái Game Over ngay lập tức
          playGameOver();
          break; // Thoát khỏi luồng xử lý của PLAYING để đóng băng màn hình game
        }

        // 4. VẼ LẠI KHUNG CẢNH TRÒ CHƠI
        renderGame();
      }
      break;

    case GAME_OVER:
      if (buttonPressed) {
        playTone(1000, 100);
        delay(250); // Chống dính nút nhấn khi bấm chơi lại lượt mới
        resetGame();
      }
      break;
  }
}

// ====================================================================
// QUẢN LÝ ĐỒ HỌA & HIỂN THỊ MÀN HÌNH
// ====================================================================

// Hàm cập nhật đồ họa sử dụng phương pháp Ghi Đè Cục Bộ (Partial Redraw)
// Giúp xử lý dứt điểm hiện tượng giật, nhấp nháy chữ do việc lạm dụng hàm lcd.clear()
void renderGame() {
  // 1. Cập nhật Điểm số: Chỉ định vị con trỏ và in đè lên vùng góc phải màn hình
  lcd.setCursor(12, 0);
  lcd.print(score);

  // 2. Kết xuất đồ họa Khủng long: Hoán đổi vị trí Dòng tùy theo trạng thái nhảy
  if (isJumping) {
    lcd.setCursor(DINO_COLUMN, 1); lcd.print(" ");      // Xóa bóng ma ở dòng dưới mặt đất
    lcd.setCursor(DINO_COLUMN, 0); lcd.write(0);         // In ký tự Khủng long (ô nhớ 0) lên dòng trên
  } else {
    lcd.setCursor(DINO_COLUMN, 0); lcd.print(" ");      // Xóa bóng ma ở dòng trên không trung
    lcd.setCursor(DINO_COLUMN, 1); lcd.write(0);         // In ký tự Khủng long (ô nhớ 0) xuống dòng dưới
  }

  // 3. Kết xuất đồ họa Xương rồng: Chỉ thực hiện khi vị trí cột thay đổi so với chu kỳ trước
  if (cactusPosition != lastCactusPos) {
    // Điều kiện xóa: Vị trí cũ hợp lệ và không đè lên vùng đồ họa an toàn của Khủng long ở dòng dưới
    if (lastCactusPos >= 0 && lastCactusPos != DINO_COLUMN) { 
      lcd.setCursor(lastCactusPos, 1);
      lcd.print(" "); // Ghi đè ký tự khoảng trắng để xóa vết tích cũ
    }
    // Vẽ xương rồng tại tọa độ mới nếu nằm trong phạm vi hiển thị từ cột 0 đến 15
    if (cactusPosition >= 0) {
      lcd.setCursor(cactusPosition, 1);
      lcd.write(1); // In ký tự Xương rồng (ô nhớ 1)
    }
    // Lưu lại tọa độ hiện tại làm mốc tham chiếu xóa cho chu kỳ kế tiếp
    lastCactusPos = cactusPosition;
  }
}

// Hiển thị màn hình giao diện chờ ban đầu khi vừa khởi động hệ thống
void showStartScreen() {
  lcd.clear(); // Xóa sạch bộ đệm toàn bộ màn hình
  lcd.setCursor(3, 0);
  lcd.print("DINO GAME");
  lcd.setCursor(1, 1);
  lcd.print("Press to Start");
}

// Hiển thị màn hình báo tử khi xảy ra va chạm và phát âm thanh thua cuộc
void playGameOver() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("GAME OVER");
  lcd.setCursor(2, 1);
  lcd.print("Score: ");
  lcd.print(score);
  
  // Tạo hiệu ứng âm thanh thất bại (Hạ tần số từ cao xuống thấp)
  tone(BUZZER_PIN, 400, 200);
  delay(200);
  tone(BUZZER_PIN, 250, 400);
}

// Khởi động lại các tham số trò chơi về giá trị ban đầu để chuẩn bị cho lượt chơi mới
void resetGame() {
  lcd.clear(); 
  score = 0;
  cactusPosition = 15;
  lastCactusPos = 15; 
  currentSpeed = BASE_SPEED;
  isJumping = false;
  currentState = PLAYING;  // Kích hoạt máy trạng thái sang luồng chơi game chính thức
  lastMoveTime = millis(); // Đồng bộ lại gốc thời gian chạy đa nhiệm ban đầu
}