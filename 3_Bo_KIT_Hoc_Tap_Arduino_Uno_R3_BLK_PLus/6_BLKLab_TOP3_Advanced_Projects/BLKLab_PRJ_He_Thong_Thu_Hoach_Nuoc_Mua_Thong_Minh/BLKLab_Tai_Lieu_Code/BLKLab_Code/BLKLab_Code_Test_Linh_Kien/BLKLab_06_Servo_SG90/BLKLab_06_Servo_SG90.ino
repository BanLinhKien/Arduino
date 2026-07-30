#include <Servo.h>

const byte PIN_SERVO = 9;
const byte GOC_XA = 50;
const byte GOC_THU = 125;
Servo servoChuyenHuong;

void setup() {
  Serial.begin(9600);
  servoChuyenHuong.attach(PIN_SERVO);
  servoChuyenHuong.write(GOC_XA);
  Serial.println(F("Bat dau tai vi tri XA"));
  delay(1000);
}

void loop() {
  Serial.println(F("SERVO -> THU NUOC"));
  servoChuyenHuong.write(GOC_THU);
  delay(3000);

  Serial.println(F("SERVO -> XA NUOC"));
  servoChuyenHuong.write(GOC_XA);
  delay(3000);
}

