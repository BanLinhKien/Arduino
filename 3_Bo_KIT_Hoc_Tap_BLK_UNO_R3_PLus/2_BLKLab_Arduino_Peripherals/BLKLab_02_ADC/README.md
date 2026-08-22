<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/ADC.png" alt="Arduino" />

## Chương trình đầu tiên với ADC

Sau khi tìm hiểu về bộ chuyển đổi tương tự sang số (ADC - Analog to Digital Converter), chúng ta sẽ thực hành chương trình đầu tiên để đọc giá trị điện áp từ một biến trở.

Trong ví dụ này, Arduino UNO R3 sẽ liên tục đọc điện áp tại chân **A0** và hiển thị giá trị ADC lên **Serial Monitor**.

### Mục tiêu

Sau bài thực hành này, bạn sẽ biết cách:

- Đọc tín hiệu Analog bằng ADC.
- Sử dụng hàm `analogRead()`.
- Hiểu mối quan hệ giữa điện áp đầu vào và giá trị ADC.
- Quan sát dữ liệu trên Serial Monitor.

### Linh kiện cần chuẩn bị

| STT | Linh kiện | Số lượng |
|:---:|-----------|:--------:|
| 1 | [KIT Arduino UNO R3 CH340G](https://banlinhkien.com/kit-arduino-uno-r3-ch340g-p6649363.html) | 1 |
| 2 | [Triết Áp Đơn B10K](https://banlinhkien.com/triet-ap-don-b10k-p6648290.html) | 1 |
| 3| [Board Test GL No.12](https://banlinhkien.com/board-test-gl-no.12-p6649617.html) | 1 |
| 4| [Dây Cắm Mạch YC-65](https://banlinhkien.com/bo-day-cam-mach-65-soi-yc65-p12118881.html) | 3 sợi |


### Sơ đồ đấu nối

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/ADC2.png" alt="Arduino" />

### Kết nối

- Chân ngoài thứ nhất → 5V
- Chân ngoài thứ hai → GND
- Chân giữa → A0

---

## Chương trình

```cpp
const int ADC_PIN = A0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int adcValue = analogRead(ADC_PIN);

    Serial.print("ADC Value: ");
    Serial.println(adcValue);

    delay(200);
}
```



## Phân tích chương trình

### 1. Khai báo chân ADC

```cpp
const int ADC_PIN = A0;
```

Arduino sẽ sử dụng chân **A0** để đọc tín hiệu Analog.

### 2. Khởi tạo Serial

```cpp
Serial.begin(9600);
```

Khởi tạo giao tiếp Serial với tốc độ **9600 baud** để hiển thị dữ liệu lên Serial Monitor.


### 3. Đọc giá trị ADC

```cpp
int adcValue = analogRead(A0);
```

Hàm `analogRead()` sẽ chuyển đổi điện áp đầu vào thành giá trị số.

Giá trị nhận được nằm trong khoảng:

| Điện áp | Giá trị ADC |
|---------|-------------:|
| 0V | 0 |
| 2.5V | ≈512 |
| 5V | 1023 |

### 4. Hiển thị kết quả

```cpp
Serial.print("ADC Value: ");
Serial.println(adcValue);
```

Giá trị ADC sẽ được gửi tới Serial Monitor để quan sát.

Ví dụ:

```text
ADC Value: 15
ADC Value: 238
ADC Value: 512
ADC Value: 756
ADC Value: 1023
```

### 5. Chu kỳ đọc

```cpp
delay(200);
```

Arduino sẽ đọc giá trị ADC sau mỗi **200 ms**.

## Kết quả mong đợi

Sau khi nạp chương trình:

- Mở **Serial Monitor**.
- Chọn tốc độ **9600 baud**.
- Xoay biến trở.

Bạn sẽ thấy giá trị ADC thay đổi liên tục từ **0** đến **1023**.


## Kiến thức mở rộng

Arduino UNO R3 sử dụng bộ ADC có độ phân giải **10 bit**.

Điều đó có nghĩa là điện áp đầu vào từ **0V đến 5V** sẽ được chia thành **1024 mức**:

```text
0V                              5V
│────────────────────────────────│
0                              1023
```

Mỗi bước ADC tương ứng khoảng:

```text
5V / 1024 ≈ 4.88 mV
```

Nói cách khác, cứ thay đổi khoảng **4.88 mV**, giá trị ADC sẽ tăng hoặc giảm **1 đơn vị**.

---

## Thử thách

Sau khi hoàn thành bài thực hành, hãy thử:

- Đọc giá trị từ chân **A1** thay vì **A0**.
- Thay đổi tốc độ cập nhật dữ liệu từ **200 ms** xuống **50 ms**.
- Tính điện áp thực tế từ giá trị ADC theo công thức:

```cpp
float voltage = adcValue * 5.0 / 1023.0;
```

Sau đó hiển thị cả giá trị ADC và điện áp:

```text
ADC Value : 512

Voltage   : 2.50 V
```

Đây là bước đầu tiên để làm việc với các cảm biến Analog như cảm biến ánh sáng, cảm biến nhiệt độ, cảm biến độ ẩm đất, cảm biến khí gas và nhiều cảm biến khác sử dụng tín hiệu điện áp.

---
<p align="center"> <img src="https://pos.nvncdn.com/f2fe44-24897/store/20180126_gVLn1I1Irv2dz2XjhYDIshMM.png" alt="Arduino" width="300" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>