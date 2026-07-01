# <img src="https://img.icons8.com/?size=100&id=qJhh65nOkrqR&format=png&color=000000" alt="Arduino" width="40"  /> Nền Tảng Lập Trình Arduino & Lộ Trình Phát Triển Toàn Diện


<p align="center"> <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_1.png" alt="Arduino" width="400" style="display: block; margin: 0 auto; border-radius: 8px;" /></p>

---

## 1. <img src="https://img.icons8.com/?size=100&id=L4pdpzfSacLz&format=png&color=000000" alt="Arduino" width="30"  /> Arduino là gì?

**Arduino** là nền tảng điện tử mã nguồn mở gồm phần cứng và phần mềm, dễ sử dụng và linh hoạt. Các bo mạch Arduino có thể đọc dữ liệu từ môi trường *(ánh sáng, nhiệt độ, độ ẩm, nút nhấn, tin nhắn…)*, sau đó điều khiển các thiết bị như động cơ, đèn LED hoặc gửi thông tin đến nơi khác. Vi điều khiển trên bo mạch được lập trình bằng **ngôn ngữ C/C++** thông qua **Arduino IDE**, biên dịch thành mã máy để thực thi.

> ⏳ **Lịch sử hình thành:** Ra đời năm 2003 tại [Học viện Interaction Design](https://en.wikipedia.org/wiki/Interaction_Design_Institute_Ivrea?utm_source=chatgpt.com), *Ivrea (Italy)*, Arduino ban đầu được tạo ra nhằm giúp sinh viên không chuyên về điện tử có thể nhanh chóng chế tạo sản phẩm với chi phí thấp. Là một dự án mã nguồn mở, Arduino khuyến khích cộng đồng toàn cầu cùng phát triển và đóng góp.

> 🍻 **Tên gọi thú vị:** Tên **Arduino** lấy từ một quán bar ở *Ivrea*, nơi các nhà sáng lập gặp nhau hình thành ý tưởng. Quán được đặt theo tên một vị chỉ huy quân đội, sau trở thành vua Italy giai đoạn 1002–1014.

<p align="center"> <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_2.png" alt="Arduino" width="400" style="display: block; margin: 0 auto; border-radius: 8px;"  /></p>

---

## <img src="https://img.icons8.com/?size=100&id=63231&format=png&color=000000" alt="Arduino" width="30"  /> Tại sao là Arduino?

**Arduino** được ưa chuộng trong nhiều lĩnh vực nhờ sự đơn giản, dễ sử dụng và mã nguồn mở. Nó phù hợp cho cả người mới bắt đầu lẫn lập trình viên giàu kinh nghiệm. Cộng đồng Arduino toàn cầu rất lớn, cung cấp nhiều thư viện và hỗ trợ mạnh mẽ, giúp người dùng dễ dàng tìm giải pháp cho vấn đề gặp phải.

* **Đối với người không chuyên (kiến trúc sư, giáo viên, nghệ sĩ…):** Arduino giúp họ nhanh chóng tạo ra sản phẩm điện tử mà trước đây gần như không thể.
* **Đối với kỹ sư điện tử:** Arduino có thể bị xem là quá đơn giản vì nó che giấu phần phức tạp của lập trình vi điều khiển. Tuy nhiên, để làm chủ Arduino ở mức cao hơn (tùy biến hiệu ứng LED, kết nối cảm biến, truyền nhận dữ liệu…), đòi hỏi kiến thức sâu về vi điều khiển và các giao thức truyền dữ liệu.

🌟 **Ưu điểm lớn nhất:** Cú pháp lệnh đơn giản, thư viện phong phú, cộng đồng lớn và mã nguồn mở. Ngoài ra, Arduino còn cung cấp *Hardware Abstraction Library (HAL)* dành cho những ai muốn nghiên cứu sâu cách thức hoạt động bên trong của nền tảng này.

---

## <img src="https://img.icons8.com/?size=100&id=2tud6xmnCEsU&format=png&color=000000" alt="Arduino" width="30"/> Bảng So Sánh Các Board Arduino Phổ Biến

| **Thông số** | **UNO R3** | **Nano 3.x** | **Nano 2.x** | **Leonardo** | **Mega 2560** | **UNO R4 Minima** | **UNO R4 WiFi** |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Hình ảnh** | <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_8.png" width="120"> | <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_3.jpg" width="120"> | <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_3.jpg" width="120"> | <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_4.jpeg" width="120"> | <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_5.jpg" width="120"> | <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_6.webp" width="120"> | <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_7.webp" width="120"> |
| **Vi điều khiển** | ATmega328P | ATmega328 | ATmega168 | ATmega32u4 | ATmega2560 | Renesas RA4M1 (ARM Cortex-M4) | Renesas RA4M1 (ARM Cortex-M4) |
| **Đồng xử lý** | — | — | — | — | — | Không | ESP32-S3-MINI (WiFi/BT) |
| **Điện áp hoạt động** | 5V | 5V | 5V | 5V | 5V | 5V | 5V |
| **Điện áp đầu vào (khuyến nghị)** | 7–12V | 7–12V | 7–12V | 7–12V | 7–12V | 6–24V | 6–24V |
| **Điện áp đầu vào (giới hạn)** | 6–20V | 6–20V | 6–20V | 6–20V | 6–20V | — | — |
| **Chân Digital I/O** | 14 (6 PWM) | 14 (6 PWM) | 14 (6 PWM) | 20 (7 PWM) | 54 (15 PWM) | 14 (6 PWM) | 14 (6 PWM) |
| **Chân Analog** | 6 | 8 | 8 | 12 | 16 | 6 (14-bit ADC) | 6 (14-bit ADC) |
| **Dòng điện tối đa mỗi chân I/O** | 20 mA | 20 mA | 20 mA | 20 mA | 20 mA | 8 mA | 8 mA |
| **Dòng điện cho chân 3.3V** | 50 mA | 50 mA | 50 mA | 50 mA | 50 mA | — | — |
| **Bộ nhớ Flash** | 32 KB (0.5 KB bootloader) | 32 KB (2 KB bootloader) | 16 KB (2 KB bootloader) | 32 KB (4 KB bootloader) | 256 KB (8 KB bootloader) | 256 KB | 256 KB |
| **SRAM** | 2 KB | 2 KB | 1 KB | 2.5 KB | 8 KB | 32 KB | 32 KB |
| **EEPROM / Data Flash** | 1 KB | 1 KB | 0.5 KB | 1 KB | 4 KB | 8 KB (Data Flash) | 8 KB (Data Flash) |
| **Tần số xung nhịp** | 16 MHz | 16 MHz | 16 MHz | 16 MHz | 16 MHz | 48 MHz | 48 MHz |
| **UART (Serial)** | 1 | 1 | 1 | 1 | 4 | 1 | 1 |
| **Kích thước** | 68.6 × 53.4 mm | 45 × 18 mm | 45 × 18 mm | 68.6 × 53.4 mm | 101.52 × 53.3 mm | 68.6 × 53.4 mm | 68.6 × 53.4 mm |
| **Trọng lượng** | 25 g | 7 g | 7 g | 20 g | 37 g | 25 g | 25 g |
| **Tính năng nổi bật** | Chuẩn học tập phổ biến nhất | Kích thước nhỏ gọn | Phiên bản Nano đời cũ | Hỗ trợ HID (chuột, bàn phím) | 4 UART, nhiều I/O | DAC 12-bit, CAN Bus, OP AMP | WiFi, Bluetooth, LED Matrix 12×8, Qwiic I2C |
---

## 2. 💻 Cài đặt phần mềm Arduino IDE


**Arduino IDE** (Integrated Development Environment) là phần mềm chính thức để lập trình cho bo mạch Arduino. Đây là công cụ miễn phí, hỗ trợ Windows, macOS và Linux.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_8.png" alt="Arduino IDE" width="500" style="display: block; margin: 0 auto;" />

### 2.1. Tải Arduino IDE

Truy cập trang chủ chính thức của Arduino để tải phần mềm:

🔗 **https://www.arduino.cc/en/software**

<img src="https://support.arduino.cc/hc/theming_assets/01KEEJRXNH1WFCAZ8SDRQGEDCW" alt="Trang tải Arduino IDE" width="200" style="display: block; margin: 0 auto;" />

Chọn phiên bản phù hợp với hệ điều hành của bạn:

| Hệ điều hành | Tệp cần tải |
|---|---|
| Windows 10/11 | Arduino IDE 2.x – Windows Installer (.exe) |
| macOS (Intel / Apple Silicon) | Arduino IDE 2.x – macOS (.dmg) |
| Linux (64-bit) | Arduino IDE 2.x – Linux AppImage (.AppImage) |

> 💡 **Khuyến nghị:** Sử dụng **Arduino IDE 2.x** (phiên bản mới nhất) vì có giao diện hiện đại hơn, hỗ trợ autocomplete, Serial Plotter tích hợp và nhiều tính năng hữu ích khác.

---

### 2.2. Cài Đặt Trên Windows

**Bước 1:** Chạy tệp `.exe` vừa tải về. Nếu Windows hỏi "Do you want to allow this app to make changes?", chọn **Yes**.

<img src="https://docs.arduino.cc/static/a07377d2bf60623d012b62af58438774/a6d36/downloading-and-installing-img01.png" alt="Cài đặt Arduino IDE Windows" width="500" style="display: block; margin: 0 auto;" />

**Bước 2:** Cửa sổ cài đặt xuất hiện. Chọn **I Agree** để đồng ý với điều khoản.

**Bước 3:** Chọn thư mục cài đặt (mặc định là `C:\Program Files\Arduino IDE`) → Nhấn **Install**.

<img src="https://docs.arduino.cc/static/fcbf5c44560912d18cc8d99d076bae5d/a6d36/downloading-and-installing-img02.png" alt="Cài đặt Arduino IDE Windows" width="500" style="display: block; margin: 0 auto;" />

**Bước 4:** Quá trình cài đặt diễn ra tự động. Sau khi hoàn tất, nhấn **Finish**. Trình cài đặt có thể yêu cầu cài thêm **USB driver** cho Arduino – nhấn **Install** để cho phép.

---

### 2.3. Cài Đặt Trên macOS

**Bước 1:** Mở tệp `.dmg` vừa tải về.

**Bước 2:** Kéo biểu tượng **Arduino IDE** vào thư mục **Applications**.

<img src="https://docs.arduino.cc/static/5aa93a8c140c7aaadf39ed6e63dc808d/a6d36/downloading-and-installing-img03.png" alt="Cài đặt Arduino IDE Windows" width="500" style="display: block; margin: 0 auto;" />

**Bước 3:** Mở **Finder → Applications → Arduino IDE** để khởi chạy. Nếu macOS cảnh báo "app from unidentified developer", vào **System Preferences → Security & Privacy → Open Anyway**.

---

### 2.4. Khởi Động Arduino IDE Lần Đầu

Sau khi cài đặt xong, mở Arduino IDE. Giao diện chính sẽ như sau:

<img src="https://docs.arduino.cc/static/6510febddcd0559432446f94ba189bfb/a6d36/ide-2-overview.png" alt="Giao diện Arduino IDE 2" width="650" style="display: block; margin: 0 auto;" />

Các thành phần chính của giao diện:

| Khu vực | Chức năng |
|---|---|
| **Toolbar** (thanh công cụ) | Compile, Upload, Serial Monitor, Serial Plotter |
| **Editor** (vùng soạn thảo) | Viết code Arduino (sketch) |
| **Output Panel** (phía dưới) | Hiển thị kết quả biên dịch và thông báo lỗi |
| **Board & Port Selector** | Chọn loại bo mạch và cổng COM kết nối |

---

### 2.5. Cài Đặt Driver USB (CH340/CP2102)

Bo mạch **Arduino UNO R3 clone** (hàng Trung Quốc giá rẻ) thường dùng chip chuyển đổi **CH340** thay vì chip gốc ATMEGA16U2. Máy tính cần cài driver riêng để nhận diện.

<img src="https://components101.com/sites/default/files/components/CH340-IC.jpg" alt="Chip CH340" width="350" style="display: block; margin: 0 auto;" />

**Kiểm tra bo mạch của bạn:** Lật mặt sau bo mạch, tìm chip nhỏ có ký hiệu **CH340** hoặc **CP2102**.

**Tải driver CH340:**
- 🔗 Windows/macOS: https://sparks.gogo.co.nz/ch340.html
- Sau khi cài, cắm Arduino vào máy tính qua cáp USB. Kiểm tra trong **Device Manager** (Windows) hoặc `/dev/tty.wchusbserial*` (macOS) để xác nhận đã nhận cổng.

> <img src="https://img.icons8.com/?size=100&id=pIPl8tqh3igN&format=png&color=000000"  width="20"/> Với **Arduino UNO R3 chính hãng** (Arduino.cc), máy tính sẽ tự nhận diện mà không cần cài driver thêm.

---

## 3. 🔧 Kết Nối Arduino Với Máy Tính Và Cấu Hình IDE

### 3.1. Kết Nối Qua Cáp USB

Dùng cáp **USB Type-A sang USB Type-B** (cáp hình vuông, giống cáp máy in cũ) để kết nối Arduino UNO R3 với máy tính.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_9" alt="Nano" width="600" style="display: block; margin: 0 auto;"  />

Khi cắm đúng:
- **Đèn LED nguồn ON** (màu xanh lá hoặc đỏ) trên bo mạch sẽ sáng lên.
- Máy tính sẽ nhận diện một cổng **COM mới** (Windows) hoặc `/dev/ttyACM0` / `/dev/ttyUSB0` (Linux/macOS).

---

### 3.2. Chọn Bo Mạch Và Cổng Kết Nối

Trong Arduino IDE, thực hiện 2 bước quan trọng trước khi nạp code:

**Bước 1 – Chọn loại bo mạch:**

Vào menu **Tools → Board → Arduino AVR Boards → Arduino Uno**

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_10.png" alt="UNO" width="600" style="display: block; margin: 0 auto;"  />

**Bước 2 – Chọn cổng kết nối:**

Vào menu **Tools → Port** và chọn cổng COM có Arduino (thường là `COM3`, `COM4`, ... trên Windows hoặc `/dev/ttyACM0` trên Linux).

> 💡 **Mẹo:** Cổng đúng thường có chú thích "(Arduino Uno)" ngay bên cạnh.

---

## 4. ⏳ Nạp Chương Trình Đầu Tiên – Blink LED

Sau khi cài đặt xong, chúng ta sẽ chạy thử chương trình đầu tiên kinh điển trong thế giới Arduino: **Blink LED** – nhấp nháy đèn LED trên bo mạch.

### 4.1. Mở Ví Dụ Có Sẵn

Arduino IDE đã đi kèm nhiều ví dụ mẫu. Mở chương trình Blink bằng cách:

**File → Examples → 01.Basics → Blink**

<img src="https://docs.arduino.cc/static/5b1b3c4b0d0dca4e2a0c6f3a67e67a3e/29007/ide-2-blink-example.png" alt="Mở ví dụ Blink" width="500" style="display: block; margin: 0 auto;" />

Cửa sổ soạn thảo sẽ hiển thị code như sau:

```cpp
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
```

---

### 4.2. Giải Thích Code

Một chương trình Arduino (gọi là **Sketch**) luôn có 2 hàm bắt buộc:

**`void setup()`**
- Chạy **một lần duy nhất** khi Arduino khởi động hoặc nhấn nút Reset.
- Dùng để khởi tạo: cấu hình chân GPIO, khởi tạo Serial, kết nối WiFi,...

**`void loop()`**
- Chạy **lặp lại vô tận** sau khi `setup()` kết thúc.
- Đây là nơi đặt logic chính của chương trình.

Giải thích từng dòng trong ví dụ Blink:

| Lệnh | Ý nghĩa |
|---|---|
| `pinMode(LED_BUILTIN, OUTPUT)` | Cấu hình chân LED tích hợp (chân 13) là ngõ ra |
| `digitalWrite(LED_BUILTIN, HIGH)` | Đặt chân 13 = mức cao (5V) → LED sáng |
| `delay(1000)` | Dừng chương trình 1000ms = 1 giây |
| `digitalWrite(LED_BUILTIN, LOW)` | Đặt chân 13 = mức thấp (0V) → LED tắt |

> 📌 **`LED_BUILTIN`** là hằng số được Arduino IDE định nghĩa sẵn, tương ứng với chân số **13** – chân kết nối với LED có ký hiệu **L** trên bo mạch Arduino UNO R3.

---

### 4.3. Biên Dịch (Compile)

Trước khi nạp code vào Arduino, cần biên dịch để kiểm tra lỗi cú pháp.

Nhấn nút **✓ Verify** (hoặc tổ hợp phím **Ctrl+R**):

<img src="https://docs.arduino.cc/static/9c86771f21c63130a363a4575b8ed712/a6d36/uploading-a-sketch-img01.png" alt="Biên dịch chương trình" width="500" style="display: block; margin: 0 auto;" />

Nếu biên dịch thành công, vùng Output phía dưới sẽ hiển thị:

```
Sketch uses 924 bytes (2%) of program storage space. Maximum is 32256 bytes.
Global variables use 9 bytes (0%) of dynamic memory, leaving 2039 bytes for local variables.
```

---

### 4.4. Nạp Code (Upload)

Sau khi biên dịch thành công, nhấn nút **→ Upload** (hoặc tổ hợp phím **Ctrl+U**) để nạp code vào Arduino.

Trong quá trình nạp:
- Hai đèn LED nhỏ **TX** và **RX** trên bo mạch sẽ nháy liên tục – đây là dấu hiệu dữ liệu đang được truyền.
- Thanh tiến trình ở dưới cùng hiển thị phần trăm hoàn thành.

Khi nạp xong, IDE thông báo:

```
Done uploading.
```

---

### 4.5. Quan Sát Kết Quả

Ngay sau khi nạp xong, đèn LED ký hiệu **"L"** trên bo mạch Arduino UNO R3 sẽ bắt đầu **nhấp nháy**: sáng 1 giây, tắt 1 giây, lặp lại liên tục.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_8.png" alt="Arduino Blink LED" width="400" style="display: block; margin: 0 auto;" />

🎉 **Chúc mừng!** Bạn đã nạp thành công chương trình đầu tiên lên Arduino UNO R3!

---

### 4.6. Thử Thay Đổi Thời Gian Nhấp Nháy

Hãy thử thay đổi giá trị trong hàm `delay()` để thấy sự khác biệt:

```cpp
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);   // Sáng nhanh hơn - 0.2 giây
  digitalWrite(LED_BUILTIN, LOW);
  delay(800);   // Tắt lâu hơn - 0.8 giây
}
```

Sau khi sửa, nhấn **Upload** lại và quan sát đèn LED thay đổi theo nhịp mới.

> 🔬 **Thử thách:** Hãy thử đặt `delay(50)` và quan sát hiện tượng gì xảy ra. Mắt người không thể phân biệt được khi LED nháy với tốc độ quá nhanh!

---

## 5. 🛠 Xử Lý Lỗi Thường Gặp

| Thông báo lỗi | Nguyên nhân | Cách khắc phục |
|---|---|---|
| `avrdude: ser_open(): can't open device` | Sai cổng COM hoặc chưa chọn Port | Kiểm tra lại **Tools → Port** |
| `Board at COM3 is not available` | Arduino chưa kết nối hoặc driver chưa cài | Kiểm tra cáp USB và cài driver CH340 |
| `'pinMode' was not declared in this scope` | Sai tên hàm / lỗi cú pháp | Kiểm tra chính tả, phân biệt HOA/thường |
| Đèn TX/RX không nháy khi Upload | Chọn sai loại bo mạch | Chọn lại **Tools → Board → Arduino Uno** |
| Upload thành công nhưng LED không nháy | Code logic sai | Kiểm tra lại tên chân và giá trị `delay` |

---

## Tổng Kết

Qua phần này, bạn đã:

- <img src="https://img.icons8.com/?size=100&id=pIPl8tqh3igN&format=png&color=000000"  width="20"/>  Cài đặt thành công **Arduino IDE 2.x**
- <img src="https://img.icons8.com/?size=100&id=pIPl8tqh3igN&format=png&color=000000"  width="20"/> Kết nối bo mạch **Arduino UNO R3** với máy tính
- <img src="https://img.icons8.com/?size=100&id=pIPl8tqh3igN&format=png&color=000000"  width="20"/> Cấu hình đúng **Board** và **Port** trong IDE
- <img src="https://img.icons8.com/?size=100&id=pIPl8tqh3igN&format=png&color=000000"  width="20"/> Hiểu cấu trúc cơ bản của một **Arduino Sketch** (setup + loop)
- <img src="https://img.icons8.com/?size=100&id=pIPl8tqh3igN&format=png&color=000000"  width="20"/> Biên dịch và nạp chương trình **Blink LED** đầu tiên

Ở các phần tiếp theo, chúng ta sẽ tìm hiểu cách điều khiển LED với nút nhấn, sử dụng PWM để điều chỉnh độ sáng, và đọc tín hiệu analog từ cảm biến.
