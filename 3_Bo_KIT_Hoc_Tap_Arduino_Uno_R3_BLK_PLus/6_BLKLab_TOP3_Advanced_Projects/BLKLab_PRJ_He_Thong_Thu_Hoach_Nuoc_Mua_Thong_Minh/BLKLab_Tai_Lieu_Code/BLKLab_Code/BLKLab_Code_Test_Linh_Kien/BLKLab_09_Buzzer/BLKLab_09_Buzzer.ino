const byte PIN_BUZZER = 12;

void setup() {
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
}

void loop() {
  // Test cho active buzzer nho, dong <= 20 mA.
  digitalWrite(PIN_BUZZER, HIGH);
  delay(150);
  digitalWrite(PIN_BUZZER, LOW);
  delay(850);

  // Neu la passive buzzer, thay khoi tren bang:
  // tone(PIN_BUZZER, 2000, 150);
  // delay(1000);
}

