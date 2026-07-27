const byte PIN_DAT = A0;

// Thay hai gia tri nay sau khi do thuc te.
int GIA_TRI_KHO = 800;
int GIA_TRI_UOT = 350;

void setup() {
  Serial.begin(9600);
}

void loop() {
  long tong = 0;
  for (byte i = 0; i < 10; i++) {
    tong += analogRead(PIN_DAT);
    delay(10);
  }
  int raw = tong / 10;
  int phanTram = map(raw, GIA_TRI_KHO, GIA_TRI_UOT, 0, 100);
  phanTram = constrain(phanTram, 0, 100);

  Serial.print(F("RAW="));
  Serial.print(raw);
  Serial.print(F("  DO AM="));
  Serial.print(phanTram);
  Serial.println('%');
  delay(500);
}

