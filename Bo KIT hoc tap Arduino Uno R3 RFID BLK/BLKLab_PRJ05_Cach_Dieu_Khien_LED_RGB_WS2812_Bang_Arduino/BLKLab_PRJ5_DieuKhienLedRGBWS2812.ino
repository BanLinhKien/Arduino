/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 *                    Hướng Dẫn Điều khiển Led RGB WS2812 
 * Đấu nối: 
  - D3: Chân kết nối với DIN của LED WS2812
 */
 
#include <Adafruit_NeoPixel.h>                                 // Gọi thư viện Adafruit NeoPixel để sử dụng

#define LED_PIN 3                                              // Định nghĩa chân kết nối với DIN của LED WS2812
#define NUM_LEDS 12                                            // Số lượng LED trong dây NeoPixel
#define BRIGHTNESS 5                                           // Điều chỉnh độ sáng (0-255)
#define LED_TYPE NEO_GRB + NEO_KHZ800                          // Loại của LED NeoPixel, ở đây là loại RGB với tần số NEO_KHZ800.

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, LED_TYPE);          // Khởi tạo đối tượng dải đèn NeoPixel

void setup() {
  strip.begin();                                               // Khởi động dải đèn
  strip.show();                                                // Tắt tất cả các LED khi khởi động
  strip.setBrightness(BRIGHTNESS);                             // Thiết lập độ sáng cho dải đèn.
}   

void loop() 
{
  // Gọi các hàm điều khiển LED tại đây
  
  rainbowCycle(20);                                             // Gọi hàm tạo hiệu ứng chạy qua màu sắc của LED NeoPixel.
}

// Hàm tạo hiệu ứng chạy qua màu sắc của LED NeoPixel.

void rainbowCycle(int wait) 
{
  uint16_t i, j;

  for(j=0; j<256*5; j++)                                        // Chạy qua màu sắc từ đỏ đến tím
  {           
    for(i=0; i< strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel((i+j) & 255));               // Thiết lập màu sắc cho từng LED
    }
    strip.show();                                               // Hiển thị trạng thái mới của các LED sau khi đã cập nhật màu sắc.
    delay(wait);                                                // Đợi một khoảng thời gian (wait) trước khi chuyển sang vòng lặp tiếp theo.
  }
}

// Hàm tính toán màu sắc

uint32_t Wheel(byte WheelPos) 
{
  if(WheelPos < 85) 
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);    // Màu từ đỏ đến vàng
  } 
  else if(WheelPos < 170) 
  {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);    // Màu từ vàng đến xanh lá cây
  } 
  else 
  {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);    // Màu từ xanh lá cây đến tím
  }
}
