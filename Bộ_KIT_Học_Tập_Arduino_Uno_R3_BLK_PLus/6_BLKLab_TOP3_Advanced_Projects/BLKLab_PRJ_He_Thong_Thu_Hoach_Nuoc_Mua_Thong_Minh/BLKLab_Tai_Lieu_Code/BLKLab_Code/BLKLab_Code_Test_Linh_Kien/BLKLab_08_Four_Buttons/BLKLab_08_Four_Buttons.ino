const byte PIN_UP = 5;
const byte PIN_DOWN = 6;
const byte PIN_LEFT = 7;
const byte PIN_RIGHT = 10;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  Serial.println(F("Nhan tung nut de kiem tra"));
}

void loop() {
  if (digitalRead(PIN_UP) == LOW) {
    Serial.println(F("UP"));
    delay(200);
  }
  if (digitalRead(PIN_DOWN) == LOW) {
    Serial.println(F("DOWN"));
    delay(200);
  }
  if (digitalRead(PIN_LEFT) == LOW) {
    Serial.println(F("LEFT"));
    delay(200);
  }
  if (digitalRead(PIN_RIGHT) == LOW) {
    Serial.println(F("RIGHT"));
    delay(200);
  }

  if (digitalRead(PIN_LEFT) == LOW && digitalRead(PIN_RIGHT) == LOW) {
    Serial.println(F("LEFT + RIGHT: DUNG KHAN CAP"));
    delay(500);
  }
}

