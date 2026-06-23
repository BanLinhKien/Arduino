# 🚀 Bắt Đầu Với Arduino UNO R3

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_8.png" alt="Arduino" width="500" style="display: block; margin: 0 auto; border-radius: 8px;" />

Bất kỳ một chương trình học nào cũng cần nên bắt đầu một cách từ từ. Bởi vì thời điểm này chúng ta đều mới bắt đầu, nhiều khái niệm, kiến thức về lĩnh vực này gần như không có nhiều. Ở bài này giúp các bạn có thể nắm được các kiến thức cơ bản về điện tử, làm sao để biên dịch, nạp được chương trình trong **Arduino**, cũng như nắm được một số kiến thức về kiến trúc chương trình của **Arduino**. 

**🎯 Nội dung sẽ tìm hiểu ở phần này như sau:**
- Giới thiệu các khái niệm, linh kiện điện tử cơ bản.
- Cài đặt Arduino IDE và nạp chương trình.
- Blink LED (Chớp tắt đèn LED).

---

## 1. ⚡ Giới thiệu một số khái niệm và linh kiện điện tử cơ bản

Trong phần này chúng ta sẽ tìm hiểu về 1 số khái niệm cũng như 1 số linh kiện điện tử cơ bản. Các kiến thức này cũng đã được trình bày chi tiết ở chương trình vật lý bậc phổ thông. Mỗi khái niệm hay linh kiện sẽ có những video giúp chúng ta dễ hiểu hơn những khái niệm cũng như cách hoạt động của các linh kiện điện tử thông dụng.

### 🔴 Điện áp, dòng điện và điện trở

#### 1.1 Điện áp (Voltage)
**Điện áp** hay còn gọi là hiệu điện thế (tiếng Anh: *voltage*) là sự chênh lệch về điện áp giữa 2 điểm, nó là công thực hiện được để di chuyển một hạt điện tích trong trường tĩnh điện từ điểm này đến điểm kia. Hiệu điện thế có thể đại diện cho nguồn năng lượng (lực điện), hoặc sự mất đi, sử dụng, hoặc năng lượng lưu trữ (giảm thế).

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_1.png" alt="Điện áp" width="400" style="display: block; margin: 10px auto; border-radius: 8px;" />

#### 1.2 Dòng điện (Current)
**Dòng điện** (tiếng Anh: *electric current*) là dòng chuyển dịch có hướng của các hạt mang điện. Trong mạch điện, các hạt mang điện phần lớn là các electron chuyển động bên trong dây dẫn. Kim loại là chất dẫn điện phổ biến nhất, kim loại có hạt nhân mang điện tích dương không thể di chuyển, chỉ có các electron tích điện âm có khả năng di chuyển tự do trong vùng dẫn, do đó, trong kim loại các electron là các hạt mang điện.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_2.png" alt="Dòng điện" width="400" style="display: block; margin: 10px auto; border-radius: 8px;" />

> 💡 **Quy ước:** Chiều dòng điện được quy ước là chiều đi từ **cực dương** qua dây dẫn và các thiết bị điện đến **cực âm** của nguồn. 

Sự chuyển dịch có hướng của các điện tích sinh ra do tác động của điện trường gây ra bởi hiệu điện thế. Do đó, có thể hiểu là điện áp sinh ra dòng điện trong một mạch điện, hay nói đơn giản là *"điện áp có trước dòng điện"* trong mạch điện.

**🎬 Video tham khảo:**
- What is Voltage: [www.youtube.com/watch?v=V1ulri4s_E8](https://www.youtube.com/watch?v=V1ulri4s_E8)
- Electricity and Circuits: [www.youtube.com/watch?v=D2monVkCkX](https://www.youtube.com/watch?v=D2monVkCkX)

#### 1.3 Điện trở (Resistance)
**Điện trở** (tiếng Anh: *electric resistance*) là một đại lượng đặc trưng cho khả năng cản trở dòng điện của một vật. Đơn vị của điện trở là **Ohm (Ω)**. Khái niệm điện trở của vật xuất phát từ định luật Ohm. 

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_3.png" alt="Điện trở" width="300" style="display: block; margin: 10px auto; border-radius: 8px;" />

Điện trở gồm 2 tiếp điểm kết nối, thường được dùng để hạn chế cường độ dòng điện chạy trong mạch, điều chỉnh mức độ tín hiệu, dùng để chia điện áp, kích hoạt các linh kiện điện tử chủ động như transistor và có trong rất nhiều ứng dụng khác.

#### 1.4 Định luật Ohm (Ohm’s law)
**Phát biểu định luật:** Cường độ dòng điện chạy qua dây dẫn tỉ lệ thuận với hiệu điện thế đặt vào 2 đầu dây dẫn và tỉ lệ nghịch với điện trở của dây dẫn.

**Công thức:** > ## $$I = \frac{V}{R}$$

Trong đó:
- **V:** Điện áp (Voltage) - Đơn vị: Volts (V)
- **I:** Dòng điện (Current) - Đơn vị: Amperes (A)
- **R:** Điện trở (Resistor) - Đơn vị: Ohms (Ω)

### 🔵 Tụ điện (Capacitor)
**Khái niệm:** **Tụ điện** là linh kiện điện tử gồm 2 vật dẫn đặt gần nhau. Mỗi vật dẫn đó gọi là một bản của tụ điện. Khoảng không gian giữa hai bản có thể là chân không hay bị chiếm bởi một chất điện môi nào đó.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_4.png" alt="Tụ điện" width="300" style="display: block; margin: 10px auto; border-radius: 8px;" />

**Các thông số đặc trưng:**
- **Điện dung:** Đại diện cho khả năng tích điện của tụ. Đơn vị là Fara (F).
- **Điện áp làm việc:** Giá trị điện áp cao nhất mà tụ có thể chịu được. Nếu vượt quá, lớp điện môi sẽ bị đánh thủng gây nổ tụ.
- **Nhiệt độ làm việc:** Nhiệt độ tại vị trí đặt tụ điện không được vượt quá thông số này để đảm bảo an toàn.

🎬 **Video tham khảo:** [www.youtube.com/watch?v=5hFC9ugTGLs](https://www.youtube.com/watch?v=5hFC9ugTGLs)

### 🟢 Cuộn cảm (Inductor)
**Khái niệm:** **Cuộn cảm** (tiếng Anh là *coil* hay *inductor*) là cuộn dây bao gồm nhiều vòng dây dẫn điện quấn quanh một lõi vật liệu từ. Dùng để làm phần ứng trong máy phát điện, lọc nhiễu, tạo nam châm điện...

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/3_5.webp" alt="Cuộn cảm" width="350" style="display: block; margin: 10px auto; border-radius: 8px;" />

Mỗi cuộn cảm có một độ tự cảm, kí hiệu là **L**, đo bằng đơn vị Henry (**H**) đặc trưng cho khả năng sinh suất điện động cảm ứng và tích lũy năng lượng điện từ.

🎬 **Video tham khảo:** [www.youtube.com/watch?v=NgwXkUt3XxQ](https://www.youtube.com/watch?v=NgwXkUt3XxQ)

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

> ✅ Với **Arduino UNO R3 chính hãng** (Arduino.cc), máy tính sẽ tự nhận diện mà không cần cài driver thêm.

---

## 3. 🔧 Kết Nối Arduino Với Máy Tính Và Cấu Hình IDE

### 3.1. Kết Nối Qua Cáp USB

Dùng cáp **USB Type-A sang USB Type-B** (cáp hình vuông, giống cáp máy in cũ) để kết nối Arduino UNO R3 với máy tính.

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/8/8b/USB_AB_DDiff.jpg/640px-USB_AB_DDiff.jpg" alt="Cáp USB Type-B" width="400" style="display: block; margin: 0 auto;" />

Khi cắm đúng:
- **Đèn LED nguồn ON** (màu xanh lá hoặc đỏ) trên bo mạch sẽ sáng lên.
- Máy tính sẽ nhận diện một cổng **COM mới** (Windows) hoặc `/dev/ttyACM0` / `/dev/ttyUSB0` (Linux/macOS).

---

### 3.2. Chọn Bo Mạch Và Cổng Kết Nối

Trong Arduino IDE, thực hiện 2 bước quan trọng trước khi nạp code:

**Bước 1 – Chọn loại bo mạch:**

Vào menu **Tools → Board → Arduino AVR Boards → Arduino Uno**

<img src="https://docs.arduino.cc/static/e3c05cf1cfa7fbc7f6cc38cac4e9b0d2/29007/ide-2-select-board.png" alt="Chọn bo mạch Arduino" width="500" style="display: block; margin: 0 auto;" />

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

Sau khi biên dịch thành công, nhấn nút **→ Upload** (hoặc tổ hợp phím **Ctrl+U**) để nạp code vào Arduino:

<img src="https://docs.arduino.cc/static/9c86771f21c63130a363a4575b8ed712/a6d36/uploading-a-sketch-img01.png" alt="Nạp code vào Arduino" width="500" style="display: block; margin: 0 auto;" />

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

- ✅ Cài đặt thành công **Arduino IDE 2.x**
- ✅ Kết nối bo mạch **Arduino UNO R3** với máy tính
- ✅ Cấu hình đúng **Board** và **Port** trong IDE
- ✅ Hiểu cấu trúc cơ bản của một **Arduino Sketch** (setup + loop)
- ✅ Biên dịch và nạp chương trình **Blink LED** đầu tiên

Ở các phần tiếp theo, chúng ta sẽ tìm hiểu cách điều khiển LED với nút nhấn, sử dụng PWM để điều chỉnh độ sáng, và đọc tín hiệu analog từ cảm biến.
