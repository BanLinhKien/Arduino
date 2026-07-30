#include <Adafruit_NeoPixel.h>

const byte PIN_NEOPIXEL = 11;
const byte SO_LED = 12;
Adafruit_NeoPixel strip(SO_LED, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void hienThiMau(uint32_t mau) {
  strip.fill(mau, 0, SO_LED);
  strip.show();
}

void setup() {
  strip.begin();
  strip.setBrightness(40); // Gioi han dong dien.
  strip.clear();
  strip.show();
}

void loop() {
  hienThiMau(strip.Color(0, 0, 120));   // Xanh duong: cho mua
  delay(1500);
  hienThiMau(strip.Color(120, 70, 0));  // Vang/cam: dang xa
  delay(1500);
  hienThiMau(strip.Color(0, 120, 0));   // Xanh la: dang thu/tuoi
  delay(1500);
  hienThiMau(strip.Color(120, 0, 0));   // Do: canh bao
  delay(1500);
  hienThiMau(strip.Color(80, 0, 100));  // Tim: manual
  delay(1500);
  strip.clear();
  strip.show();
  delay(1000);
}

