<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/155c721f-6270-4207-8ac8-56589e044fa7.png" alt="Arduino" />

## Chương trình đầu tiên với GPIO

Sau khi tìm hiểu về GPIO, chúng ta sẽ viết chương trình đầu tiên để điều khiển một chân GPIO trên Arduino UNO R3.

Trong ví dụ này, Arduino sẽ bật và tắt đèn LED liên tục sau mỗi 1 giây. Đây là chương trình cơ bản nhất giúp bạn làm quen với cách cấu hình và điều khiển GPIO.


### Mục tiêu
Sau bài thực hành này, bạn sẽ biết cách:

- Khai báo một chân GPIO ở chế độ OUTPUT.
- Xuất mức logic HIGH và LOW.
- Hiểu được hoạt động của hàm `digitalWrite()`.

### Linh kiện cần chuẩn bị

| STT | Linh kiện | Số lượng |
|:---:|-----------|:--------:|
| 1 | [KIT Arduino UNO R3 CH340G](https://banlinhkien.com/kit-arduino-uno-r3-ch340g-p6649363.html) | 1 |
| 2 | [LED 5MM Phủ Đỏ](https://banlinhkien.com/led-5mm-phu-do-10c-p6651528.html) | 1 |
| 3 | [Trở Vạch 1/4W 5% 220R](https://banlinhkien.com/tro-vach-14w-5-220r-50c-p6650237.html) | 1 |
| 4 | [Board Test GL No.12](https://banlinhkien.com/board-test-gl-no.12-p6649617.html) | 1 |
| 5 | [Dây Cắm Mạch YC-65](https://banlinhkien.com/bo-day-cam-mach-65-soi-yc65-p12118881.html) | 2 sợi |

## Sơ đồ đấu nối

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/GPIO.png" alt="Arduino" />


**Kết nối:**

- Chân D8 → Điện trở 220Ω.
- Điện trở → Chân Anode (+) của LED.
- Chân Cathode (-) của LED → GND.

> **Lưu ý:** Điện trở 220Ω có tác dụng giới hạn dòng điện, giúp bảo vệ LED và chân GPIO.


### Chương trình

```cpp
const int LED_PIN = 8;          // Khai báo chân kết nối LED
void setup()
{
    pinMode(LED_PIN, OUTPUT);   // Cấu hình chân D8 là OUTPUT
}
void loop()
{
    digitalWrite(LED_PIN, HIGH);// Bật LED
    delay(1000);                // Chờ 1 giây
    digitalWrite(LED_PIN, LOW); // Tắt LED
    delay(1000);                // Chờ 1 giây
}
```

## Phân tích chương trình

### 1. Khai báo chân GPIO

```cpp
const int LED_PIN = 8;
```

Biến `LED_PIN` lưu số chân GPIO được sử dụng để điều khiển LED.

Việc sử dụng biến giúp chương trình dễ đọc và thuận tiện khi thay đổi chân kết nối.


### 2. Hàm `setup()`

```cpp
void setup()
{
    pinMode(LED_PIN, OUTPUT);
}
```

Hàm `setup()` chỉ được thực hiện **một lần duy nhất** khi Arduino khởi động hoặc nhấn nút RESET.

Lệnh:

```cpp
pinMode(LED_PIN, OUTPUT);
```

cấu hình chân D8 thành chân xuất tín hiệu (OUTPUT).

### 3. Hàm `loop()`

```cpp
void loop()
{
    ...
}
```

Sau khi `setup()` kết thúc, Arduino sẽ liên tục thực hiện hàm `loop()` từ đầu đến cuối và lặp lại vô hạn.

### 4. Bật LED

```cpp
digitalWrite(LED_PIN, HIGH);
```

GPIO xuất mức điện áp khoảng **5V**, làm cho LED sáng.

### 5. Tạm dừng chương trình

```cpp
delay(1000);
```

Arduino tạm dừng thực thi chương trình trong **1000 mili giây (1 giây)**.

### 6. Tắt LED

```cpp
digitalWrite(LED_PIN, LOW);
```

GPIO xuất mức điện áp **0V**, LED tắt.

### 7. Chu trình lặp

Sau khi kết thúc hàm `loop()`, Arduino sẽ tự động quay lại đầu hàm và tiếp tục thực hiện.

Kết quả là LED sẽ sáng và tắt liên tục với chu kỳ:

- Sáng: 1 giây
- Tắt: 1 giây

## Kết quả mong đợi

Sau khi nạp chương trình thành công:

- LED sẽ sáng trong 1 giây.
- LED sẽ tắt trong 1 giây.
- Quá trình này lặp lại liên tục.

Nếu LED không sáng, hãy kiểm tra:

- LED đã đấu đúng cực (+/-) chưa.
- Có sử dụng điện trở hạn dòng không.
- Đã chọn đúng cổng COM và đúng loại bo mạch Arduino UNO R3 trong Arduino IDE.
- Chương trình đã được nạp thành công hay chưa.

## Thử thách

Sau khi hoàn thành bài thực hành, hãy thử thay đổi chương trình để:

- LED nhấp nháy nhanh hơn (200 ms).
- LED nhấp nháy chậm hơn (2 giây).
- Thay đổi chân điều khiển từ D8 sang D13.
- Điều chỉnh thời gian sáng và thời gian tắt khác nhau.

> **Gợi ý:** Chỉ cần thay đổi giá trị trong hàm `delay()` hoặc thay đổi biến `LED_PIN`.

---
<p align="center"> <img src="https://pos.nvncdn.com/f2fe44-24897/store/20180126_gVLn1I1Irv2dz2XjhYDIshMM.png" alt="Arduino" width="300" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>