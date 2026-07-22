<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/UART.png" alt="Arduino" />

## Chương trình đầu tiên với UART

Sau khi tìm hiểu về giao tiếp UART (Universal Asynchronous Receiver/Transmitter), chúng ta sẽ thực hành chương trình đầu tiên để gửi dữ liệu từ Arduino UNO R3 đến máy tính thông qua cổng USB.

Trong ví dụ này, Arduino sẽ gửi chuỗi ký tự **"Hello UART!"** lên **Serial Monitor** sau mỗi 1 giây.

### Mục tiêu

Sau bài thực hành này, bạn sẽ biết cách:

- Khởi tạo giao tiếp UART.
- Gửi dữ liệu từ Arduino đến máy tính.
- Sử dụng các hàm `Serial.begin()`, `Serial.print()` và `Serial.println()`.
- Quan sát dữ liệu trên Serial Monitor.


### Linh kiện cần chuẩn bị

| STT | Linh kiện | Số lượng |
|:---:|-----------|:--------:|
| 1 | KIT Arduino UNO R3 CH340G | 1 |
| 2 | Cáp USB Type-B | 1 |

> **Lưu ý:** Arduino UNO R3 sử dụng IC chuyển đổi USB-UART (CH340G hoặc ATmega16U2), vì vậy không cần kết nối thêm dây UART ngoài để giao tiếp với máy tính.

---

### Sơ đồ kết nối

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/UART1.png" alt="Arduino" />

---

## Chương trình

```cpp
void setup()
{
    // Khởi tạo UART với tốc độ 9600 baud
    Serial.begin(9600);
}

void loop()
{
    Serial.println("Hello UART!");

    delay(1000);
}
```

## Phân tích chương trình

### 1. Khởi tạo UART

```cpp
Serial.begin(9600);
```

Lệnh này khởi tạo cổng UART với tốc độ truyền dữ liệu là **9600 baud**.

> Hai thiết bị giao tiếp UART phải sử dụng cùng tốc độ baud để dữ liệu được truyền chính xác.

### 2. Gửi dữ liệu

```cpp
Serial.println("Hello UART!");
```

Hàm `Serial.println()` gửi chuỗi ký tự đến máy tính và tự động xuống dòng sau khi gửi.

Nếu không muốn xuống dòng, có thể sử dụng:

```cpp
Serial.print("Hello UART!");
```

### 3. Tạm dừng chương trình

```cpp
delay(1000);
```

Arduino sẽ gửi dữ liệu sau mỗi **1 giây**.

## Kết quả mong đợi

Sau khi nạp chương trình:

1. Mở **Serial Monitor** trong Arduino IDE.
2. Chọn tốc độ **9600 baud**.
3. Quan sát cửa sổ Serial Monitor.

Bạn sẽ thấy:

```text
Hello UART!
Hello UART!
Hello UART!
Hello UART!
...
```

---

## Kiến thức mở rộng

UART là giao thức truyền dữ liệu nối tiếp không đồng bộ.

Trên Arduino UNO R3, cổng UART phần cứng sử dụng hai chân:

| Chân | Chức năng |
|:----:|-----------|
| D0 | RX (Receive) - Nhận dữ liệu |
| D1 | TX (Transmit) - Gửi dữ liệu |

Khi Arduino kết nối với máy tính qua cổng USB, dữ liệu từ D0 và D1 sẽ được IC CH340G chuyển đổi thành giao tiếp USB để Arduino IDE có thể nhận và hiển thị trên Serial Monitor.


## Các tốc độ Baud phổ biến

| Baud Rate | Ứng dụng |
|:---------:|-----------|
| 9600 | Mặc định, dễ sử dụng |
| 19200 | Truyền dữ liệu tốc độ trung bình |
| 38400 | Thiết bị ngoại vi |
| 57600 | Truyền dữ liệu nhanh |
| 115200 | ESP8266, ESP32, GPS, Debug tốc độ cao |


## Thử thách

Sau khi hoàn thành bài thực hành, hãy thử:

- Thay đổi nội dung chuỗi gửi đi:

```cpp
Serial.println("BanLinhKien");
```

- Thay đổi thời gian gửi dữ liệu từ **1000 ms** xuống **500 ms**.

- Gửi thêm giá trị của một biến:

```cpp
int counter = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.print("Counter: ");
    Serial.println(counter);

    counter++;

    delay(1000);
}
```

Kết quả:

```text
Counter: 0
Counter: 1
Counter: 2
Counter: 3
...
```

Qua bài thực hành này, bạn đã biết cách khởi tạo giao tiếp UART và gửi dữ liệu từ Arduino UNO R3 đến máy tính. Đây là nền tảng để thực hiện các bài học tiếp theo như **nhận dữ liệu từ Serial Monitor**, **giao tiếp giữa hai Arduino**, hoặc kết nối với các module như Bluetooth HC-05, GPS và ESP8266.

---
<p align="center"> <img src="https://pos.nvncdn.com/f2fe44-24897/store/20180126_gVLn1I1Irv2dz2XjhYDIshMM.png" alt="Arduino" width="300" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>
