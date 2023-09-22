/*
 * =========================================================================================================================*
 *                                                     BANLINHKIEN.COM
 * =========================================================================================================================*                           
 *                                             Thiết kế đàn piano bằng nút nhấn 
 * Đấu nối: 
 + Phím Bấm:
  -D10: nối chân (pin1) của phím bấm 1     
  -D9: nối chân (pin1) của phím bấm 2    
  -D8: nối chân (pin1) của phím bấm 3      
  -D7: nối chân (pin1) của phím bấm 4
  -D6: nối chân (pin1) của phím bấm 5
  -D5: nối chân (pin1) của phím bấm 6    
  -D4: nối chân (pin1) của phím bấm 7
  -D3: nối chân (pin1) của phím bấm 8
  - Chú thích: + pin 1: (chân nối với Arduino)
               + pin 2: (chân nối với đất - GND)
 + Loa:
  -D2: nối chân Anot của loa   

 * Hướng dẫn chơi happy birthday đơn giản:
   Ta đánh theo nhịp: 1 1 2 1 4 3,                 1 1 2 1 5 4,              1 1 8 6 4 3 2
                     Happy birthday to you,    happy birthday to you,      happy birthday to…
*/

const int p1 = 10; 
const int p2 = 9;  
const int p3 = 8;  
const int p4 = 7;  
const int p5 = 6;  
const int p6 = 5;  
const int p7 = 4;  
const int p8 = 3;  

const int OFF = LOW;                                                // Chế độ tắt (độ thấp)
const int ON = HIGH;                                                // Chế độ bật (độ cao)

// Định nghĩa các âm thanh bằng tần số (đơn vị là Hz)
const int NOTE_C4 = 262;
const int NOTE_D4 = 294;
const int NOTE_E4 = 330;
const int NOTE_F4 = 349;
const int NOTE_G4 = 392;
const int NOTE_A4 = 440;
const int NOTE_B4 = 494;
const int NOTE_C5 = 523;

// Hàm phát ra âm thanh
void playSound(int frequency) {
  tone(2, frequency);                                               // Sử dụng chân 2 để tạo âm thanh với tần số cho trước
  delay(500);                                                       // Giữ âm thanh trong 500ms
  noTone(2);                                                        // Tắt âm thanh trên chân 2
}

void setup() {
  // Khởi tạo chân GPIO
  pinMode(p1, INPUT_PULLUP);
  pinMode(p2, INPUT_PULLUP);
  pinMode(p3, INPUT_PULLUP);
  pinMode(p4, INPUT_PULLUP);
  pinMode(p5, INPUT_PULLUP);
  pinMode(p6, INPUT_PULLUP);
  pinMode(p7, INPUT_PULLUP);
  pinMode(p8, INPUT_PULLUP);

  // Khởi tạo Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // Kiểm tra các phím bấm
  if (digitalRead(p1) == OFF) {
    playSound(NOTE_C4);                                             // Phát âm thanh tương ứng với phím 1
    Serial.println("Phim 1 duoc nhan");
  }
  if (digitalRead(p2) == OFF) {
    playSound(NOTE_D4);                                             // Phát âm thanh tương ứng với phím 2
    Serial.println("Phim 2 duoc nhan");
  }
  if (digitalRead(p3) == OFF) {
    playSound(NOTE_E4);                                             // Phát âm thanh tương ứng với phím 3
    Serial.println("Phim 3 duoc nhan");
  }
  if (digitalRead(p4) == OFF) {
    playSound(NOTE_F4);                                             // Phát âm thanh tương ứng với phím 4
    Serial.println("Phim 4 duoc nhan");
  }
  if (digitalRead(p5) == OFF) {
    playSound(NOTE_G4);                                             // Phát âm thanh tương ứng với phím 5
    Serial.println("Phim 5 duoc nhan");
  }
  if (digitalRead(p6) == OFF) {
    playSound(NOTE_A4);                                             // Phát âm thanh tương ứng với phím 6
    Serial.println("Phim 6 duoc nhan");
  }
  if (digitalRead(p7) == OFF) {
    playSound(NOTE_B4);                                             // Phát âm thanh tương ứng với phím 7
    Serial.println("Phim 7 duoc nhan");
  }
  if (digitalRead(p8) == OFF) {
    playSound(NOTE_C5);                                             // Phát âm thanh tương ứng với phím 8
    Serial.println("Phim 8 duoc nhan");
  }
}
