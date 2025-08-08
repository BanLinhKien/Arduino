# Cài Đặt Arduino IDE

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_1.jpg" alt="Nano" width="800" style="display: block; margin: 0 auto;"  />

## Khái niệm

**Arduino IDE** là chữ viết tắt của Arduino Integrated Development Environment, một công cụ lập trình với các board mạch Arduino. Nó bao gồm các phần chính là: 
- Editor: trình soạn thảo văn bản, dùng để viết code.
- Debugger: công cụ giúp tìm kiếm và sửa lỗi phát sinh khi build chương trình.
- Compiler hoặc interpreter: công cụ giúp biên dịch code thành ngôn ngữ mà vi điều khiển có thể hiểu và thực thi code theo yêu cầu của người dùng.

Hiểu một cách đơn giản, **Arduino IDE** là 1 phần mềm giúp chúng ta nạp code đã viết vào board mạch và thực thi ứng dụng. 

## Cài đặt

### Bước 1: Tải về Arduino IDE

- Truy cập trang chủ Arduino: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
- Chọn phiên bản phù hợp với hệ điều hành của bạn (Windows, macOS, Linux).

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_2.png" alt="Nano" width="800" style="display: block; margin: 0 auto;"  />

- Nhấn nút **Download** để tải về.

### Bước 2: Cài đặt Arduino IDE trên Windows

1. Mở file vừa tải về (thường có đuôi `.exe`).

    <span style="color: blue;">*arduino-ide_2.3.6_Windows_64bit.exe*<span>

2. Nhấn **I Agree** để tiếp tục.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_3.png" alt="Nano" width="800" style="display: block; margin: 0 auto;"  />

3. Chọn thư mục cài đặt (có thể để mặc định).

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_5.png" alt="Nano" width="800" style="display: block; margin: 0 auto;"  />

4. Nhấn **Install** để bắt đầu cài đặt.

5. Đợi quá trình cài đặt hoàn tất, nhấn **Finish**.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_6.png" alt="Nano" width="800" style="display: block; margin: 0 auto;"  />

### Bước 3: Khởi động Arduino IDE

- Tìm biểu tượng Arduino IDE trên màn hình hoặc trong menu Start.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_7.png" alt="Nano" width="800" style="display: block; margin: 0 auto;"  />

- Nhấn đúp để mở phần mềm.

## Sử dụng Arduino IDE với board Arduino UNO R3

Sau khi khởi động Arduino IDE, phần mềm sẽ có giao diện giống bên dưới:

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_8.png" alt="Nano" width="800" style="display: block; margin: 0 auto;"  />

**1. Thanh menu (Menu Bar)**

- Chứa các mục: **File**, **Edit**, **Sketch**, **Tools**, **Help**.

- Dùng để tạo, mở, lưu dự án; chỉnh sửa code; cấu hình board, cổng **COM**; truy cập thư viện, ví dụ mẫu; và trợ giúp.

**2. Thanh công cụ (Toolbar)**

Gồm các nút chính:

- **Verify** (✓): Kiểm tra lỗi cú pháp code.

- **Upload** (→): Biên dịch và nạp code vào Arduino.

- **Select Board/Port**: Chọn loại board Arduino và cổng COM đang kết nối.

**3. Thanh công cụ bên trái (Sidebar)**

Cung cấp các nút truy cập nhanh:

- Quản lý file sketch

- Mở Serial Monitor

- Mở Serial Plotter

- Truy cập thư viện

- Tìm kiếm code

**4. Khu vực soạn thảo mã lệnh (Code Editor)**

- Nơi viết và chỉnh sửa chương trình Arduino.

- Chứa các hàm cơ bản:

    - `setup()`: Chạy một lần khi khởi động.

    - `loop()`: Chạy lặp liên tục sau setup().

**5. Khu vực thông báo & Serial Monitor (Output/Console)**

- Hiển thị thông tin biên dịch, lỗi, cảnh báo.

- Hiển thị dữ liệu từ Arduino khi sử dụng Serial Monitor.

- Cho phép gõ lệnh gửi xuống Arduino khi cần.

Sử dụng cable **USB A-B** kết nối với máy tính và board **Arduino UNO R3** như hình bên dưới:

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/2_9" alt="Nano" width="800" style="display: block; margin: 0 auto;"  />