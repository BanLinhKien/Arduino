/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_BUZZER = 9; 

// 1. ĐỊNH NGHĨA TẤT CẢ CÁC NỐT QUÃNG 7 
#define NOTE_C7  2093
#define NOTE_D7  2349
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_G7  3136
#define NOTE_A7  3520
#define NOTE_B7  3951

// 2. MẢNG GIAI ĐIỆU (Sử dụng các nốt đã khai báo ở trên)
int melody[] = {
  NOTE_C7, NOTE_C7, NOTE_G7, NOTE_G7, NOTE_A7, NOTE_A7, NOTE_G7,
  NOTE_F7, NOTE_F7, NOTE_E7, NOTE_E7, NOTE_D7, NOTE_D7, NOTE_C7,
  NOTE_G7, NOTE_G7, NOTE_F7, NOTE_F7, NOTE_E7, NOTE_E7, NOTE_D7,
  NOTE_G7, NOTE_G7, NOTE_F7, NOTE_F7, NOTE_E7, NOTE_E7, NOTE_D7
};

// Mảng chứa thời gian phát mỗi nốt (ms)
int noteDurations[] = {
  500, 500, 500, 500, 500, 500, 1000,
  500, 500, 500, 500, 500, 500, 1000,
  500, 500, 500, 500, 500, 500, 1000,
  500, 500, 500, 500, 500, 500, 1000
};

void setup() {
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop() {
  delay(2000); // Nghỉ 2 giây trước khi lặp lại

  for (int i = 0; i < 28; i++) {
    // Phát nốt nhạc
    tone(PIN_BUZZER, melody[i], noteDurations[i]);

    // Tạo khoảng nghỉ giữa các nốt
    int pauseBetweenNotes = noteDurations[i] * 1.30;
    delay(pauseBetweenNotes);

    // Ngắt âm thanh nốt cũ
    noTone(PIN_BUZZER);
  }
}