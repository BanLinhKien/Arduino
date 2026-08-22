/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

// --- KHAI BÁO CẤU HÌNH ---
const int PIN_TRIG = 10;
const int PIN_ECHO = 9;
const int PIN_BUZZER = 11;

// Mảng chứa chân điều khiển 7 LED cảnh báo
const int PIN_LEDS[] = {8, 7, 6, 5, 4, 3, 2}; 

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  // Cài đặt OUTPUT cho toàn bộ LED trong mảng
  for (int i = 0; i < 7; i++) {
    pinMode(PIN_LEDS[i], OUTPUT);
  }
  
  Serial.begin(9600);
  Serial.println("He thong canh bao muc nuoc - BANLINHKIEN.COM");
}

void loop() {
  // 1. ĐO KHOẢNG CÁCH SIÊU ÂM
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);
  float distance = (duration * 0.0343) / 2;

  // 2. XÁC ĐỊNH MỨC CẢNH BÁO (LEVEL)
  int level = 0;

  if (distance < 5)        level = 7;
  else if (distance < 10)  level = 6;
  else if (distance < 15)  level = 5;
  else if (distance < 20)  level = 4;
  else if (distance < 25)  level = 3;
  else if (distance < 30)  level = 2;
  else                     level = 1;

  // 3. HIỂN THỊ TRÊN DÃY LED
  // Bật các LED từ 0 đến level, tắt các LED còn lại
  for (int i = 0; i < 7; i++) {
    if (i < level) {
      digitalWrite(PIN_LEDS[i], HIGH);
    } else {
      digitalWrite(PIN_LEDS[i], LOW);
    }
  }

  // 4. CẢNH BÁO CÒI (Khi mức cao nhất - Vật cản quá gần)
  if (level == 7) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(50);
    digitalWrite(PIN_BUZZER, LOW);
    delay(50);
    digitalWrite(PIN_BUZZER, HIGH);
    delay(50);
    digitalWrite(PIN_BUZZER, LOW);
  }

  // 5. IN THÔNG TIN LÊN SERIAL MONITOR
  Serial.print("Khoang cach: ");
  Serial.print(distance);
  Serial.print(" cm | Muc: ");
  Serial.println(level);

  delay(100); // Đợi 0.1s cho chu kỳ tiếp theo
}