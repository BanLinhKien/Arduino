<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/TC.png" alt="Arduino" />

## Chương trình đầu tiên với Timer/Counter

Sau khi tìm hiểu về Timer/Counter, chúng ta sẽ thực hành chương trình đầu tiên để điều khiển đèn LED nhấp nháy theo chu kỳ bằng **Timer1**.

Khác với các ví dụ trước sử dụng hàm `delay()`, trong bài thực hành này Timer1 sẽ tự động tạo ngắt sau mỗi khoảng thời gian xác định, từ đó điều khiển LED nhấp nháy mà không làm dừng chương trình.

### Mục tiêu

Sau bài thực hành này, bạn sẽ biết cách:

- Sử dụng Timer1 trên Arduino UNO R3.
- Tạo ngắt theo chu kỳ.
- Điều khiển LED mà không sử dụng `delay()`.
- Hiểu vai trò của Timer trong các ứng dụng thời gian thực.

### Linh kiện cần chuẩn bị

| STT | Linh kiện | Số lượng |
|:---:|-----------|:--------:|
| 1 | [KIT Arduino UNO R3 CH340G](https://banlinhkien.com/kit-arduino-uno-r3-ch340g-p6649363.html) | 1 |
| 2 | [LED 5MM Phủ Đỏ](https://banlinhkien.com/led-5mm-phu-do-10c-p6651528.html) | 1 |
| 3 | [Trở Vạch 1/4W 5% 220R](https://banlinhkien.com/tro-vach-14w-5-220r-50c-p6650237.html) | 1 |
| 4 | [Board Test GL No.12](https://banlinhkien.com/board-test-gl-no.12-p6649617.html) | 1 |
| 5 | [Dây Cắm Mạch YC-65](https://banlinhkien.com/bo-day-cam-mach-65-soi-yc65-p12118881.html) | 2 sợi |

### Cài đặt thư viện

Chương trình sử dụng thư viện **TimerOne**.

Trong Arduino IDE:

```
Sketch
    → Include Library
        → Manage Libraries...
```

Tìm kiếm:

```
TimerOne
```

Sau đó cài đặt thư viện.


## Sơ đồ đấu nối

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/GPIO.png" alt="Arduino" />


## Chương trình

```cpp
#include <TimerOne.h>

const int LED_PIN = 8;

volatile bool ledState = false;

void timerInterrupt()
{
    ledState = !ledState;

    digitalWrite(LED_PIN, ledState);
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    // Chu kỳ 500000 us = 500 ms
    Timer1.initialize(500000);

    // Gọi hàm timerInterrupt mỗi 500 ms
    Timer1.attachInterrupt(timerInterrupt);
}

void loop()
{

}
```

---

## Phân tích chương trình

### 1. Khai báo thư viện

```cpp
#include <TimerOne.h>
```

Thư viện `TimerOne` giúp cấu hình Timer1 dễ dàng mà không cần thao tác trực tiếp với các thanh ghi của ATmega328P.

### 2. Khởi tạo Timer

```cpp
Timer1.initialize(500000);
```

Giá trị truyền vào có đơn vị là **micro giây (µs)**.

```text
500000 µs = 500 ms
```

Timer1 sẽ tạo một sự kiện sau mỗi **500 ms**.

### 3. Đăng ký hàm ngắt

```cpp
Timer1.attachInterrupt(timerInterrupt);
```

Mỗi khi Timer1 hết thời gian đếm, Arduino sẽ tự động gọi:

```cpp
timerInterrupt();
```

### 4. Hàm ngắt Timer

```cpp
void timerInterrupt()
{
    ledState = !ledState;

    digitalWrite(LED_PIN, ledState);
}
```

Mỗi lần Timer1 phát sinh ngắt:

- LED đang tắt → bật.
- LED đang bật → tắt.

### 5. Hàm loop()

```cpp
void loop()
{

}
```

Điểm đặc biệt là **không cần viết bất kỳ lệnh nào trong `loop()`**.

LED vẫn nhấp nháy nhờ Timer1 hoạt động độc lập.

---

## Kết quả mong đợi

Sau khi nạp chương trình:

- LED sáng trong 500 ms.
- LED tắt trong 500 ms.
- Quá trình lặp lại liên tục.

Bạn sẽ thấy LED nhấp nháy giống như ví dụ sử dụng `delay()`, nhưng chương trình không bị "đứng" trong thời gian chờ.


## So sánh với delay()

| delay() | Timer |
|-----------|--------|
| Chương trình tạm dừng | Chương trình vẫn tiếp tục chạy |
| Không thể xử lý nhiều tác vụ cùng lúc | Có thể xử lý nhiều tác vụ đồng thời |
| Phù hợp chương trình đơn giản | Phù hợp hệ thống thời gian thực |

Ví dụ:

```cpp
delay(1000);
```

CPU sẽ chờ đúng 1 giây.

Trong khi đó:

```cpp
Timer1.initialize(1000000);
```

CPU vẫn tiếp tục thực hiện các công việc khác và chỉ gọi hàm ngắt khi đủ thời gian.


## Kiến thức mở rộng

Arduino UNO R3 sử dụng vi điều khiển **ATmega328P**, tích hợp ba bộ Timer/Counter:

| Timer | Độ rộng | Ứng dụng |
|:------:|:-------:|-----------|
| Timer0 | 8 bit | `delay()`, `millis()`, `micros()` |
| Timer1 | 16 bit | Servo, Timer, Counter |
| Timer2 | 8 bit | PWM, tạo xung |

Trong đó:

- **Timer0** được Arduino IDE sử dụng cho các hàm thời gian.
- **Timer1** thường dùng cho các ứng dụng cần độ chính xác cao.
- **Timer2** thường dùng cho PWM và phát âm thanh.

## Thử thách

Sau khi hoàn thành bài thực hành, hãy thử:

- Thay đổi chu kỳ từ **500 ms** thành **1000 ms**:

```cpp
Timer1.initialize(1000000);
```

- Thử chu kỳ **200 ms**:

```cpp
Timer1.initialize(200000);
```

- Kết nối thêm một LED khác và tạo hiệu ứng nhấp nháy xen kẽ.

Qua bài thực hành này, bạn đã biết cách sử dụng **Timer1** để tạo các sự kiện theo chu kỳ mà không cần dùng `delay()`. Đây là nền tảng quan trọng để xây dựng các ứng dụng như đồng hồ số, bộ đếm thời gian, điều khiển động cơ, tạo xung PWM hoặc các hệ thống thời gian thực trên Arduino UNO R3.

---
<p align="center"> <img src="https://pos.nvncdn.com/f2fe44-24897/store/20180126_gVLn1I1Irv2dz2XjhYDIshMM.png" alt="Arduino" width="300" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>
