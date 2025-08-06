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
  -D11: nối chân (pin1) của phím 9
  -D12: nối chân (pin1) của phím 10
  - Chú thích: + pin 1: (chân nối với Arduino)
               + pin 2: (chân nối với đất - GND)
 + Loa:
  -D2: nối chân Anot của loa   

*/

const int p1 = 10; 
const int p2 = 9;  
const int p3 = 8;  
const int p4 = 7;  
const int p5 = 6;  
const int p6 = 5;  
const int p7 = 4;  
const int p8 = 3;  
const int p9 = 11; 
const int p10 =12 ; 
const int OFF = LOW;                                                // Chế độ tắt (độ thấp)
const int ON = HIGH;                                                // Chế độ bật (độ cao)

// Định nghĩa các âm thanh bằng tần số (đơn vị là Hz)
const int NOTE_do = 820;
const int NOTE_re = 880;
const int NOTE_mi = 956;
const int NOTE_pha = 1014;
const int NOTE_sol = 1136;
const int NOTE_la =  1275;
const int NOTE_si  = 1432;
const int NOTE_do2 = 1519;
const int NOTE_re2 = 1700;
const int NOTE_mi2 = 1915;

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
  pinMode(p9, INPUT_PULLUP);
  pinMode(p10, INPUT_PULLUP);

  // Khởi tạo Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // Kiểm tra các phím bấm
  if (digitalRead(p1) == OFF) {
    playSound(NOTE_do);                                             // Phát âm thanh tương ứng với phím 1
    Serial.println("Phim 1 duoc nhan");
  }
  if (digitalRead(p2) == OFF) {
    playSound(NOTE_re);                                             // Phát âm thanh tương ứng với phím 2
    Serial.println("Phim 2 duoc nhan");
  }
  if (digitalRead(p3) == OFF) {
    playSound(NOTE_mi);                                             // Phát âm thanh tương ứng với phím 3
    Serial.println("Phim 3 duoc nhan");
  }
  if (digitalRead(p4) == OFF) {
    playSound(NOTE_pha);                                             // Phát âm thanh tương ứng với phím 4
    Serial.println("Phim 4 duoc nhan");
  }
  if (digitalRead(p5) == OFF) {
    playSound(NOTE_sol);                                             // Phát âm thanh tương ứng với phím 5
    Serial.println("Phim 5 duoc nhan");
  }
  if (digitalRead(p6) == OFF) {
    playSound(NOTE_la);                                             // Phát âm thanh tương ứng với phím 6
    Serial.println("Phim 6 duoc nhan");
  }
  if (digitalRead(p7) == OFF) {
    playSound(NOTE_si);                                             // Phát âm thanh tương ứng với phím 7
    Serial.println("Phim 7 duoc nhan");
  }
  if (digitalRead(p8) == OFF) {
    playSound(NOTE_do2);                                             // Phát âm thanh tương ứng với phím 8
    Serial.println("Phim 8 duoc nhan");
  }
  if (digitalRead(p9) == OFF) {
    playSound(NOTE_re2);                                             // Phát âm thanh tương ứng với phím 8
    Serial.println("Phim 9 duoc nhan");
  }
  if (digitalRead(p10) == OFF) {
    playSound(NOTE_mi2);                                             // Phát âm thanh tương ứng với phím 8
    Serial.println("Phim 10 duoc nhan");
  }
}
