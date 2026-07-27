#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println(F("Khong tim thay DS3231"));
    while (1) {}
  }

  if (rtc.lostPower()) {
    Serial.println(F("RTC mat nguon, dat gio theo luc bien dich"));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Chi bo comment dong duoi de dat lai gio, sau do comment lai va nap lan nua.
  // rtc.adjust(DateTime(2026, 7, 17, 14, 30, 0));
}

void loop() {
  DateTime now = rtc.now();
  char text[20];
  snprintf(text, sizeof(text), "%02d/%02d/%04d %02d:%02d:%02d",
           now.day(), now.month(), now.year(),
           now.hour(), now.minute(), now.second());
  Serial.println(text);
  delay(1000);
}

