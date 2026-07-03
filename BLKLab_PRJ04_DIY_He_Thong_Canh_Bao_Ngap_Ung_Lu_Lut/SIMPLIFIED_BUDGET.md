# 💰 DANH SÁCH MŨI NGÂN SÁCH ĐỠN GIẢN (~1 TRIỆU)

## 📌 PHIÊN BẢN CHO HỌC SINH CẤP 3

**Tổng chi phí:** 1,150,000 đ (có thể giảm xuống 950K đ nếu tiết kiệm)

> **Ghi chú:** Phiên bản này loại bỏ pin dự trữ lớn, solar mppt, thiết bị premium để tập trung vào tính khả thi cho học sinh.

---

## 📦 DANH SÁCH PHỤ KIỆN CHI TIẾT

### Nhóm 1: Lõi Vi Xử Lý (Microcontroller)

| STT | Tên linh kiện | Mẫu | Nơi mua | Giá (K đ) | SL | Cộng (K đ) | Ghi chú |
|-----|--------------|-----|--------|----------|----|-----------|----|
| 1 | Arduino UNO R3 | A000066 (CH340) | Shopee | 150 | 1 | 150 | Vi xử lý chính |

**Lựa chọn:** Arduino UNO R3 (phổ biến, dễ học)

---

### Nhóm 2: Cảm Biến & Đo Đạc (Sensors)

| STT | Tên linh kiện | Mẫu | Nơi mua | Giá (K đ) | SL | Cộng (K đ) | Ghi chú |
|-----|--------------|-----|--------|----------|----|-----------|----|
| 2 | Cảm biến siêu âm | SRF04 | Shopee | 35 | 1 | 35 | Đo khoảng cách/mực nước |
| 3 | Module RTC | DS1307 hoặc DS3231 | Shopee | 50 | 1 | 50 | Đồng hồ thời gian thực |
| 4 | Cảm biến khí ẩm | DHT22 (tùy chọn) | Shopee | 60 | 0 | 0 | Nâng cấp sau |

**Cộng cộng:** 85K

---

### Nhóm 3: Hiển Thị & Người Dùng Interface

| STT | Tên linh kiện | Mẫu | Nơi mua | Giá (K đ) | SL | Cộng (K đ) | Ghi chú |
|-----|--------------|-----|--------|----------|----|-----------|----|
| 5 | LCD 16x2 I2C | 1602A + IIC Adapter | Shopee | 100 | 1 | 100 | Hiển thị mực nước + thời gian |
| 6 | Buzzer (còi báo) | HJ-1687 hoặc Active Buzzer | Shopee | 15 | 1 | 15 | Âm báo cảnh báo |

**Cộng cộng:** 115K

---

### Nhóm 4: Giao Tiếp & Truyền Dữ Liệu

| STT | Tên linh kiện | Mẫu | Nơi mua | Giá (K đ) | SL | Cộng (K đ) | Ghi chú |
|-----|--------------|-----|--------|----------|----|-----------|----|
| 7 | Module SIM GSM/2G | A7680C hoặc SIM800L | Shopee | 120 | 1 | 120 | Gửi SMS cảnh báo |
| 8 | Bộ breakout board | SIM A7680 carrier | Shopee | 25 | 1 | 25 | Hỗ trợ lắp A7680 |
| 9 | Dây jumper M-M | 40 dây, 1 bộ | Shopee | 20 | 1 | 20 | Kết nối breadboard |
| 10 | Breadboard | 400 lỗ | Shopee | 35 | 1 | 35 | Không cần焊 linh kiện |

**Cộng cộng:** 200K

---

### Nhóm 5: Năng Lượng & Dây Cấp

| STT | Tên linh kiện | Mẫu | Nơi mua | Giá (K đ) | SL | Cộng (K đ) | Ghi chú |
|-----|--------------|-----|--------|----------|----|-----------|----|
| 11 | Pin Lithium 18650 | Li-ion 3000mAh (2 cái) | Shopee | 50 | 2 | 100 | 2×18650 = 7.4V, kết cấu series |
| 12 | Holder pin 2×18650 | Có chân | Shopee | 15 | 1 | 15 | Giữ pin, dễ thay |
| 13 | DC-DC Buck Converter | 12V→5V hoặc 7.4V→5V | Shopee | 30 | 1 | 30 | Chuyển đổi điện áp |
| 14 | Dây cấp USB | Micro-USB hoặc Type-C | Shopee | 15 | 1 | 15 | Sạc pin từ powerbank/computer |
| 15 | Powerbank 10,000mAh | Bất kỳ | Có sẵn | 0 | - | 0 | Sạc pin Arduino (mượn) |

**Cộng cộng:** 160K (không tính powerbank sẵn có)

> **Lưu ý:** Thay vì dùng pin LiFePO₄ lớn 3.7M, ta dùng 2 pin 18650 (~100K) để tiết kiệm. Chỉ chạy được 1-2 ngày, nhưng cho học sinh thử nghiệm cũng đủ.

---

### Nhóm 6: Năng Lượng Mặt Trời (SOLAR - Tùy Chọn)

| STT | Tên linh kiện | Mẫu | Nơi mua | Giá (K đ) | SL | Cộng (K đ) | Ghi chú |
|-----|--------------|-----|--------|----------|----|-----------|----|
| 16 | Panel Solar Mini | 5W @ 6V (Tùy chọn) | Shopee | 150 | 1 | 150 | Sạc pin từ mặt trời |
| 17 | USB Solar Charger | Có sẵn (PowerBank với solar) | Có sẵn | 0 | - | 0 | Mượn từ nhà |

**Cộng cộng:** 150K (nếu mua panel) hoặc 0K (nếu không)

> **Ghi chú:** Nếu ngân sách chặt, có thể loại bỏ panel lúc trước, sau đó thêm sau. Dùng powerbank đã có để sạc qua USB.

---

### Nhóm 7: Bảo Vệ & Mạch Bổ Trợ

| STT | Tên linh kiện | Mẫu | Nơi mua | Giá (K đ) | SL | Cộng (K đ) | Ghi chú |
|-----|--------------|-----|--------|----------|----|-----------|----|
| 18 | Resistor 10kΩ | 1/4W, loạt | Điện tử Việt | 5 | 2 | 10 | Chia áp điều khỉn |
| 19 | Resistor 2.2kΩ | 1/4W, loạt | Điện tử Việt | 5 | 2 | 10 | Chia áp điều khỉn |
| 20 | Tụ điện 10µF | Electrolytic | Điện tử Việt | 5 | 3 | 15 | Lọc nhiễu |
| 21 | Tụ điện 0.1µF | Ceramic | Điện tử Việt | 2 | 3 | 6 | Lọc nhiễu RF |
| 22 | Đèn LED đỏ | 5mm | Điện tử Việt | 1 | 5 | 5 | Kiểm tra + Indicator |
| 23 | Cầu chì 5A | Nhanh, mini | Điện tử Việt | 3 | 2 | 6 | Bảo vệ quá dòng |
| 24 | Bread + Jumper Set | Bộ combo | Shopee | 40 | 1 | 40 | Toàn bộ dây, resistor, tụ |

**Cộng cộng:** 92K (nếu mua riêng) hoặc 40K (nếu mua combo)

> **Mẹo tiết kiệm:** Mua bộ combo Bread board + jumper + resistor + tụ từ Shopee (~40K) thay vì mua riêng lẻ.

---

### Nhóm 8: SIM Card & Dịch Vụ

| STT | Tên linh kiện | Mẫu | Nơi mua | Giá (K đ) | SL | Cộng (K đ) | Ghi chú |
|-----|--------------|-----|--------|----------|----|-----------|----|
| 25 | Thẻ SIM 2G/3G | Bất kỳ (Viettel, Mobi, Vinaphone) | Cửa hàng | 0 | 1 | 0 | Dùng thẻ cũ hoặc mua |
| 26 | Gói SMS 1 tháng | 10K SMS | Operator | 50 | 1 | 50 | Dùng test khoảng 1 tháng |

**Cộng cộng:** 50K (chỉ gói SMS, SIM miễn phí)

---

## 💰 TÍNH TOÁN CHI PHÍ TỔNG

| Nhóm | Nội dung | Chi phí (K đ) |
|-----|---------|--------------|
| 1 | Lõi Vi Xử Lý | 150 |
| 2 | Cảm Biến & Đo Đạc | 85 |
| 3 | Hiển Thị & Giao Diện | 115 |
| 4 | Giao Tiếp & Truyền Dữ Liệu | 200 |
| 5 | Năng Lượng & Dây Cấp | 160 |
| 6 | **Solar (Tùy chọn)** | **150** |
| 7 | Bảo Vệ & Mạch Bổ Trợ | 40 |
| 8 | SIM & Dịch Vụ | 50 |
| | **TỔNG (không Solar)** | **800** |
| | **TỔNG (có Solar)** | **950** |

### Variant: Có thêm phụ kiện để gá ngoài trời

| Phụ kiện | Giá (K đ) |
|----------|----------|
| Hộp chứa chống nước IP65 | 100 |
| Dây đặt panel ngoài trời | 50 |
| Clips cố định PCB | 30 |
| Mực tính | 20 |
| | **Cộng: 200K** |

### **TỔNG CÓ THỂ TĂNG LÊN**
- **Không solar + ngoài trời:** 1,000K
- **Có solar + ngoài trời:** 1,150K ✅ (Đúng ngân sách)

---

## 🛒 NƠI MUA PHỤ KIỆN

### Recommended (Uy tín + Giá rẻ)

| Nơi mua | Liên kết | Ghi chú |
|---------|---------|---------|
| **Shopee** | shopee.vn | Tất cả linh kiện điện tử cơ bản |
| **Điện Tử Việt** | dienta.vn | Resistor, tụ, linh kiện nhỏ |
| **Arduino Store VN** | Arduino VN hoặc Shopee | Arduino, Shield, Sensor cao cấp |
| **Lạc Việt** | lacviet.com.vn | Linh kiện tin cậy, bảo hành |

### Mẫu Tìm Kiếm Shopee

```
Shopee Search:
- "Arduino UNO R3" → ~150K
- "SRF04 ultrasonic sensor" → ~35K
- "LCD 16x2 I2C" → ~100K
- "SIM A7680C module" → ~120K
- "Breadboard 400 hole" → ~35K
- "Pin 18650 lithium" → ~50K (2 cái)
- "DC-DC 12V to 5V converter" → ~30K
- "Solar panel 5W" → ~150K (nếu mua)
```

---

## 💡 CÁC CÁCH TIẾT KIỆM CHI PHÍ

### 1. Mượn Thiết Bị Có Sẵn
```
✅ PowerBank 10,000mAh (thay vì pin 18650+holder)
   → Tiết kiệm: 100K đ

✅ Laptop/Computer để nạp code (thay vì mua FTDI)
   → Tiết kiệm: 50K đ

✅ Smartphone cũ để test SMS (không cần mua riêng)
   → Tiết kiệm: 100K đ
```

### 2. Mua Combo Bundle
```
✅ Bộ Breadboard + Jumper + Resistor (1 bộ ~40K)
   Thay vì mua riêng lẻ (~120K)
   → Tiết kiệm: 80K đ

✅ Bộ Sensor 37-in-1 (nếu dùng lại sau)
   → Tiết kiệm ngoài dự án này
```

### 3. Tìm Đơn Vị Giáo Dục
```
✅ Nhờ trường cấp Arduino + Breadboard sẵn
   → Tiết kiệm: 150K đ

✅ Nhờ nhà mạng cấp SIM thử nghiệm
   → Tiết kiệm: 50K đ
```

### 4. Mua Hàng Cùng Bạn Bè
```
✅ Chia sẻ chi phí vận chuyển
✅ Mua số lượng lớn để xin giảm giá
```

---

## 🎯 PHIÊN BẢN TỐI THIỂU (Học Tập Ở NHÀ)

Nếu chỉ muốn học/thí nghiệm ở nhà, có thể loại bỏ một số phụ kiện:

| Tên | Giá gốc | Loại bỏ? | Giá mới |
|-----|---------|---------|---------|
| Arduino UNO R3 | 150 | Không | 150 |
| Cảm biến SRF04 | 35 | Không | 35 |
| LCD 16x2 I2C | 100 | Không | 100 |
| RTC DS1307 | 50 | Không | 50 |
| Module SIM A7680C | 120 | ❌ Loại bỏ | 0 |
| Buzzer | 15 | Không | 15 |
| Pin 18650×2 | 100 | Thay → USB Power | 0 |
| Solar Panel | 150 | ❌ Loại bỏ | 0 |
| Dây + Resistor + Tụ | 100 | Không | 100 |
| **TỔNG** | 820 | - | **450** |

**=> Phiên bản tối thiểu: 450K đ** (Tất cả chức năng ngoại trừ gửi SMS & Solar)

---

## 🔄 NÂNG CẤP SAU CUỘC THI

Sau khi chiến thắng/hoàn thành cuộc thi, có thể nâng cấp:

| Nâng cấp | Chi phí | Chức năng thêm |
|---------|---------|--------------|
| Pin dự trữ 10Ah | +1,500K | Chạy 5-7 ngày |
| MPPT Controller | +1,000K | Sạc hiệu quả 90% |
| Cảm biến DHT22 | +60K | Đo nhiệt độ, độ ẩm |
| Cloud (ThingSpeak) | Miễn phí | Theo dõi từ xa |
| LCD TFT 3.5" | +200K | Giao diện đẹp hơn |
| GPS Module | +100K | Định vị vị trí cảnh báo |

---

## 📋 CHECK LIST MUA HÀNG

```
☐ Arduino UNO R3 × 1
☐ Cảm biến SRF04 × 1
☐ Module RTC (DS1307 hoặc DS3231) × 1
☐ LCD 16x2 I2C × 1
☐ Module SIM A7680C × 1
☐ Breadboard 400 lỗ × 1
☐ Dây jumper M-M × 40 dây
☐ Resistor 10k, 2.2k Ω (bộ)
☐ Tụ điện 10µF, 0.1µF (bộ)
☐ Buzzer chủ động × 1
☐ Pin 18650 × 2 cái
☐ Holder pin 2×18650 × 1
☐ DC-DC 5V Converter × 1
☐ LED đỏ 5mm × 5 cái
☐ Cầu chì 5A × 2 cái
☐ Solar Panel 5W (nếu ngân sách cho phép)
☐ Hộp chứa IP65 (nếu lắp ngoài trời)
```

---

## 📝 LỊCH SỬ CHỈNH SỬA

- **2026-06-15:** Tạo danh sách mũi đơn giản cho học sinh cấp 3
- **Phiên bản:** 1.0 (Chi phí ~1 triệu)

---

**💡 Mẹo:** In danh sách này ra và mang theo khi mua hàng để không quên linh kiện!
