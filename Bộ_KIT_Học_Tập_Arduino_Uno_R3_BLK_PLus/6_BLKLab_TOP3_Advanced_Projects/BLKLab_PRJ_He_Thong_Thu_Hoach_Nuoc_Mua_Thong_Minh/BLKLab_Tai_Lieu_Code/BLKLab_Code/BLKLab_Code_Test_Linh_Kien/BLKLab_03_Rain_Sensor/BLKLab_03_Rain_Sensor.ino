const byte PIN_MUA = 2;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_MUA, INPUT);
}

void loop() {
  int giaTri = digitalRead(PIN_MUA);

  // Phan lon module LM393: LOW = co mua, HIGH = khong mua.
  if (giaTri == LOW) {
    Serial.println(F("CO MUA / MAT CAM BIEN DANG UOT"));
  } else {
    Serial.println(F("KHONG MUA / MAT CAM BIEN KHO"));
  }

  delay(300);
}

