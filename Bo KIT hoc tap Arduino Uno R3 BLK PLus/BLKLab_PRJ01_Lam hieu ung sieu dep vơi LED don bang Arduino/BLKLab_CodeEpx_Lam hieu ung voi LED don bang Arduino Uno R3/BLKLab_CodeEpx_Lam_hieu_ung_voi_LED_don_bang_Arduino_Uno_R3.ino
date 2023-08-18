/*
 * =================================================================================================*
 *                                          BANLINHKIEN.COM                                         *
 *==================================================================================================*                        
 *                        Hướng Dẫn Làm Hiệu Ứng Với LED Đơn Bằng Arduino 
 * 
 * Đấu nối: nối lần lượt các chân 2-13 của Arduino với 12 LED đơn
 * D2: kết nối với LED 1                             D8: kết nối với LED 7
 * D3: kết nối với LED 2                             D9: kết nối với LED 8
 * D4: kết nối với LED 3                             D10: kết nối với LED 9
 * D5: kết nối với LED 4                             D11: kết nối với LED 10
 * D6: kết nối với LED 5                             D12: kết nối với LED 11
 * D7: kết nối với LED 6                             D13: kết nối với LED 12
 
 */
 //Khai báo mảng và khởi tạo các chân của Arduino kết nối với 12 LED đơn
 
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

//Tính số phần tử trong mảng ledPins và gán nó cho biến numLeds

const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

// Khai báo các hàm được sử dụng
void allOn();                             //Hàm bật tất cả các LED
void allOff();                            //Hàm tắt tất cả các LED
void blinkAll();                          //Hàm nhấp nháy LED
void runningLight();                      //Hàm tạo hiêu ứng LED sáng đuổi
void chasing();                           //Hàm đuổi chớp tắt LED
void randomBlink();                       //Hàm nhấp nháy một LED bất kì
void knightRider();                       //Hàm tạo hiệu ứng sáng dau sao
void sparkle();                           //Hàm tạo hiệu ứng chớp tắt cho LED
void fadeInOut();                         //Hàm tạo hiệu ứng bật tất cả LED xong mờ dần
void alternatingBlink();                  //Hàm tạo hiệu ứng LED sáng nháy xen kẽ

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);          //Đặt các chân trong mảng ledPins làm chân đầu ra
  }
}

void loop() {
  allOn();                                //Gọi hàm allOn và thực hiện nó
  delay(1000);                            //Tạo trễ 1s
  
  allOff();                               //Gọi hàm allOff và thực hiện nó
  delay(1000);                            //Tạo trễ 1s
  
  blinkAll();                             //Gọi hàm blinkAll và thực hiện nó
  delay(1000);                            //Tạo trễ 1s
  
  runningLight();                         //Gọi hàm runningLingt và thực hiện nó
  delay(2000);                            //Tạo trễ 1s
  
  chasing();                              //Gọi hàm chasing và thực hiện nó
  delay(2000);                            //Tạo trễ 1s
  
  randomBlink();                          //Gọi hàm randomBlink và thực hiện nó
  delay(2000);                            //Tạo trễ 1s
  
  knightRider();                          //Gọi hàm kinghtRider và thực hiện nó
  delay(2000);                            //Tạo trễ 1s
  
  sparkle();                              //Gọi hàm sparkle và thực hiện nó
  delay(2000);                            //Tạo trễ 1s
  
  fadeInOut();                            //Gọi hàm fadeInOut và thực hiện nó
  delay(3000);                            //Tạo trễ 1s
  
  alternatingBlink();                     //Gọi hàm alternatingBlink và thực hiện nó
  delay(2000);                            //Tạo trễ 1s
}

void allOn() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);       //Bật tất cả các LED 
  }
}

void allOff() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void blinkAll() {
  allOn();
  delay(500);
  allOff();
  delay(500);
}

void runningLight() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(200);
    digitalWrite(ledPins[i], LOW);
  }
}

void chasing() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

void randomBlink() {
  for (int i = 0; i < 5; i++) {
    int randomLed = random(numLeds);
    digitalWrite(ledPins[randomLed], HIGH);
    delay(100);
    digitalWrite(ledPins[randomLed], LOW);
  }
}

void knightRider() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
  }
  for (int i = numLeds - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], LOW);
    delay(100);
  }
}

void sparkle() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
  int randomLed = random(numLeds);
  digitalWrite(ledPins[randomLed], HIGH);
  delay(300);
}

void fadeInOut() {
  for (int brightness = 0; brightness <= 255; brightness++) {
    for (int i = 0; i < numLeds; i++) {
      analogWrite(ledPins[i], brightness);
    }
    delay(10);
  }
  for (int brightness = 255; brightness >= 0; brightness--) {
    for (int i = 0; i < numLeds; i++) {
      analogWrite(ledPins[i], brightness);
    }
    delay(10);
  }
}

void alternatingBlink() {
  for (int i = 0; i < numLeds; i += 2) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(500);
  for (int i = 0; i < numLeds; i += 2) {
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = 1; i < numLeds; i += 2) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(500);
  for (int i = 1; i < numLeds; i += 2) {
    digitalWrite(ledPins[i], LOW);
  }
}
