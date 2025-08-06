# Arduino

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_1.png" alt="Arduino" width="400" style="display: block; margin: 0 auto;" />

## Arduino là gì?

Arduino là nền tảng điện tử mã nguồn mở gồm phần cứng và phần mềm, dễ sử dụng và linh hoạt. Các bo mạch Arduino có thể đọc dữ liệu từ môi trường (ánh sáng, nhiệt độ, độ ẩm, nút nhấn, tin nhắn…), sau đó điều khiển các thiết bị như động cơ, đèn LED hoặc gửi thông tin đến nơi khác. Vi điều khiển trên bo mạch được lập trình bằng ngôn ngữ C++ thông qua Arduino IDE, biên dịch thành mã máy để thực thi.

Ra đời năm 2003 tại Học viện Interaction Design, Ivrea (Italy), Arduino ban đầu được tạo ra nhằm giúp sinh viên không chuyên về điện tử có thể nhanh chóng chế tạo sản phẩm với chi phí thấp. Là một dự án mã nguồn mở, Arduino khuyến khích cộng đồng toàn cầu cùng phát triển và đóng góp.

Tên Arduino lấy từ một quán bar ở Ivrea, nơi các nhà sáng lập gặp nhau hình thành ý tưởng. Quán được đặt theo tên một vị chỉ huy quân đội, sau trở thành vua Italy giai đoạn 1002–1014.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_2.png" alt="Arduino" width="400" style="display: block; margin: 0 auto;"  />

## Tại sao là Arduino?

Arduino được ưa chuộng trong nhiều lĩnh vực nhờ sự đơn giản, dễ sử dụng và mã nguồn mở. Nó phù hợp cho cả người mới bắt đầu lẫn lập trình viên giàu kinh nghiệm. Cộng đồng Arduino toàn cầu rất lớn, cung cấp nhiều thư viện và hỗ trợ mạnh mẽ, giúp người dùng dễ dàng tìm giải pháp cho vấn đề gặp phải.

Với người không chuyên điện tử như kiến trúc sư, giáo viên, nghệ sĩ…, Arduino giúp họ nhanh chóng tạo ra sản phẩm điện tử mà trước đây gần như không thể. Với kỹ sư điện tử, Arduino có thể bị xem là quá đơn giản vì nó che giấu phần phức tạp của lập trình vi điều khiển. Tuy nhiên, để làm chủ Arduino ở mức cao hơn (tùy biến hiệu ứng LED, kết nối cảm biến, truyền nhận dữ liệu…), đòi hỏi kiến thức sâu về vi điều khiển và các giao thức truyền dữ liệu.

Ưu điểm lớn nhất của Arduino là cú pháp lệnh đơn giản, thư viện phong phú, cộng đồng lớn và mã nguồn mở. Ngoài ra, Arduino còn cung cấp Hardware Abstraction Library (HAL) dành cho những ai muốn nghiên cứu sâu cách thức hoạt động bên trong của nền tảng này.

# Các Board Mạch Arduino Trên Thị Trường

Hiện nay, thị trường có hàng trăm board mạch Arduino khác nhau, chủ yếu là các biến thể PCB của những board mạch chính từ nhà sản xuất Arduino. Các board này thường được cải tiến với tính năng mới hoặc thiết kế lại để giảm giá thành, giúp tiếp cận nhiều người dùng hơn. Dưới đây là một số board mạch Arduino chính:

## 1. Arduino Uno R3

Arduino Uno R3 là board mạch phổ biến nhất, phù hợp cho người mới bắt đầu học về điện tử và lập trình.

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_8.png" alt="Arduino" width="400" style="display: block; margin: 0 auto;" />

### Thông số kỹ thuật

| **Thông số**                     | **Giá trị**                     |
|----------------------------------|---------------------------------|
| Vi điều khiển                   | ATmega328P                     |
| Điện áp hoạt động               | 5V                             |
| Điện áp đầu vào (khuyến nghị)   | 7-12V                          |
| Điện áp đầu vào (giới hạn)      | 6-20V                          |
| Chân Digital I/O                | 14 (6 chân hỗ trợ PWM)         |
| Chân Analog                     | 6                              |
| Dòng điện tối đa mỗi chân I/O   | 20 mA                          |
| Dòng điện cho chân 3.3V         | 50 mA                          |
| Bộ nhớ Flash                    | 32 KB (0.5 KB dùng cho bootloader) |
| SRAM                            | 2 KB                           |
| EEPROM                          | 1 KB                           |
| Tần số xung nhịp                | 16 MHz                         |
| Kích thước                      | 68.6 mm x 53.4 mm              |
| Trọng lượng                     | 25 g                           |

### Đặc điểm nổi bật:
- Lập trình qua cổng USB.
- 4 LED: nguồn, RX, TX, Debug.
- Nút nhấn Reset.
- Jack nguồn DC (khi không dùng USB).
- Header cho In-circuit Serial Programmer (ICSP).
- **Giá tham khảo**: €20.00 (theo www.arduino.cc).

### Giới thiệu vi điều khiển ATmega328:
ATmega328 là vi điều khiển 8-bit của Atmel, tích hợp CPU, RAM (2 KB), ROM (32 KB Flash), EEPROM (1 KB), và các giao thức I2C, SPI. Nó là "trái tim" của Arduino Uno R3, hỗ trợ giao tiếp với cảm biến và điều khiển thiết bị.

| **Thông số**                     | **Giá trị**                     |
|----------------------------------|---------------------------------|
| CPU                              | 8-bit AVR                      |
| Hiệu suất                        | 20 MIPS tại 20 MHz             |
| Flash                            | 32 KB                          |
| SRAM                             | 2 KB                           |
| EEPROM                           | 1 KB                           |
| Số chân                          | 28 (PDIP, MLF), 32 (TQFP, MLF) |
| Tần số tối đa                    | 20 MHz                         |
| Kênh cảm ứng                     | 16                             |
| Chân I/O tối đa                  | 26                             |
| Ngắt ngoài                       | 2                              |
| Giao diện USB                    | Không                          |

## 2. Arduino Nano

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_3.jpg" alt="Nano" width="400" style="display: block; margin: 0 auto;"  />

Arduino Nano là phiên bản thu gọn của Uno, sử dụng vi điều khiển ATmega328 (Nano 3.x) hoặc ATmega168 (Nano 2.x), được thiết kế cho các dự án cần kích thước nhỏ gọn, dễ tích hợp vào hệ thống nhúng.

### Thông số kỹ thuật

| **Thông số**                     | **Giá trị (Nano 3.x)**         | **Giá trị (Nano 2.x)**         |
|----------------------------------|--------------------------------|--------------------------------|
| Vi điều khiển                   | ATmega328                     | ATmega168                     |
| Điện áp hoạt động               | 5V                            | 5V                            |
| Điện áp đầu vào (khuyến nghị)   | 7-12V                         | 7-12V                         |
| Điện áp đầu vào (giới hạn)      | 6-20V                         | 6-20V                         |
| Chân Digital I/O                | 14 (6 chân hỗ trợ PWM)        | 14 (6 chân hỗ trợ PWM)        |
| Chân Analog                     | 8                             | 8                             |
| Dòng điện tối đa mỗi chân I/O   | 20 mA                         | 20 mA                         |
| Dòng điện cho chân 3.3V         | 50 mA                         | 50 mA                         |
| Bộ nhớ Flash                    | 32 KB (2 KB dùng cho bootloader) | 16 KB (2 KB dùng cho bootloader) |
| SRAM                            | 2 KB                          | 1 KB                          |
| EEPROM                          | 1 KB                          | 0.5 KB                        |
| Tần số xung nhịp                | 16 MHz                        | 16 MHz                        |
| Kích thước                      | 45 mm x 18 mm                 | 45 mm x 18 mm                 |
| Trọng lượng                     | 7 g                           | 7 g                           |

### Đặc điểm nổi bật:
- **Kích thước nhỏ gọn**: Với kích thước chỉ 45 mm x 18 mm, Nano lý tưởng cho các dự án cần tiết kiệm không gian, như thiết bị đeo hoặc hệ thống nhúng.
- **Cổng Mini-B USB**: Thay thế cổng USB-B chuẩn, phù hợp với thiết kế nhỏ gọn.
- **Chân Analog bổ sung**: 8 chân Analog (so với 6 của Uno R3), tăng khả năng đọc dữ liệu từ cảm biến.
- **Không có jack nguồn DC**: Chỉ cấp nguồn qua USB hoặc chân VIN, giúp giảm kích thước.
- **Tương thích với Uno**: Sử dụng vi điều khiển tương tự Uno R3 (ATmega328 cho Nano 3.x), hỗ trợ hầu hết các thư viện và shield (với adapter).
- **Giá tham khảo**: €21.00 (theo www.arduino.cc).

### Ứng dụng:
Arduino Nano phù hợp cho các dự án nhỏ gọn như thiết bị IoT, cảm biến di động, robot mini, hoặc các ứng dụng cần tích hợp vào không gian hạn chế như quần áo thông minh hoặc mô hình điều khiển từ xa.

## 3. Arduino Leonardo

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_4.jpeg" alt="arduino" width="400" style="display: block; margin: 0 auto;" />

Arduino Leonardo sử dụng vi điều khiển ATmega32u4, tích hợp khả năng giao tiếp USB trực tiếp, phù hợp cho các dự án cần giả lập thiết bị đầu vào như chuột, bàn phím.

### Thông số kỹ thuật

| **Thông số**                     | **Giá trị**                     |
|----------------------------------|---------------------------------|
| Vi điều khiển                   | ATmega32u4                     |
| Điện áp hoạt động               | 5V                             |
| Điện áp đầu vào (khuyến nghị)   | 7-12V                          |
| Điện áp đầu vào (giới hạn)      | 6-20V                          |
| Chân Digital I/O                | 20 (7 chân hỗ trợ PWM)         |
| Chân Analog                     | 12                             |
| Dòng điện tối đa mỗi chân I/O   | 20 mA                          |
| Dòng điện cho chân 3.3V         | 50 mA                          |
| Bộ nhớ Flash                    | 32 KB (4 KB dùng cho bootloader) |
| SRAM                            | 2.5 KB                         |
| EEPROM                          | 1 KB                           |
| Tần số xung nhịp                | 16 MHz                         |
| Kích thước                      | 68.6 mm x 53.4 mm              |
| Trọng lượng                     | 20 g                           |

### Đặc điểm nổi bật:
- **Tích hợp USB-to-Serial**: Vi điều khiển ATmega32u4 có khả năng giao tiếp USB trực tiếp, không cần chip FTDI riêng như Uno R3, giúp giảm chi phí và tăng tốc độ truyền dữ liệu.
- **Hỗ trợ HID**: Có thể giả lập chuột, bàn phím, hoặc joystick, lý tưởng cho các dự án giao diện người-máy.
- **Nhiều chân I/O hơn**: 20 chân Digital I/O (7 chân PWM) và 12 chân Analog, tăng khả năng kết nối so với Uno R3.
- **Cổng USB và jack nguồn DC**: Hỗ trợ lập trình và cấp nguồn linh hoạt.
- **Tương thích với shield**: Giữ nguyên kích thước của Uno R3, tương thích với nhiều shield Arduino.
- **Giá tham khảo**: €18.00 (theo www.arduino.cc).

### Ứng dụng:
Arduino Leonardo phù hợp cho các dự án cần tương tác USB, như bàn phím tùy chỉnh, bộ điều khiển game, hoặc thiết bị mô phỏng đầu vào. Nó cũng được sử dụng trong các ứng dụng tự động hóa hoặc điều khiển thiết bị với số lượng cảm biến lớn.

## 4. Arduino Mega 2560

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_5.jpg" alt="Arduino" width="400" style="display: block; margin: 0 auto;" />

Arduino Mega 2560 là phiên bản nâng cấp mạnh mẽ của Uno, sử dụng vi điều khiển ATmega2560, được thiết kế cho các dự án phức tạp đòi hỏi nhiều chân I/O và bộ nhớ lớn, như máy in 3D, robot, hoặc hệ thống điều khiển tự động.

### Thông số kỹ thuật

| **Thông số**                     | **Giá trị**                     |
|----------------------------------|---------------------------------|
| Vi điều khiển                   | ATmega2560                     |
| Điện áp hoạt động               | 5V                             |
| Điện áp đầu vào (khuyến nghị)   | 7-12V                          |
| Điện áp đầu vào (giới hạn)      | 6-20V                          |
| Chân Digital I/O                | 54 (15 chân hỗ trợ PWM)        |
| Chân Analog                     | 16                             |
| Dòng điện tối đa mỗi chân I/O   | 20 mA                          |
| Dòng điện cho chân 3.3V         | 50 mA                          |
| Bộ nhớ Flash                    | 256 KB (8 KB dùng cho bootloader) |
| SRAM                            | 8 KB                           |
| EEPROM                          | 4 KB                           |
| Tần số xung nhịp                | 16 MHz                         |
| UART                            | 4                              |
| Kích thước                      | 101.52 mm x 53.3 mm            |
| Trọng lượng                     | 37 g                           |

### Đặc điểm nổi bật:
- **Số lượng chân I/O lớn**: 54 chân Digital I/O (15 chân PWM) và 16 chân Analog, lý tưởng cho các dự án cần nhiều kết nối cảm biến và thiết bị.
- **Bộ nhớ lớn**: Flash 256 KB (gấp 8 lần Uno R3), SRAM 8 KB, EEPROM 4 KB, hỗ trợ các chương trình phức tạp.
- **4 cổng UART**: Cho phép giao tiếp với nhiều thiết bị ngoại vi qua giao thức Serial.
- **Cổng USB và jack nguồn DC**: Hỗ trợ lập trình và cấp nguồn linh hoạt.
- **Hỗ trợ shield**: Tương thích với nhiều shield Arduino, mở rộng khả năng như kết nối Ethernet, WiFi, hoặc điều khiển động cơ.
- **Giá tham khảo**: €38.50 (theo www.arduino.cc).

### Ứng dụng:
Arduino Mega 2560 phù hợp cho các dự án yêu cầu xử lý nhiều dữ liệu hoặc điều khiển đồng thời nhiều thiết bị, như robot tự hành, máy in 3D, hệ thống giám sát môi trường, hoặc các ứng dụng IoT phức tạp.

## 5. Arduino UNO R4

Arduino UNO R4 là phiên bản mới nhất của dòng UNO, ra mắt vào tháng 6/2023, mang đến bước đột phá với vi điều khiển 32-bit và nhiều tính năng hiện đại. UNO R4 có hai phiên bản: **UNO R4 Minima** (tập trung vào hiệu suất cơ bản) và **UNO R4 WiFi** (tích hợp kết nối không dây và ma trận LED).

<img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_6.webp" alt="UNO_R4" width="400" />  <img src="https://raw.githubusercontent.com/theduong6168/BLKLab/refs/heads/main/image/1_7.webp" alt="UNO_R4" width="400" />

### Thông số kỹ thuật (UNO R4 Minima và WiFi)

| **Thông số**                     | **Giá trị (UNO R4 Minima)**     | **Giá trị (UNO R4 WiFi)**       |
|----------------------------------|----------------------------------|----------------------------------|
| Vi điều khiển                   | Renesas RA4M1 (ARM Cortex-M4)   | Renesas RA4M1 (ARM Cortex-M4)   |
| Đồng xử lý                      | Không                           | ESP32-S3-MINI (WiFi/BT)         |
| Điện áp hoạt động               | 5V                              | 5V                              |
| Điện áp đầu vào (khuyến nghị)   | 6-24V                           | 6-24V                           |
| Chân Digital I/O                | 14 (6 chân hỗ trợ PWM)          | 14 (6 chân hỗ trợ PWM)          |
| Chân Analog                     | 6 (14-bit ADC)                  | 6 (14-bit ADC)                  |
| Dòng điện tối đa mỗi chân I/O   | 8 mA                            | 8 mA                            |
| Bộ nhớ Flash                    | 256 KB                          | 256 KB                          |
| SRAM                            | 32 KB                           | 32 KB                           |
| EEPROM                          | 8 KB (Data Flash)               | 8 KB (Data Flash)               |
| Tần số xung nhịp                | 48 MHz                          | 48 MHz                          |
| Kích thước                      | 68.6 mm x 53.4 mm               | 68.6 mm x 53.4 mm               |
| Trọng lượng                     | 25 g                            | 25 g                            |
| Tính năng bổ sung               | DAC 12-bit, CAN Bus, OP AMP     | DAC 12-bit, CAN Bus, OP AMP, Ma trận LED 12x8, Qwiic I2C, WiFi/Bluetooth |

### Đặc điểm nổi bật:
- **Vi điều khiển 32-bit**: Sử dụng Renesas RA4M1 (ARM Cortex-M4) chạy ở 48 MHz, mạnh hơn gấp 3 lần so với Uno R3, với bộ nhớ Flash 256 KB (gấp 8 lần) và SRAM 32 KB (gấp 16 lần).
- **Cổng USB-C**: Thay thế cổng USB-B, hỗ trợ tốc độ truyền dữ liệu nhanh hơn và tương thích với thiết bị hiện đại.
- **Điện áp đầu vào mở rộng**: Hỗ trợ lên đến 24V, phù hợp với nhiều thiết bị như động cơ, dải LED.
- **Tính năng bổ sung (UNO R4 WiFi)**:
  - Tích hợp module ESP32-S3 cho kết nối WiFi và Bluetooth, hỗ trợ IoT và điều khiển từ xa qua Arduino IoT Cloud.
  - Ma trận LED 12x8 (96 điểm) sử dụng Charlieplexing, lý tưởng cho hiển thị hình ảnh động hoặc dữ liệu cảm biến.
  - Cổng Qwiic I2C để kết nối dễ dàng với các module cảm biến và thiết bị ngoại vi.
- **Hỗ trợ HID**: Có thể giả lập chuột, bàn phím hoặc gamepad qua USB.
- **Tương thích ngược**: Giữ nguyên kích thước, chân cắm và điện áp 5V của Uno R3, đảm bảo tương thích với các shield hiện có.
- **Giá tham khảo**: €18.00 (Minima), €25.00 (WiFi) (theo www.arduino.cc).

### Ứng dụng:
- **UNO R4 Minima**: Phù hợp cho các dự án yêu cầu hiệu suất cao nhưng không cần kết nối không dây, như tự động hóa, điều khiển âm thanh (nhờ DAC 12-bit), hoặc các ứng dụng CAN Bus.
- **UNO R4 WiFi**: Lý tưởng cho các dự án IoT, hiển thị hình ảnh động (qua ma trận LED), hoặc điều khiển từ xa như drone, DJ controller, hoặc hệ thống chiếu sáng thông minh.

## 6. Bộ KIT Học Tập Arduino Uno R3 BLK Plus

Bộ KIT Học Tập Arduino Uno R3 BLK Plus là bộ công cụ học tập được biên soạn bởi BanLinhKien, phiên bản nâng cấp của Bộ KIT Arduino Uno R3 RFID BLK, nhằm hỗ trợ người học từ cơ bản đến nâng cao khám phá lập trình và ứng dụng Arduino. Với 41 linh kiện và module, bộ KIT này giúp người dùng làm quen với các giao thức giao tiếp hiện đại và thực hiện nhiều dự án sáng tạo.

<img src="https://pos.nvncdn.com/f2fe44-24897/ps/20230821_o5LKOchlIF.jpeg" alt="Bộ KIT Học Tập Arduino Uno R3 BLK Plus" width="400" />   <img src="https://pos.nvncdn.com/f2fe44-24897/ps/20230818_gWTNTGRu66.jpeg" alt="Bộ KIT Học Tập Arduino Uno R3 BLK Plus" width="400" />   

### Đặc điểm nổi bật
- **Phiên bản nâng cấp**: Bao gồm 41 linh kiện/module (so với 34 của Bộ KIT Uno R3 RFID BLK), hỗ trợ các giao thức I2C, SPI, USART, OneWire, phù hợp cho cả người mới bắt đầu và người học nâng cao.
- **Module đa dạng**: Bao gồm cảm biến siêu âm SRF04, cảm biến độ ẩm đất TH, module nhiệt độ/độ ẩm DHT11, module thời gian thực DS1307+AT24C32, và module NE555, mở rộng khả năng sáng tạo.
- **Dự án phong phú**: Hỗ trợ hơn 40 dự án từ cơ bản (điều khiển LED, hiển thị LCD) đến nâng cao (hệ thống nhà thông minh, khóa cửa RFID, thùng rác thông minh), với mã nguồn và sơ đồ đấu nối được cung cấp.
- **Dễ sử dụng**: Các linh kiện được chuyển thành module để dễ kết nối, đi kèm hộp đựng chuyên dụng HD-UNO, phù hợp cho học sinh, sinh viên, và người đam mê điện tử.
- **Bảo hành**: 6 tháng cho board Arduino Uno R3, đổi trả trong 7 ngày nếu có lỗi nhà sản xuất (trừ trường hợp cháy nổ).
- **Giá tham khảo**: 699,000 VNĐ (theo banlinhkien.com).[](https://banlinhkien.com/bo-kit-hoc-tap-arduino-uno-r3-blk-plus-p38419270.html)

### Bảng danh sách linh kiện

| **Tên Linh Kiện**                                    | **Số lượng** | **Tên Linh Kiện**                                    | **Số lượng** |
|------------------------------------------------------|--------------|------------------------------------------------------|--------------|
| KIT Arduino UNO R3 CH340G (Kèm Dây USB và Jump Đơn 2.54mm) | 1            | Module LED 7 0.36 4 Số Katot                        | 1            |
| LCD1602 Xanh Lá 5V                                   | 1            | LED 7 0.56 1 Số Anot (Sáng Đỏ)                      | 1            |
| Module Chuyển Đổi I2C cho LCD1602                    | 1            | IC 74HC595 DIP16                                    | 1            |
| Module Điều khiển Động Cơ L298 V3 5-35VDC 2A         | 1            | LED Matrix 2 Màu Xanh Đỏ 8x8 38x38x7.2mm Anode Chung | 1            |
| Động cơ Mini 130 1-6V 18000RPM                       | 1            | Module Thu Hồng Ngoại 1838T MH-R38                  | 1            |
| Động cơ Servo SG90 (Góc Quay 180)                    | 1            | Remote Hồng Ngoại 20 Phím                           | 1            |
| Động Cơ Bước 5V STEP MOTOR 28BYJ-48 5VDC             | 1            | Nút Nhấn 4 Chân 12x12x7.3MM (OMRON B3F)             | 4            |
| Module Cảm Biến Ánh Sáng MS-CDS05                    | 1            | Vỏ Nút Nhấn B3F 10MM (Tròn Đen)                     | 4            |
| Module Cảm Biến Rung SW-420                          | 1            | Trở Vạch 1/4W 5% 220R                               | 10           |
| Module Cảm Biến Âm Thanh                             | 1            | Trở Vạch 1/4W 5% 1K                                 | 10           |
| Cảm Biến Siêu Âm SRF04                               | 1            | Trở Vạch 1/4W 5% 10K                                | 10           |
| Module relay Mini 1 kênh 5V10A BLK                   | 1            | LED 5MM Phủ Đỏ                                      | 10           |
| Module Cảm Biến Mưa                                  | 1            | LED 5MM Phủ Xanh Lá                                 | 10           |
| Cảm Biến Đo Độ Ẩm Đất TH                             | 1            | LED 5MM Phủ Vàng                                    | 10           |
| Module NE555                                         | 1            | Pin 9V (Loại Thường)                                | 1            |
| Module DHT11                                         | 1            | Dây Đế Pin 9V Ra Đầu Jack DC5.5x2.1mm               | 1            |
| Module DS1307+AT24C32                                | 1            | Triết Áp Đơn B10K                                   | 1            |
| Module LED Neo Pixel 12 LED RGB                      | 1            | Còi Chíp 5V 9.5x12MM                                | 1            |
| Board Test GL No.12                                  | 1            | Keypad 4x4 SMD                                      | 1            |
| Bộ Dây Cắm Mạch YC-65                                | 1            | Hộp Đựng KIT Học Tập Arduino HD-UNO                 | 1            |
| Dây 40P 20CM Đ-C (Hai Đầu Đực Cái) (10 Sợi)          | 1            |                                                      |              |

### Ứng dụng
Bộ KIT hỗ trợ hơn 40 dự án, từ cơ bản đến nâng cao, phù hợp cho học sinh, sinh viên, và người đam mê điện tử:
- **Cơ bản**: Điều khiển LED, hiển thị dữ liệu trên LCD1602, giao tiếp với keypad, điều khiển động cơ Servo/Step Motor.
- **Nâng cao**: Hệ thống nhà thông minh, khóa cửa RFID, thùng rác thông minh biết nói, đồng hồ LED quay, cảnh báo rò rỉ khí gas, hoặc game rắn săn mồi trên LED Matrix.
- **Ứng dụng thực tế**: Hệ thống giám sát nhiệt độ/độ ẩm, bãi đỗ xe RFID, cánh tay robot, hoặc đồng hồ cát LED Matrix, phù hợp cho học tập lập trình nhúng và IoT.

### Tài liệu
- Mã nguồn, sơ đồ đấu nối, và hướng dẫn chi tiết cho hơn 40 dự án được cung cấp tại [GitHub BanLinhKien](https://github.com/BanLinhKien/Arduino).[](https://github.com/BanLinhKien/Arduino)
- Thông tin sản phẩm và hỗ trợ kỹ thuật: [banlinhkien.com](https://banlinhkien.com/bo-kit-hoc-tap-arduino-uno-r3-blk-plus-p38419270.html).[](https://banlinhkien.com/bo-kit-hoc-tap-arduino-uno-r3-blk-plus-p38419270.html)

---

*Thông tin tham khảo từ [www.arduino.cc](https://www.arduino.cc), [wikipedia.org](https://wikipedia.org) và [banlinhkien.com](https://banlinhkien.com) .*
