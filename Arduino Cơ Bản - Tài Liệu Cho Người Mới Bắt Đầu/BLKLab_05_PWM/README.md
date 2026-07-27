<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/PWM1.png" alt="Arduino" />

## Chương trình đầu tiên với PWM

Sau khi tìm hiểu về PWM (Pulse Width Modulation), chúng ta sẽ thực hành chương trình đầu tiên để điều khiển độ sáng của đèn LED.

Trong ví dụ này, Arduino UNO R3 sẽ thay đổi độ rộng xung PWM để làm LED sáng dần từ tối đến sáng nhất, sau đó giảm dần về tối và lặp lại liên tục.


### Mục tiêu

Sau bài thực hành này, bạn sẽ biết cách:

- Sử dụng hàm `analogWrite()`.
- Điều khiển tín hiệu PWM trên Arduino UNO R3.
- Thay đổi độ sáng của LED bằng PWM.
- Hiểu ý nghĩa của giá trị Duty Cycle.

### Linh kiện cần chuẩn bị

| STT | Linh kiện | Số lượng |
|:---:|-----------|:--------:|
| 1 | [KIT Arduino UNO R3 CH340G](https://banlinhkien.com/kit-arduino-uno-r3-ch340g-p6649363.html) | 1 |
| 2 | [LED 5MM Phủ Đỏ](https://banlinhkien.com/led-5mm-phu-do-10c-p6651528.html) | 1 |
| 3 | [Trở Vạch 1/4W 5% 220R](https://banlinhkien.com/tro-vach-14w-5-220r-50c-p6650237.html) | 1 |
| 4 | [Board Test GL No.12](https://banlinhkien.com/board-test-gl-no.12-p6649617.html) | 1 |
| 5 | [Dây Cắm Mạch YC-65](https://banlinhkien.com/bo-day-cam-mach-65-soi-yc65-p12118881.html) | 2 sợi |


### Sơ đồ đấu nối

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/PWM2.png" alt="Arduino" />

### Kết nối

- Chân D9 → Điện trở 220Ω.
- Điện trở → Chân Anode (+) của LED.
- Chân Cathode (-) → GND.

> **Lưu ý:** Chân D9 là một trong các chân hỗ trợ PWM trên Arduino UNO R3. Bạn cũng có thể sử dụng các chân D3, D5, D6, D10 hoặc D11.

---

## Chương trình

```cpp
const int LED_PIN = 9;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    // Tăng dần độ sáng
    for (int brightness = 0; brightness <= 255; brightness++)
    {
        analogWrite(LED_PIN, brightness);
        delay(10);
    }

    // Giảm dần độ sáng
    for (int brightness = 255; brightness >= 0; brightness--)
    {
        analogWrite(LED_PIN, brightness);
        delay(10);
    }
}
```

## Phân tích chương trình

### 1. Khai báo chân PWM

```cpp
const int LED_PIN = 9;
```

LED được kết nối với chân **D9**, đây là chân hỗ trợ xuất tín hiệu PWM.


### 2. Cấu hình chân OUTPUT

```cpp
pinMode(LED_PIN, OUTPUT);
```

Thiết lập chân D9 ở chế độ OUTPUT để có thể xuất tín hiệu PWM.

### 3. Xuất tín hiệu PWM

```cpp
analogWrite(LED_PIN, brightness);
```

Hàm `analogWrite()` tạo tín hiệu PWM với độ rộng xung tương ứng giá trị `brightness`.

Giá trị hợp lệ từ:

```text
0 → 255
```

Trong đó:

| Giá trị | Duty Cycle | Độ sáng LED |
|:---------:|:-----------:|:-------------:|
| 0 | 0% | Tắt |
| 64 | 25% | Mờ |
| 128 | 50% | Trung bình |
| 192 | 75% | Sáng |
| 255 | 100% | Sáng tối đa |


### 4. Hiệu ứng tăng độ sáng

```cpp
for (int brightness = 0; brightness <= 255; brightness++)
```

Giá trị PWM tăng từ **0** đến **255**, khiến LED sáng dần.


### 5. Hiệu ứng giảm độ sáng

```cpp
for (int brightness = 255; brightness >= 0; brightness--)
```

Giá trị PWM giảm từ **255** về **0**, khiến LED tối dần.


### 6. Tạm dừng chương trình

```cpp
delay(10);
```

Mỗi lần thay đổi độ sáng, Arduino chờ **10 ms** để mắt người có thể quan sát hiệu ứng chuyển đổi mượt mà.

## Kết quả mong đợi

Sau khi nạp chương trình:

- LED sẽ sáng dần từ tối đến sáng nhất.
- Sau đó LED sẽ tối dần về mức tắt.
- Quá trình này sẽ lặp lại liên tục.

Hiệu ứng giống như đèn "thở" (Breathing LED), thường được sử dụng trên các thiết bị điện tử hiện đại.

---

## Kiến thức mở rộng

Trên Arduino UNO R3, hàm `analogWrite()` sử dụng bộ PWM có độ phân giải **8 bit**, vì vậy giá trị điều khiển nằm trong khoảng:

```text
0 ───────────────────────────► 255
```

Trong đó:

- **0** tương ứng Duty Cycle **0%** (LED tắt).
- **255** tương ứng Duty Cycle **100%** (LED sáng tối đa).

Các chân hỗ trợ PWM trên Arduino UNO R3 được ký hiệu bằng dấu **~** trên bo mạch:

| Chân PWM | Tần số mặc định |
|:---------:|:---------------:|
| D3 | ~490 Hz |
| D5 | ~980 Hz |
| D6 | ~980 Hz |
| D9 | ~490 Hz |
| D10 | ~490 Hz |
| D11 | ~490 Hz |


## Thử thách

Sau khi hoàn thành bài thực hành, hãy thử các bài tập sau:

- Thay đổi chân PWM từ **D9** sang **D5** hoặc **D10**.
- Điều chỉnh tốc độ hiệu ứng bằng cách thay đổi thời gian trong `delay()`.
- Chỉ cho LED sáng ở mức **50%** bằng lệnh:

```cpp
analogWrite(LED_PIN, 128);
```

- Thử các giá trị PWM khác nhau như:

```cpp
analogWrite(LED_PIN, 30);
analogWrite(LED_PIN, 80);
analogWrite(LED_PIN, 180);
analogWrite(LED_PIN, 255);
```

Quan sát và so sánh sự thay đổi độ sáng của LED.

> **Gợi ý:** Trong chương tiếp theo, bạn sẽ kết hợp **ADC và PWM** để điều khiển độ sáng LED bằng một biến trở. Đây là ứng dụng rất phổ biến trong các hệ thống điều khiển ánh sáng, quạt và động cơ.


---
<p align="center"> <img src="https://pos.nvncdn.com/f2fe44-24897/store/20180126_gVLn1I1Irv2dz2XjhYDIshMM.png" alt="Arduino" width="300" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>
