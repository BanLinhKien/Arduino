<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/I2C.png" alt="Arduino" />

## Chương trình đầu tiên với I2C

Sau khi tìm hiểu về giao tiếp I2C (Inter-Integrated Circuit), chúng ta sẽ thực hành chương trình đầu tiên để hiển thị nội dung lên màn hình LCD I2C 16x2.

Trong ví dụ này, Arduino UNO R3 sẽ giao tiếp với màn hình LCD thông qua chỉ **2 dây tín hiệu (SDA và SCL)** và hiển thị dòng chữ **"Hello, World!"** cùng một thông điệp giới thiệu.


### Mục tiêu

Sau bài thực hành này, bạn sẽ biết cách:

- Khởi tạo giao tiếp I2C.
- Sử dụng thư viện điều khiển LCD I2C.
- Hiển thị văn bản lên màn hình LCD.
- Định vị con trỏ bằng hàm `setCursor()`.


### Linh kiện cần chuẩn bị

| STT | Linh kiện | Số lượng |
|:---:|-----------|:--------:|
| 1 | [KIT Arduino UNO R3 CH340G](https://banlinhkien.com/kit-arduino-uno-r3-ch340g-p6649363.html) | 1 |
| 2 | [LCD1602 Xanh Lá 5V](https://banlinhkien.com/lcd1602-xanh-la-5v-p6649628.html) | 1 |
| 3 | [Module Chuyển Đổi I2C cho LCD1602](https://banlinhkien.com/module-chuyen-doi-i2c-cho-lcd1602-p6647652.html) | 1 |
| 4 | [Dây Cắm Mạch YC-65](https://banlinhkien.com/bo-day-cam-mach-65-soi-yc65-p12118881.html) | 4 sợi |

### Sơ đồ đấu nối

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/I2C1.png" alt="Arduino" />

> **Lưu ý:** Trên Arduino UNO R3, chân **A4** là đường dữ liệu **SDA**, còn chân **A5** là đường xung nhịp **SCL**.

---

# Chương trình

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Địa chỉ I2C của LCD (thường là 0x27 hoặc 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    // Khởi tạo LCD
    lcd.init();

    // Bật đèn nền
    lcd.backlight();

    // Hiển thị dòng đầu tiên
    lcd.setCursor(0, 0);
    lcd.print("Hello, World!");

    // Hiển thị dòng thứ hai
    lcd.setCursor(0, 1);
    lcd.print("Arduino UNO");
}

void loop()
{

}
```


## Phân tích chương trình

### 1. Khai báo thư viện

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
```

- `Wire.h` dùng để giao tiếp I2C.
- `LiquidCrystal_I2C.h` hỗ trợ điều khiển màn hình LCD I2C.

### 2. Khởi tạo màn hình

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

Trong đó:

- `0x27` là địa chỉ I2C của LCD.
- `16` là số cột.
- `2` là số hàng.

> Một số module LCD có địa chỉ **0x3F**. Nếu chương trình không hoạt động, hãy kiểm tra địa chỉ I2C của module.

### 3. Khởi tạo LCD

```cpp
lcd.init();
```

Lệnh này khởi tạo màn hình LCD và chuẩn bị cho việc hiển thị dữ liệu.

### 4. Bật đèn nền

```cpp
lcd.backlight();
```

Bật đèn nền để nội dung hiển thị rõ ràng.

### 5. Định vị con trỏ

```cpp
lcd.setCursor(0, 0);
```

Hàm `setCursor(cột, hàng)` dùng để xác định vị trí bắt đầu hiển thị.

Ví dụ:

| Lệnh | Vị trí |
|------|---------|
| `lcd.setCursor(0,0)` | Cột 0, hàng 1 |
| `lcd.setCursor(5,0)` | Cột 5, hàng 1 |
| `lcd.setCursor(0,1)` | Cột 0, hàng 2 |
| `lcd.setCursor(8,1)` | Cột 8, hàng 2 |

### 6. Hiển thị nội dung

```cpp
lcd.print("Hello, World!");
```

Hàm `print()` sẽ hiển thị chuỗi ký tự tại vị trí con trỏ hiện tại.

## Kết quả mong đợi

Sau khi nạp chương trình, màn hình LCD sẽ hiển thị:

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/I2C2.png" alt="Arduino" />

Nếu màn hình không hiển thị:

- Kiểm tra lại dây SDA và SCL.
- Điều chỉnh biến trở trên module LCD để tăng/giảm độ tương phản.
- Kiểm tra địa chỉ I2C của màn hình (0x27 hoặc 0x3F).

---

## Kiến thức mở rộng

Màn hình LCD I2C chỉ sử dụng **4 dây kết nối**:

| Chân LCD | Kết nối Arduino UNO |
|:---------:|:-------------------:|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

So với LCD 16x2 giao tiếp song song cần 6–10 dây tín hiệu, LCD I2C giúp giảm đáng kể số lượng dây nối, thuận tiện khi kết nối nhiều thiết bị ngoại vi trên cùng một bus I2C.

## Thử thách

Sau khi hoàn thành bài thực hành, hãy thử:

- Thay đổi nội dung hiển thị thành tên của bạn.
- Hiển thị hai dòng văn bản khác nhau.
- Di chuyển vị trí hiển thị bằng cách thay đổi tham số của `setCursor()`.
- Hiển thị giá trị của một biến:

```cpp
int counter = 100;

lcd.setCursor(0, 1);
lcd.print("Count: ");
lcd.print(counter);
```

Kết quả:

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/I2C3.png" alt="Arduino" />

Qua bài thực hành này, bạn đã biết cách sử dụng giao tiếp I2C để điều khiển màn hình LCD 16x2. Đây là nền tảng để xây dựng các ứng dụng như hiển thị nhiệt độ, độ ẩm, thời gian thực, trạng thái cảm biến hoặc giao diện người dùng cho các dự án Arduino.

---
<p align="center"> <img src="https://pos.nvncdn.com/f2fe44-24897/store/20180126_gVLn1I1Irv2dz2XjhYDIshMM.png" alt="Arduino" width="300" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>
