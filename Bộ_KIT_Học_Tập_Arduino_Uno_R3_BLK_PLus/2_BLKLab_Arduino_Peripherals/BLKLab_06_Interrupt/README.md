<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/IT.png" alt="Arduino" />

## Chương trình đầu tiên với Interrupt

Sau khi tìm hiểu về ngắt ngoài (External Interrupt), chúng ta sẽ thực hành chương trình đầu tiên để điều khiển đèn LED bằng nút nhấn.

Trong ví dụ này, mỗi lần nhấn nút, Arduino UNO R3 sẽ ngay lập tức phát hiện sự kiện thông qua ngắt ngoài và đổi trạng thái của đèn LED.

### Mục tiêu

Sau bài thực hành này, bạn sẽ biết cách:

- Sử dụng ngắt ngoài (External Interrupt).
- Khai báo hàm phục vụ ngắt (Interrupt Service Routine - ISR).
- Sử dụng hàm `attachInterrupt()`.
- Hiểu được ưu điểm của Interrupt so với việc sử dụng `digitalRead()`.


### Linh kiện cần chuẩn bị

| STT | Linh kiện | Số lượng |
|:---:|-----------|:--------:|
| 1 | [KIT Arduino UNO R3 CH340G](https://banlinhkien.com/kit-arduino-uno-r3-ch340g-p6649363.html) | 1 |
| 2 | [LED 5MM Phủ Đỏ](https://banlinhkien.com/led-5mm-phu-do-10c-p6651528.html) | 1 |
| 3 | [Trở Vạch 1/4W 5% 220R](https://banlinhkien.com/tro-vach-14w-5-220r-50c-p6650237.html) | 1 |
| 4 | [Board Test GL No.12](https://banlinhkien.com/board-test-gl-no.12-p6649617.html) | 1 |
| 5 | [Dây Cắm Mạch YC-65](https://banlinhkien.com/bo-day-cam-mach-65-soi-yc65-p12118881.html) | 4 sợi |
| 6 | [Nút Nhấn 4 Chân 12x12x7.3MM (OMRON B3F) ](https://banlinhkien.com/nut-nhan-4-chan-12x12x7.3mm-omron-b3f-5-chiec-p23584279.html) | 1 |


### Sơ đồ đấu nối

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/IT1.png" alt="Arduino" />

> **Lưu ý:** Sử dụng chế độ `INPUT_PULLUP`, vì vậy nút nhấn chỉ cần nối giữa chân D2 và GND.

---

## Chương trình

```cpp
const int LED_PIN = 8;
const int BUTTON_PIN = 2;

volatile bool ledState = false;

void buttonInterrupt()
{
    ledState = !ledState;
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(BUTTON_PIN),
        buttonInterrupt,
        FALLING
    );
}

void loop()
{
    digitalWrite(LED_PIN, ledState);
}
```

## Phân tích chương trình

### 1. Khai báo chân GPIO

```cpp
const int LED_PIN = 8;
const int BUTTON_PIN = 2;
```

- D8 điều khiển LED.
- D2 nhận tín hiệu từ nút nhấn.

Trên Arduino UNO R3, chân **D2 (INT0)** và **D3 (INT1)** hỗ trợ ngắt ngoài.

### 2. Biến dùng trong ngắt

```cpp
volatile bool ledState = false;
```

Từ khóa `volatile` thông báo cho trình biên dịch rằng giá trị của biến có thể thay đổi bất kỳ lúc nào bởi hàm ngắt (ISR), tránh việc tối ưu hóa làm chương trình hoạt động không đúng.

### 3. Hàm phục vụ ngắt (ISR)

```cpp
void buttonInterrupt()
{
    ledState = !ledState;
}
```

Hàm này sẽ được gọi tự động khi xảy ra ngắt.

Trong ví dụ trên, mỗi lần nhấn nút, trạng thái của biến `ledState` sẽ được đảo ngược:

- `false` → `true`
- `true` → `false`

### 4. Khởi tạo ngắt

```cpp
attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    buttonInterrupt,
    FALLING
);
```

Ý nghĩa các tham số:

- `digitalPinToInterrupt(BUTTON_PIN)`: Chuyển chân D2 thành số hiệu ngắt.
- `buttonInterrupt`: Hàm sẽ được thực hiện khi có ngắt.
- `FALLING`: Kích hoạt ngắt khi tín hiệu chuyển từ HIGH xuống LOW.

Do sử dụng `INPUT_PULLUP`, khi nhấn nút, chân D2 sẽ chuyển từ HIGH xuống LOW.

### 5. Điều khiển LED

```cpp
digitalWrite(LED_PIN, ledState);
```

Trong hàm `loop()`, Arduino chỉ cần cập nhật trạng thái LED theo giá trị của `ledState`.

## Kết quả mong đợi

Sau khi nạp chương trình:

- Ban đầu LED tắt.
- Nhấn nút lần thứ nhất → LED sáng.
- Nhấn nút lần thứ hai → LED tắt.
- Tiếp tục nhấn → LED sẽ đổi trạng thái sau mỗi lần nhấn.

---

## Kiến thức mở rộng

Arduino UNO R3 chỉ hỗ trợ **2 chân ngắt ngoài**:

| Chân | Ngắt |
|:----:|:----:|
| D2 | INT0 |
| D3 | INT1 |

Các chế độ kích hoạt ngắt:

| Chế độ | Ý nghĩa |
|---------|----------|
| LOW | Khi chân ở mức LOW |
| CHANGE | Khi tín hiệu thay đổi HIGH ↔ LOW |
| RISING | Khi tín hiệu chuyển từ LOW lên HIGH |
| FALLING | Khi tín hiệu chuyển từ HIGH xuống LOW |

Trong bài thực hành này sử dụng **FALLING** vì nút nhấn được cấu hình với `INPUT_PULLUP`.

## Lưu ý khi sử dụng Interrupt

- Hàm ISR nên thực hiện thật nhanh.
- Không sử dụng `delay()` trong ISR.
- Không sử dụng `Serial.print()` trong ISR vì có thể làm chương trình hoạt động không ổn định.
- Nếu chia sẻ dữ liệu giữa ISR và `loop()`, nên khai báo biến với từ khóa `volatile`.

## Thử thách

Sau khi hoàn thành bài thực hành, hãy thử:

- Chuyển chân ngắt từ D2 sang D3.
- Thay đổi chế độ kích hoạt từ `FALLING` sang `RISING` hoặc `CHANGE` và quan sát sự khác biệt.
- Thêm một biến đếm số lần nhấn nút:

```cpp
volatile int counter = 0;

void buttonInterrupt()
{
    counter++;
}
```

Sau đó hiển thị số lần nhấn lên Serial Monitor hoặc LCD I2C.

Qua bài thực hành này, bạn đã biết cách sử dụng ngắt ngoài để xử lý các sự kiện ngay khi chúng xảy ra. Interrupt là nền tảng quan trọng trong các ứng dụng yêu cầu phản hồi nhanh như bộ mã hóa quay (Rotary Encoder), cảm biến tốc độ, cảm biến hồng ngoại, bàn phím và các hệ thống điều khiển thời gian thực.

---
<p align="center"> <img src="https://pos.nvncdn.com/f2fe44-24897/store/20180126_gVLn1I1Irv2dz2XjhYDIshMM.png" alt="Arduino" width="300" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>
