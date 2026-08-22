<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/EEPROM.png" alt="Arduino" />

## Chương trình đầu tiên với EEPROM

Sau khi tìm hiểu về bộ nhớ EEPROM (Electrically Erasable Programmable Read-Only Memory), chúng ta sẽ thực hành chương trình đầu tiên để lưu và đọc dữ liệu trên Arduino UNO R3.

Trong ví dụ này, Arduino sẽ ghi một giá trị vào EEPROM, sau đó đọc lại và hiển thị kết quả trên **Serial Monitor**.


### Mục tiêu

Sau bài thực hành này, bạn sẽ biết cách:

- Sử dụng thư viện `EEPROM.h`.
- Ghi dữ liệu vào EEPROM.
- Đọc dữ liệu từ EEPROM.
- Hiểu được khả năng lưu trữ dữ liệu ngay cả khi mất nguồn.


### Linh kiện cần chuẩn bị

| STT | Linh kiện | Số lượng |
|:---:|-----------|:--------:|
| 1 | KIT Arduino UNO R3 CH340G | 1 |
| 2 | Cáp USB Type-B | 1 |

> **Lưu ý:** EEPROM là bộ nhớ được tích hợp sẵn trong vi điều khiển ATmega328P, vì vậy không cần kết nối thêm linh kiện ngoài.


### Sơ đồ kết nối

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/UART1.png" alt="Arduino" />

## Chương trình

```cpp
#include <EEPROM.h>

const byte EEPROM_ADDRESS = 0;

void setup()
{
    Serial.begin(9600);

    // Ghi giá trị 123 vào EEPROM
    EEPROM.write(EEPROM_ADDRESS, 123);

    // Đọc lại giá trị vừa lưu
    byte value = EEPROM.read(EEPROM_ADDRESS);

    Serial.print("Gia tri trong EEPROM: ");
    Serial.println(value);
}

void loop()
{

}
```
---

## Phân tích chương trình

### 1. Khai báo thư viện

```cpp
#include <EEPROM.h>
```

Thư viện `EEPROM.h` là thư viện chuẩn của Arduino dùng để làm việc với bộ nhớ EEPROM.

### 2. Chọn địa chỉ lưu dữ liệu

```cpp
const byte EEPROM_ADDRESS = 0;
```

EEPROM được chia thành nhiều ô nhớ.

Mỗi ô nhớ có một địa chỉ riêng.

Trong ví dụ này sử dụng địa chỉ:

```text
0
```

### 3. Ghi dữ liệu

```cpp
EEPROM.write(EEPROM_ADDRESS, 123);
```

Lệnh trên sẽ ghi giá trị:

```text
123
```

vào ô nhớ số:

```text
0
```

### 4. Đọc dữ liệu

```cpp
byte value = EEPROM.read(EEPROM_ADDRESS);
```

Arduino đọc dữ liệu tại địa chỉ 0 và lưu vào biến `value`.


### 5. Hiển thị kết quả

```cpp
Serial.print("Gia tri trong EEPROM: ");
Serial.println(value);
```

Mở **Serial Monitor** sẽ thấy:

```text
Gia tri trong EEPROM: 123
```

## Kết quả mong đợi

Sau khi nạp chương trình:

1. Mở **Serial Monitor**.
2. Chọn tốc độ **9600 baud**.

Kết quả hiển thị:

```text
Gia tri trong EEPROM: 123
```

Nếu nhấn nút **RESET** hoặc tắt nguồn rồi cấp nguồn lại, giá trị vẫn được lưu trong EEPROM.

---

## Kiến thức mở rộng

EEPROM là vùng nhớ **không mất dữ liệu khi mất nguồn** (Non-Volatile Memory).

Điều này khác với RAM:

| RAM | EEPROM |
|------|---------|
| Mất dữ liệu khi mất nguồn | Giữ nguyên dữ liệu khi mất nguồn |
| Đọc/Ghi rất nhanh | Đọc nhanh, ghi chậm hơn |
| Ghi không giới hạn trong điều kiện bình thường | Số lần ghi có giới hạn |

Trên Arduino UNO R3:

| Thông số | Giá trị |
|-----------|----------|
| Dung lượng EEPROM | 1024 Byte |
| Địa chỉ | 0 → 1023 |
| Số lần ghi khuyến nghị | Khoảng 100.000 lần mỗi ô nhớ |


## Lưu ý khi sử dụng EEPROM

Không nên ghi dữ liệu liên tục trong vòng lặp:

```cpp
void loop()
{
    EEPROM.write(0, 100);
}
```

Việc ghi liên tục sẽ làm giảm tuổi thọ của EEPROM.

Nếu chỉ muốn ghi khi dữ liệu thay đổi, hãy sử dụng:

```cpp
EEPROM.update(address, value);
```

Hàm `update()` chỉ ghi khi giá trị mới khác với giá trị đang lưu, giúp kéo dài tuổi thọ EEPROM.


## Thử thách

Sau khi hoàn thành bài thực hành, hãy thử:

- Thay đổi địa chỉ lưu từ `0` sang `10`.
- Ghi một giá trị khác, ví dụ `200`.
- Đọc dữ liệu từ nhiều địa chỉ khác nhau.
- Thay `EEPROM.write()` bằng `EEPROM.update()` và quan sát chương trình vẫn hoạt động bình thường.

Ví dụ:

```cpp
EEPROM.update(0, 200);

byte value = EEPROM.read(0);

Serial.println(value);
```

## Ứng dụng của EEPROM

EEPROM thường được sử dụng để lưu các thông tin cần giữ lại sau khi mất nguồn, chẳng hạn như:

- Cấu hình WiFi.
- Giá trị hiệu chuẩn cảm biến.
- Mức độ sáng LED.
- Ngưỡng cảnh báo.
- Thời gian cài đặt.
- Số lần thiết bị hoạt động.
- Các thông số người dùng.

Nhờ khả năng lưu trữ lâu dài, EEPROM là một thành phần rất hữu ích trong các dự án Arduino yêu cầu ghi nhớ cấu hình hoặc trạng thái của hệ thống.

---
<p align="center"> <img src="https://pos.nvncdn.com/f2fe44-24897/store/20180126_gVLn1I1Irv2dz2XjhYDIshMM.png" alt="Arduino" width="300" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>
