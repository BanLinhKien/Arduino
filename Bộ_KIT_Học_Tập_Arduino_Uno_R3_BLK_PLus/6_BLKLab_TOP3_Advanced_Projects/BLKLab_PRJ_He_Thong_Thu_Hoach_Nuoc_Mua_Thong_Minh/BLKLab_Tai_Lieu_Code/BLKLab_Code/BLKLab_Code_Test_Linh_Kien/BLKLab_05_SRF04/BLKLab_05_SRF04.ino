const byte PIN_TRIG = 3;
const byte PIN_ECHO = 4;

// Khoang cach tu mat cam bien den day be khi be rong.
// Do thuc te va thay doi gia tri nay cho dung voi mo hinh.
const float CHIEU_CAO_BE_CM = 10.0;

float doKhoangCachCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(3);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Timeout 30 ms, tuong duong khoang cach toi da khoang 5 m.
  unsigned long thoiGian = pulseIn(PIN_ECHO, HIGH, 30000UL);
  if (thoiGian == 0) return -1.0;

  return thoiGian * 0.0343 / 2.0;
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  Serial.println(F("TEST HC-SR04"));
}

void loop() {
  // Lay trung binh cac phep do hop le de giam dao dong.
  float tong = 0;
  byte soMau = 0;

  for (byte i = 0; i < 5; i++) {
    float khoangCach = doKhoangCachCm();
    if (khoangCach >= 2.0 && khoangCach <= 400.0) {
      tong += khoangCach;
      soMau++;
    }
    delay(60);
  }

  if (soMau == 0) {
    Serial.println(F("LOI: Khong nhan duoc ECHO hop le"));
    delay(500);
    return;
  }

  float khoangCach = tong / soMau;
  float chieuCaoNuoc = CHIEU_CAO_BE_CM - khoangCach;
  float phanTram = chieuCaoNuoc * 100.0 / CHIEU_CAO_BE_CM;
  phanTram = constrain(phanTram, 0.0, 100.0);

  Serial.print(F("Khoang cach: "));
  Serial.print(khoangCach, 1);
  Serial.print(F(" cm | Muc nuoc: "));
  Serial.print(phanTram, 1);
  Serial.println('%');

  delay(500);
}

