/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 * Code: Điều khiển led RGB WS2812 - Điều khiển Led RGB WS2812
 * Đấu nối: 
  - D6: Chân kết nối với DIN của LED WS2812
  
 * Các hàm sử dụng:   
 * - rainbowCycle: Hàm tạo hiệu ứng chạy qua màu sắc của LED NeoPixel.
 * - uint32_t: Hàm tính toán màu sắc
 */
 
#include <Adafruit_NeoPixel.h>

#define LED_PIN 3                      // Chân kết nối với DIN của LED WS2812
#define NUM_LEDS 12                    // Số lượng LED trong dây NeoPixel
#define BRIGHTNESS 5                   // Điều chỉnh độ sáng (0-255)
#define LED_TYPE NEO_GRB + NEO_KHZ800  // Loại của LED NeoPixel, ở đây là loại RGB với tần số NEO_KHZ800.

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, LED_TYPE);

void setup() {
  strip.begin();
  strip.show();                       // Tắt tất cả các LED khi khởi động
  strip.setBrightness(BRIGHTNESS);    // Thiết lập độ sáng cho dải đèn.
}

void loop() 
{
  // Gọi các hàm điều khiển LED tại đây
  rainbowCycle(20);
}

// Hàm tạo hiệu ứng chạy qua màu sắc của LED NeoPixel.
void rainbowCycle(int wait) 
{
  uint16_t i, j;

  for(j=0; j<256*5; j++) // Chạy qua màu sắc từ đỏ đến tím
  {           
    for(i=0; i< strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();       //Hiển thị trạng thái mới của các LED sau khi đã cập nhật màu sắc.
    delay(wait);        //Đợi một khoảng thời gian (wait) trước khi chuyển sang vòng lặp tiếp theo.
  }
}

//Hàm tính toán màu sắc
uint32_t Wheel(byte WheelPos) 
{
  if(WheelPos < 85) 
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) 
  {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else 
  {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
