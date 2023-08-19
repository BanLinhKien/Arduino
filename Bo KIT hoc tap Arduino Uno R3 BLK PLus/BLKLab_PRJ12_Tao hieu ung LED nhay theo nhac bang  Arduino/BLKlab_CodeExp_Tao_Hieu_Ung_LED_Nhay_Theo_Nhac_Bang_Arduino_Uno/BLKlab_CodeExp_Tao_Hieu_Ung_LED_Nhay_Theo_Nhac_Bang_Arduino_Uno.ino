/*===================================================================================================
 *                                      BANLINHKIEN.COM
 *===================================================================================================
 *                          TẠO HIỆU ỨNG NHÁY THEO NHẠC BẰNG ARDUINO
 * Đấu nối:
 *         Arduino       |        Module cảm biến âm thanh
 *           A0                            OUT
 *____________________________________________________________________________________________________*/
 
const int sensorPin = A0;                                       // Chân analog của cảm biến âm thanh
const int ledCount = 12;                                        // Số lượng LED
int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};       // Chân GPIO cho các LED

void setup() {
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);                                // Đặt các chân LED là OUTPUT
  }
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);                                    // Đặt tín hiệu từ module cảm biến âm thanh là INPUT
}

void loop() {
  int sensorValue = analogRead(sensorPin);                      // Đọc giá trị từ cảm biến âm thanh
  int threshold = 4064;                                         // Ngưỡng âm thanh, bạn có thể điều chỉnh theo yêu cầu
  Serial.println(sensorValue);
  int numLEDs = map(sensorValue, 0, 4064, 0, 12);               // Chuyển đổi giá trị đọc từ cảm biến âm thanh từ khoảng 0-4064 sang khoảng 0-15.

 
  if (sensorValue < threshold) {
    for (int i = 0; i < ledCount; i++) {
    if (i < numLEDs) {
      digitalWrite(ledPins[i], HIGH);                           // Bật lần lượt các LED với số LED được bật đã được tính ở trên
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
  }

  delay(100);                                                   // Đợi một chút trước khi đọc giá trị âm thanh tiếp theo
}
