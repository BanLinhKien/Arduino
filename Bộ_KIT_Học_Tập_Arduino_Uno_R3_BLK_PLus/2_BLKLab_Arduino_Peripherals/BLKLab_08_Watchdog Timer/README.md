<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/WT.png" alt="Arduino" />

## Chương trình đầu tiên với Watchdog Timer

Sau khi tìm hiểu về Watchdog Timer (WDT), chúng ta sẽ thực hành chương trình đầu tiên để bảo vệ hệ thống khi chương trình gặp sự cố.

Trong ví dụ này, Arduino UNO R3 sẽ sử dụng Watchdog Timer với thời gian chờ **2 giây**. Nếu chương trình không làm mới (Reset) Watchdog trong khoảng thời gian này, vi điều khiển sẽ tự động khởi động lại.

### Mục tiêu

Sau bài thực hành này, bạn sẽ biết cách:

- Khởi tạo Watchdog Timer.
- Làm mới (Reset) Watchdog Timer.
- Quan sát hiện tượng Arduino tự động Reset.
- Hiểu được vai trò của Watchdog trong các hệ thống nhúng.

### Linh kiện cần chuẩn bị

| STT | Linh kiện | Số lượng |
|:---:|-----------|:--------:|
| 1 | KIT Arduino UNO R3 CH340G | 1 |
| 2 | Cáp USB Type-B | 1 |

> **Lưu ý:** Bài thực hành này không cần kết nối thêm linh kiện ngoài. Kết quả sẽ được quan sát trên **Serial Monitor**.

### Sơ đồ kết nối

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/UART1.png" alt="Arduino" />

## Chương trình

```cpp
#include <avr/wdt.h>

void setup()
{
    Serial.begin(9600);

    Serial.println("Arduino Started");

    // Khởi tạo Watchdog với thời gian 2 giây
    wdt_enable(WDTO_2S);
}

void loop()
{
    Serial.println("System Running...");

    // Làm mới Watchdog
    wdt_reset();

    delay(1000);
}
```

## Phân tích chương trình

### 1. Khai báo thư viện

```cpp
#include <avr/wdt.h>
```

Thư viện `avr/wdt.h` cung cấp các hàm điều khiển Watchdog Timer trên ATmega328P.

### 2. Khởi tạo Watchdog

```cpp
wdt_enable(WDTO_2S);
```

Lệnh này kích hoạt Watchdog Timer với thời gian chờ **2 giây**.

Nếu quá 2 giây mà chương trình không gọi:

```cpp
wdt_reset();
```

Arduino sẽ tự động Reset.

### 3. Làm mới Watchdog

```cpp
wdt_reset();
```

Lệnh này đặt lại bộ đếm của Watchdog.

Nếu chương trình vẫn hoạt động bình thường và liên tục gọi `wdt_reset()`, Watchdog sẽ không Reset hệ thống.

### 4. Quan sát kết quả

Mở **Serial Monitor** với tốc độ **9600 baud**, bạn sẽ thấy:

```text
Arduino Started
System Running...
System Running...
System Running...
...
```

Do Watchdog luôn được làm mới sau mỗi giây nên Arduino sẽ hoạt động liên tục.

## Thử nghiệm Reset tự động

Để quan sát tác dụng của Watchdog, hãy sửa chương trình như sau:

```cpp
#include <avr/wdt.h>

void setup()
{
    Serial.begin(9600);

    Serial.println("Arduino Started");

    wdt_enable(WDTO_2S);
}

void loop()
{
    Serial.println("Waiting...");

    // Không làm mới Watchdog
    delay(3000);
}
```

---

## Kết quả mong đợi

Watchdog được đặt thời gian chờ **2 giây**, nhưng chương trình lại dừng **3 giây**.

Do không gọi:

```cpp
wdt_reset();
```

Arduino sẽ tự động khởi động lại.

Serial Monitor sẽ hiển thị:

```text
Arduino Started
Waiting...

Arduino Started
Waiting...

Arduino Started
Waiting...
```

Chu trình này sẽ lặp lại liên tục.

---

## Kiến thức mở rộng

Watchdog Timer là một bộ định thời độc lập hoạt động bên trong ATmega328P.

Ngay cả khi chương trình chính bị treo hoặc rơi vào vòng lặp vô hạn, Watchdog vẫn tiếp tục đếm thời gian.

Nếu hết thời gian mà không nhận được lệnh `wdt_reset()`, Watchdog sẽ phát sinh tín hiệu Reset để khởi động lại vi điều khiển.

## Các khoảng thời gian của Watchdog

| Hằng số | Thời gian |
|:--------:|:---------:|
| `WDTO_15MS` | 15 ms |
| `WDTO_30MS` | 30 ms |
| `WDTO_60MS` | 60 ms |
| `WDTO_120MS` | 120 ms |
| `WDTO_250MS` | 250 ms |
| `WDTO_500MS` | 500 ms |
| `WDTO_1S` | 1 giây |
| `WDTO_2S` | 2 giây |
| `WDTO_4S` | 4 giây |
| `WDTO_8S` | 8 giây |

## Ứng dụng của Watchdog Timer

Watchdog Timer thường được sử dụng trong:

- Thiết bị IoT hoạt động liên tục.
- Bộ điều khiển công nghiệp.
- Robot tự động.
- Thiết bị giám sát từ xa.
- Hệ thống điều khiển thông minh.
- Máy bán hàng tự động.
- Thiết bị đo lường.

Watchdog giúp hệ thống tự động phục hồi khi gặp lỗi mà không cần người dùng nhấn nút Reset.


## Thử thách

Sau khi hoàn thành bài thực hành, hãy thử:

- Thay đổi thời gian Watchdog từ **2 giây** sang **4 giây**:

```cpp
wdt_enable(WDTO_4S);
```

- Thử các mức thời gian khác như **500 ms**, **1 giây** hoặc **8 giây**.
- Thêm một bộ đếm để theo dõi số lần Arduino khởi động lại (có thể sử dụng EEPROM để lưu giá trị qua mỗi lần Reset).

> **Lưu ý:** Khi sử dụng Watchdog trong các dự án thực tế, hãy đảm bảo chương trình luôn gọi `wdt_reset()` định kỳ. Nếu quên hoặc gọi quá muộn, hệ thống sẽ bị Reset ngoài ý muốn.

---
<p align="center"> <img src="https://pos.nvncdn.com/f2fe44-24897/store/20180126_gVLn1I1Irv2dz2XjhYDIshMM.png" alt="Arduino" width="300" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>
