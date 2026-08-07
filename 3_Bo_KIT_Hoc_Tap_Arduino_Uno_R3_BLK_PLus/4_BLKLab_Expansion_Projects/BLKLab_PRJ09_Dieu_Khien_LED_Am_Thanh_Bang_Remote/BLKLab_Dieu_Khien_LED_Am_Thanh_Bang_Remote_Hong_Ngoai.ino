// -------------------------------------------------------------
// CHUYỂN IRREMOTE SANG TIMER 1 ĐỂ KHÔNG XUNG ĐỘT VỚI TONE() & LED D3
// -------------------------------------------------------------
#define IR_USE_AVR_TIMER1 
#include <IRremote.hpp>

// -------------------------------------------------------------
// 1. ĐỊNH NGHĨA CHÂN CẮM HARDWARE
// -------------------------------------------------------------
const int IR_RECEIVE_PIN = 2;      // Mắt thu IR 1838T nối chân D2
const int LED_PINS[3] = {3, 4, 5}; // 3 LED nối chân D3, D4, D5
const int BUZZER_PIN = 11;         // Còi chip/Loa nối chân D11

// -------------------------------------------------------------
// 2. BẢNG MÃ HEX CỦA REMOTE (Chuẩn NEC)
// -------------------------------------------------------------
#define KEY_POWER 0x45  // Phím POWER / ON-OFF
#define KEY_1     0x0C  // Phím số 1 (Bật 1 đèn)
#define KEY_2     0x18  // Phím số 2 (Bật 2 đèn)
#define KEY_3     0x5E  // Phím số 3 (Bật 3 đèn)
#define KEY_4     0x08  // Phím số 4 (Phát nhạc Jingle Bells Full)

bool isPowerOn = false;

// -------------------------------------------------------------
// 3. THỜI GIAN DEBOUNCE
// -------------------------------------------------------------
unsigned long lastIRTime = 0;
const unsigned long DEBOUNCE_DELAY = 300;

// -------------------------------------------------------------
// 4. ĐỊNH NGHĨA BẢN NHẠC JINGLE BELLS FULL (TRỌN VẸN 51 NỐT)
// -------------------------------------------------------------
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392

int melody[] = {
  // --- Câu 1: Jingle bells, jingle bells, jingle all the way ---
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,

  // --- Câu 2: Oh what fun it is to ride in a one-horse open sleigh, hey! ---
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
  NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_G4,

  // --- Câu 3: Jingle bells, jingle bells, jingle all the way ---
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,

  // --- Câu 4: Oh what fun it is to ride in a one-horse open sleigh! (Kết bài) ---
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
  NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4
};

int noteDurations[] = {
  // --- Câu 1 ---
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8, 2,

  // --- Câu 2 ---
  8, 8, 8, 8,
  8, 8, 8, 8, 8,
  8, 8, 8, 8, 4, 4,

  // --- Câu 3 ---
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8, 2,

  // --- Câu 4 ---
  8, 8, 8, 8,
  8, 8, 8, 8, 8,
  8, 8, 8, 8, 2
};

void beep() {
  for (int i = 0; i < 100; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(250);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(250);
  }
}

void setLedCount(int count) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PINS[i], (i < count) ? HIGH : LOW);
  }
}

// Hàm phát nhạc Giáng Sinh
void playChristmasMelody() {
  Serial.println("-> Dang phat nhac Giang Sinh (Jingle Bells Full)...");
  
  // Dọn sạch tín hiệu dư thừa của phím bấm
  IrReceiver.resume();
  delay(250);
  if (IrReceiver.decode()) {
    IrReceiver.resume();
  }

  int totalNotes = sizeof(melody) / sizeof(melody[0]);
  
  for (int thisNote = 0; thisNote < totalNotes; thisNote++) {
    // Nếu bấm phím mới trong lúc đang phát -> Dừng bài
    if (IrReceiver.decode()) {
      if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        IrReceiver.resume();
      } else {
        IrReceiver.resume();
        noTone(BUZZER_PIN);
        Serial.println("-> Da dung phat nhac!");
        return;
      }
    }

    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // Nhấp nháy 3 LED luân phiên theo từng nốt nhạc
    digitalWrite(LED_PINS[thisNote % 3], HIGH);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    
    noTone(BUZZER_PIN);
    digitalWrite(LED_PINS[thisNote % 3], LOW);
  }

  // Phát xong nhạc -> Bật lại 3 LED
  setLedCount(3);
}

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 3; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  
  pinMode(BUZZER_PIN, OUTPUT);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("==============================================");
  Serial.println("HE THONG 3 LED + COI CHIP DA SAN SANG!");
  Serial.println("==============================================");

  beep();
}

void loop() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      IrReceiver.resume();
      return;
    }

    uint16_t command = IrReceiver.decodedIRData.command;

    if (command != 0 && (millis() - lastIRTime > DEBOUNCE_DELAY)) {
      lastIRTime = millis();

      Serial.print("Da nhan ma Phim: 0x");
      Serial.println(command, HEX);

      switch (command) {
        case KEY_POWER:
          beep();
          isPowerOn = !isPowerOn;
          setLedCount(isPowerOn ? 3 : 0);
          Serial.println(isPowerOn ? "NGUON: ON (Bat 3 den)" : "NGUON: OFF (Tat 3 den)");
          break;

        case KEY_1:
          beep();
          isPowerOn = true;
          setLedCount(1);
          Serial.println("Chuc nang: Bat 1 den");
          break;

        case KEY_2:
          beep();
          isPowerOn = true;
          setLedCount(2);
          Serial.println("Chuc nang: Bat 2 den");
          break;

        case KEY_3:
          beep();
          isPowerOn = true;
          setLedCount(3);
          Serial.println("Chuc nang: Bat 3 den");
          break;

        case KEY_4:
          beep();
          isPowerOn = true;
          playChristmasMelody();
          break;

        default:
          break;
      }
    }
    
    IrReceiver.resume();
  }
}