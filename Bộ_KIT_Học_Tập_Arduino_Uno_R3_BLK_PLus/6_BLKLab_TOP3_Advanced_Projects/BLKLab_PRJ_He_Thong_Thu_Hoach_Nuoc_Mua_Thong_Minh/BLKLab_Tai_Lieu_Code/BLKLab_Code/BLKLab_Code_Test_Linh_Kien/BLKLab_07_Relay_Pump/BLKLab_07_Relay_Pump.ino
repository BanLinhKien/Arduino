const byte PIN_RELAY = 8;

// Doi cho nhau neu relay cua ban kich muc HIGH.
const byte RELAY_ON = LOW;
const byte RELAY_OFF = HIGH;

void setup() {
  Serial.begin(9600);
  digitalWrite(PIN_RELAY, RELAY_OFF); // Dat trang thai an toan truoc.
  pinMode(PIN_RELAY, OUTPUT);
  Serial.println(F("Relay mac dinh OFF"));
}

void loop() {
  Serial.println(F("BOM ON trong 2 giay"));
  digitalWrite(PIN_RELAY, RELAY_ON);
  delay(2000);

  Serial.println(F("BOM OFF trong 5 giay"));
  digitalWrite(PIN_RELAY, RELAY_OFF);
  delay(5000);
}

