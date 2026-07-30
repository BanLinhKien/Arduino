/* =================================================================================================*
 *                                          BANLINHKIEN.COM                                         *
 *==================================================================================================*                        
 *                        PRJ01: Làm Hiệu Ứng Siêu Đẹp Với LED Đơn Bằng Arduino 
 * 
 * Đấu nối: nối lần lượt các chân 2-11 của Arduino với 10 LED đơn
 * D2: kết nối với LED 1                             D7: kết nối với LED 6
 * D3: kết nối với LED 2                             D8: kết nối với LED 7
 * D4: kết nối với LED 3                             D9: kết nối với LED 8
 * D5: kết nối với LED 4                             D10: kết nối với LED 9
 * D6: kết nối với LED 5                             D11: kết nối với LED 10 
 * D12: kết nối với Nút nhấn
 */

 // Định nghĩa các chân LED và nút nhấn
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};   // 10 chân LED
const int numLeds = 10;
const int buttonPin = 12;                                 // Chân nút nhấn
const unsigned long delayTime = 100;                      // Thời gian trễ cơ bản (ms)
int effect = 0;                                           // Biến chọn hiệu ứng
int lastButtonState = HIGH;                               // Trạng thái nút nhấn trước đó
int buttonState = HIGH;                                   // Trạng thái nút nhấn hiện tại
unsigned long lastDebounceTime = 0;                       // Thời gian debounce cuối
const unsigned long debounceDelay = 30;                   // Thời gian debounce (ms)
bool buttonPressed = false;                               // Cờ chống nhấn giữ
unsigned long previousMillis = 0;                         // Thời gian lần cập nhật trước
int effectStep = 0;                                       // Bước hiện tại của hiệu ứng
int effectSubStep = 0;                                    // Bước phụ 

// Biến trạng thái cho các hiệu ứng cụ thể
int randomLed = 0;                                        // Cho effect9 và effect24
int groupSize = 0;                                        // Cho effect26
int start = 0;                                            // Cho effect26
int waveLength = 0;                                       // Cho effect30

void setup() {
  for (int i = 0; i < numLeds; i++) {                     // Cấu hình các chân LED là OUTPUT
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buttonPin, INPUT_PULLUP);                       // Cấu hình nút nhấn là INPUT với điện trở kéo lên
  Serial.begin(9600);                                     // Khởi tạo Serial để debug
  clearLeds();                                            // Tắt tất cả LED ban đầu
}

void loop() {
  int reading = digitalRead(buttonPin);                   // Đọc trạng thái nút nhấn
  if (reading != lastButtonState) {                       // Kiểm tra nếu trạng thái nút thay đổi
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {    // Kiểm tra nếu đã qua thời gian debounce
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW && !buttonPressed) {         // Chuyển hiệu ứng khi nhấn nút (LOW) và chưa được xử lý
        effect = (effect + 1) % 30;
        buttonPressed = true;
        effectStep = 0;                                   // Reset bước hiệu ứng
        effectSubStep = 0;                                // Reset bước phụ
        clearLeds();                                      // Tắt LED để chuyển mượt
        Serial.print("Hiệu ứng hiển thị: ");
        Serial.println(effect);
      }
      if (buttonState == HIGH) {                          // Reset cờ khi thả nút
        buttonPressed = false;
      }
    }
  }
  lastButtonState = reading;

  // Cập nhật hiệu ứng nếu đã đến thời gian
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= delayTime) {
    previousMillis = currentMillis;
    updateEffect();
  }
}

//-----------------Hàm tắt tất cả LED---------------------------
void clearLeds() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

//----------------- Hàm cập nhật hiệu ứng dựa trên effect và effectStep---------------
void updateEffect() {
  switch (effect) {
    case 0: effect1(); break;
    case 1: effect2(); break;
    case 2: effect3(); break;
    case 3: effect4(); break;
    case 4: effect5(); break;
    case 5: effect6(); break;
    case 6: effect7(); break;
    case 7: effect8(); break;
    case 8: effect9(); break;
    case 9: effect10(); break;
    case 10: effect11(); break;
    case 11: effect12(); break;
    case 12: effect13(); break;
    case 13: effect14(); break;
    case 14: effect15(); break;
    case 15: effect16(); break;
    case 16: effect17(); break;
    case 17: effect18(); break;
    case 18: effect19(); break;
    case 19: effect20(); break;
    case 20: effect21(); break;
    case 21: effect22(); break;
    case 22: effect23(); break;
    case 23: effect24(); break;
    case 24: effect25(); break;
    case 25: effect26(); break;
    case 26: effect27(); break;
    case 27: effect28(); break;
    case 28: effect29(); break;
    case 29: effect30(); break;
  }
}

// Hiệu ứng 1: Chạy qua từng LED từ trái sang phải
void effect1() {
  if (effectStep < numLeds) {
    clearLeds();
    digitalWrite(ledPins[effectStep], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 2: Chạy ngược lại từ phải sang trái
void effect2() {
  if (effectStep < numLeds) {
    clearLeds();
    digitalWrite(ledPins[numLeds - 1 - effectStep], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 3: Nhấp nháy toàn bộ LED
void effect3() {
  if (effectStep < 10) { // 5 lần bật/tắt = 10 bước
    clearLeds();
    if (effectStep % 2 == 0) {
      for (int j = 0; j < numLeds; j++) {
        digitalWrite(ledPins[j], HIGH);
      }
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 4: Sóng từ trái sang phải
void effect4() {
  if (effectStep < numLeds) {
    digitalWrite(ledPins[effectStep], HIGH);
    effectStep++;
  } else if (effectStep < 2 * numLeds) {
    digitalWrite(ledPins[effectStep - numLeds], LOW);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 5: Sóng từ phải sang trái
void effect5() {
  if (effectStep < numLeds) {
    digitalWrite(ledPins[numLeds - 1 - effectStep], HIGH);
    effectStep++;
  } else if (effectStep < 2 * numLeds) {
    digitalWrite(ledPins[numLeds - 1 - (effectStep - numLeds)], LOW);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 6: Nhấp nháy xen kẽ
void effect6() {
  if (effectStep < 10) { // 5 lần bật/tắt = 10 bước
    clearLeds();
    if (effectStep % 2 == 0) {
      for (int j = 0; j < numLeds; j += 2) {
        digitalWrite(ledPins[j], HIGH);
      }
    } else {
      for (int j = 1; j < numLeds; j += 2) {
        digitalWrite(ledPins[j], HIGH);
      }
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 7: Đuổi từ giữa ra ngoài
void effect7() {
  if (effectStep < numLeds / 2) {
    clearLeds();
    digitalWrite(ledPins[numLeds / 2 - effectStep - 1], HIGH);
    digitalWrite(ledPins[numLeds / 2 + effectStep], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 8: Đuổi từ ngoài vào giữa
void effect8() {
  if (effectStep < numLeds / 2) {
    clearLeds();
    digitalWrite(ledPins[effectStep], HIGH);
    digitalWrite(ledPins[numLeds - 1 - effectStep], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 9: Bật LED ngẫu nhiên
void effect9() {
  if (effectStep < 20) {
    clearLeds();
    randomLed = random(0, numLeds);
    digitalWrite(ledPins[randomLed], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 10: Sóng đôi
void effect10() {
  if (effectStep < numLeds - 1) {
    clearLeds();
    digitalWrite(ledPins[effectStep], HIGH);
    digitalWrite(ledPins[effectStep + 1], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 11: Nhấp nháy từng cặp
void effect11() {
  if (effectStep < 10) { // 5 lần bật/tắt
    clearLeds();
    if (effectStep % 2 == 0) {
      for (int j = 0; j < numLeds - 1; j += 2) {
        digitalWrite(ledPins[j], HIGH);
        digitalWrite(ledPins[j + 1], HIGH);
      }
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 12: Chạy kép
void effect12() {
  if (effectStep < numLeds - 1) {
    clearLeds();
    digitalWrite(ledPins[effectStep], HIGH);
    digitalWrite(ledPins[effectStep + 1], HIGH);
    effectStep++;
  } else if (effectStep < 2 * (numLeds - 1)) {
    clearLeds();
    digitalWrite(ledPins[2 * (numLeds - 1) - effectStep - 1], HIGH);
    digitalWrite(ledPins[2 * (numLeds - 1) - effectStep], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 13: Nhấp nháy nhanh chậm
void effect13() {
  if (effectStep < 6) { // 3 lần nhanh
    clearLeds();
    if (effectStep % 2 == 0) {
      for (int j = 0; j < numLeds; j++) {
        digitalWrite(ledPins[j], HIGH);
      }
    }
    effectStep++;
    previousMillis -= delayTime - 50; // Nhanh (50ms mỗi bước)
  } else if (effectStep < 12) { // 3 lần chậm
    clearLeds();
    if ((effectStep - 6) % 2 == 0) {
      for (int j = 0; j < numLeds; j++) {
        digitalWrite(ledPins[j], HIGH);
      }
    }
    effectStep++;
    previousMillis += delayTime; // Chậm (thêm 100ms)
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 14: Sóng từ hai đầu
void effect14() {
  if (effectStep < numLeds / 2) {
    clearLeds();
    digitalWrite(ledPins[effectStep], HIGH);
    digitalWrite(ledPins[numLeds - 1 - effectStep], HIGH);
    effectStep++;
  } else if (effectStep < numLeds) {
    clearLeds();
    digitalWrite(ledPins[numLeds - effectStep - 1], HIGH);
    digitalWrite(ledPins[effectStep], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 15: Tăng dần số LED bật
void effect15() {
  if (effectStep <= numLeds) {
    clearLeds();
    for (int j = 0; j < effectStep; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 16: Giảm dần số LED bật
void effect16() {
  if (effectStep <= numLeds) {
    clearLeds();
    for (int j = 0; j < numLeds - effectStep; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 17: Nhấp nháy ngẫu nhiên
void effect17() {
  if (effectStep < 20) {
    clearLeds();
    for (int j = 0; j < numLeds; j++) {
      if (random(0, 2) == 1) {
        digitalWrite(ledPins[j], HIGH);
      }
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 18: Chạy từng nhóm 3 LED
void effect18() {
  if (effectStep < numLeds - 2) {
    clearLeds();
    digitalWrite(ledPins[effectStep], HIGH);
    digitalWrite(ledPins[effectStep + 1], HIGH);
    digitalWrite(ledPins[effectStep + 2], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 19: Sóng xen kẽ
void effect19() {
  if (effectStep < 10) { // 5 lần bật/tắt
    clearLeds();
    if (effectStep % 2 == 0) {
      for (int j = 0; j < numLeds; j += 2) {
        digitalWrite(ledPins[j], HIGH);
      }
    } else {
      for (int j = 1; j < numLeds; j += 2) {
        digitalWrite(ledPins[j], HIGH);
      }
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 20: Nhấp nháy toàn bộ với tốc độ ngẫu nhiên
void effect20() {
  if (effectStep < 10) {
    clearLeds();
    if (effectStep % 2 == 0) {
      for (int j = 0; j < numLeds; j++) {
        digitalWrite(ledPins[j], HIGH);
      }
    }
    effectStep++;
    previousMillis += random(50, 200); // Tốc độ ngẫu nhiên
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 21: Xoáy tròn
void effect21() {
  if (effectStep < 10) {
    clearLeds();
    digitalWrite(ledPins[effectStep % numLeds], HIGH);
    digitalWrite(ledPins[(effectStep + 3) % numLeds], HIGH);
    digitalWrite(ledPins[(effectStep + 6) % numLeds], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 22: Nhịp tim
void effect22() {
  if (effectStep < 3) {
    if (effectSubStep <= 51) { // 255/5 ≈ 51 bước
      for (int j = 0; j < numLeds; j++) {
        analogWrite(ledPins[j], effectSubStep * 5);
      }
      effectSubStep++;
      previousMillis -= delayTime - 10; // Tăng tốc (10ms mỗi bước)
    } else {
      clearLeds();
      effectSubStep = 0;
      effectStep++;
      previousMillis += 50; // Tạm dừng 50ms
    }
  } else {
    effectStep = 0;
    effectSubStep = 0;
  }
}

// Hiệu ứng 23: Cầu vồng
void effect23() {
  if (effectStep < numLeds) {
    clearLeds();
    for (int j = 0; j < 3; j++) {
      digitalWrite(ledPins[(effectStep + j) % numLeds], HIGH);
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 24: Tia lửa
void effect24() {
  if (effectStep < 30) {
    clearLeds();
    if (effectStep % 2 == 0) {
      randomLed = random(0, numLeds);
      digitalWrite(ledPins[randomLed], HIGH);
    }
    effectStep++;
    previousMillis -= delayTime - 50; // Tăng tốc (50ms mỗi bước)
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 25: Sóng đối xứng
void effect25() {
  if (effectStep < numLeds / 2){
    clearLeds();
    for (int j = 0; j <= effectStep; j++) {
      digitalWrite(ledPins[j], HIGH);
      digitalWrite(ledPins[numLeds - 1 - j], HIGH);
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 26: Nhấp nháy nhóm ngẫu nhiên
void effect26() {
  if (effectStep == 0) {
    groupSize = random(2, 5);
    start = random(0, numLeds - groupSize + 1);
  }
  if (effectStep < 10) {
    clearLeds();
    for (int j = start; j < start + groupSize; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 27: Xoay vòng
void effect27() {
  if (effectStep < numLeds) {
    clearLeds();
    digitalWrite(ledPins[effectStep % numLeds], HIGH);
    digitalWrite(ledPins[(effectStep + 5) % numLeds], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 28: Đuổi kép ngược
void effect28() {
  if (effectStep < numLeds) {
    clearLeds();
    digitalWrite(ledPins[effectStep], HIGH);
    digitalWrite(ledPins[numLeds - 1 - effectStep], HIGH);
    effectStep++;
  } else {
    effectStep = 0;
  }
}

// Hiệu ứng 29: Nhấp nháy giảm dần
void effect29() {
  if (effectStep < numLeds) {
    clearLeds();
    if (effectSubStep == 0) {
      for (int j = 0; j < numLeds - effectStep; j++) {
        digitalWrite(ledPins[j], HIGH);
      }
    }
    effectSubStep = (effectSubStep + 1) % 2;
    if (effectSubStep == 0) {
      effectStep++;
    }
  } else {
    effectStep = 0;
    effectSubStep = 0;
  }
}

// Hiệu ứng 30: Sóng ngẫu nhiên
void effect30() {
  if (effectStep == 0) {
    waveLength = random(2, 5);
  }
  if (effectStep < 5) {
    clearLeds();
    for (int j = 0; j < waveLength; j++) {
      digitalWrite(ledPins[(effectStep + j) % numLeds], HIGH);
    }
    effectStep++;
  } else {
    effectStep = 0;
  }
}