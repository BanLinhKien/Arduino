# ⚡ Thông Số Kỹ Thuật Điện - Hệ Thống Cảnh Báo Ngập Lụt DIY
## Dành cho Thiết Kế Hệ Thống Năng Lượng Mặt Trời

**Ngày tạo:** 2026-06-20  
**Dự án:** PRJ04 - DIY Hệ Thống Cảnh Báo Ngập Lụt (Flood Warning System)  
**Mục đích:** Cung cấp tất cả thông số điện cần thiết để thiết kế hệ thống năng lượng mặt trời độc lập

---

## 📋 MỤC LỤC
1. [Tóm Tắt Hệ Thống](#tóm-tắt-hệ-thống)
2. [Danh Sách Phụ Kiện & Thông Số Điện](#danh-sách-phụ-kiện--thông-số-điện)
3. [Tiêu Thụ Năng Lượng Hàng Ngày](#tiêu-thụ-năng-lượng-hàng-ngày)
4. [Thông Số Pin & Bộ Lưu Trữ Năng Lượng](#thông-số-pin--bộ-lưu-trữ-năng-lượng)
5. [Thông Số Panel Solar & MPPT](#thông-số-panel-solar--mppt)
6. [Công Thức Tính Toán Hệ Thống](#công-thức-tính-toán-hệ-thống)
7. [Kích Thước & Dung Lượng Cần Thiết](#kích-thước--dung-lượng-cần-thiết)
8. [Bảng Tính Tóm Tắt](#bảng-tính-tóm-tắt)

---

## 🎯 Tóm Tắt Hệ Thống

**Chế Độ Hoạt Động:** 24/7 không lưới điện  
**Điện Áp Hệ Thống:** 7.4V hoặc 12V (pin)  
**Điện Áp Đầu Ra:** 5V (Arduino, cảm biến, LCD, modem)  
**Thời Gian Hoạt Động:** 5-7 ngày không mặt trời  
**Loại Hệ Thống:** Autonomous IoT + Solar + Battery + SMS Alert

---

## 📊 Danh Sách Phụ Kiện & Thông Số Điện

### 1. **Bộ Vi Xử Lý & Kiểm Soát Chính**

| Phụ kiện | Model | Điện áp | Dòng điện | Công suất | Ghi chú |
|----------|-------|---------|-----------|-----------|---------|
| Microcontroller | Arduino UNO R3 (CH340) | 5V | 50-100 mA | 0.25-0.5 W | Khi chạy code, không I/O nặng |
| DC-DC Buck Converter | LM2596/MP1584 (5V) | 7.4-12V → 5V | ≤500 mA output | 2.5 W max | Hiệu suất 85-92% |

### 2. **Cảm Biến & Đo Lường**

| Phụ kiện | Model | Điện áp | Dòng điện | Công suất | Ghi chú |
|----------|-------|---------|-----------|-----------|---------|
| Cảm biến nước | SRF04 Ultrasonic | 5V | 50 mA (trigger) | 0.25 W | Chỉ phát xung ngắn, tái sử dụng |
| Đồng hồ Thực tế | RTC DS1307/DS3231 | 5V | 0.5-3 mA | 0.0025-0.015 W | Chế độ quiescent rất thấp |

### 3. **Hiển Thị & Giao Diện Người Dùng**

| Phụ kiện | Model | Điện áp | Dòng điện | Công suất | Ghi chú |
|----------|-------|---------|-----------|-----------|---------|
| Màn hình LCD | 16×2 LCD + I2C Backlight | 5V | 30 mA (no backlight) / 200 mA (backlight ON) | 0.15-1.0 W | Backlight: 3.7V LED, ~60 Ω |
| Còi cảnh báo | Active Buzzer HJ-1687 | 5V | 30-100 mA | 0.15-0.5 W | Kích hoạt khi có cảnh báo |

### 4. **Giao Tiếp & Truyền Thông**

| Phụ kiện | Model | Điện áp | Dòng điện | Công suất | Ghi chú |
|----------|-------|---------|-----------|-----------|---------|
| Modem GSM/2G | SIM A7680C | 4.5-5.5V | Chế độ resting: 10-20 mA | 0.05-0.1 W | Peak SMS: 500-1200 mA * 30s |
| - | - | - | Peak SMS: 500-1200 mA | 2.5-6 W | Phát SMS: 30-60 giây/tin |
| - | - | - | Peak Call: 1000-1500 mA | 5-7.5 W | Gọi điện: 10-15 giây/cuộc |

### 5. **Điều Hòa & Bảo Vệ Năng Lượng**

| Phụ kiện | Model | Điện áp | Dòng điện | Công suất | Ghi chú |
|----------|-------|---------|-----------|-----------|---------|
| Bộ điều khiển sạc MPPT | MPPT Solar Controller | 12-24V | <20 A output | - | Tối ưu hóa xạc pin từ solar |
| Fuse & Diode | Schottky + Fuse 20A | Input solar | ≤20 A | - | Bảo vệ ngược dòng, quá dòng |
| Tụ lọc | 10µF + 0.1µF | 12V | - | - | Khử nhiễu, ổn định điện áp |

---

## 📈 Tiêu Thụ Năng Lượng Hàng Ngày

### **Chế Độ 1: Hoạt Động Bình Thường (Không SMS)**

| Thiết bị | Dòng điện | Thời gian | Năng lượng (Ah) | Năng lượng (Wh @ 12V) |
|----------|-----------|-----------|-----------------|----------------------|
| Arduino UNO | 75 mA | 24h | 1.8 Ah | 21.6 Wh |
| RTC DS1307 | 1 mA | 24h | 0.024 Ah | 0.3 Wh |
| Cảm biến ultrasonic | 50 mA * 0.5s/1s | 24h | 0.6 Ah | 7.2 Wh |
| LCD 16×2 (backlight OFF) | 30 mA | 24h | 0.72 Ah | 8.6 Wh |
| Modem GSM (idle/resting) | 15 mA | 24h | 0.36 Ah | 4.3 Wh |
| **TỔNG CHẾ ĐỘ BÌNH THƯỜNG** | **~171 mA avg** | **24h** | **~3.5 Ah** | **42 Wh** |

### **Chế Độ 2: Hoạt Động Bình Thường + LCD Backlight**

| Thiết bị | Dòng điện | Thời gian | Năng lượng (Ah) | Năng lượng (Wh @ 12V) |
|----------|-----------|-----------|-----------------|----------------------|
| **Chế độ bình thường (từ trên)** | - | - | 3.5 Ah | 42 Wh |
| LCD Backlight (20 giờ/ngày) | 170 mA | 20h | 3.4 Ah | 40.8 Wh |
| **TỔNG VỚI BACKLIGHT** | **~260 mA avg** | **24h** | **~6.9 Ah** | **83 Wh** |

### **Chế Độ 3: Cảnh Báo Với SMS (Trường Hợp Ngập Lụt)**

| Sự kiện | Dòng điện Peak | Thời gian | Năng lượng/lần | Lần/ngày | Năng lượng/ngày |
|---------|----------------|-----------|-----------------|----------|-----------------|
| Gửi 1 SMS | 700 mA | 30s | 0.0058 Ah | 3-5 lần | 0.017-0.029 Ah |
| Gọi điện cảnh báo | 1200 mA | 15s | 0.005 Ah | 1-2 lần | 0.005-0.010 Ah |
| **Tổng cộng** | - | - | - | **3-5 SMS + 1-2 cuộc gọi** | **0.022-0.039 Ah** |
| **Năng lượng (Wh @ 12V)** | - | - | - | - | **0.26-0.47 Wh** |
| **TỔNG HÀ NGÀY CÓ SMS** | - | - | - | - | **~42-83 Wh + 0.26-0.47 Wh = 42-83.5 Wh** |

### **Chế Độ 4: Cảnh Báo Liên Tục (Ngập Lụt Kéo Dài)**

Giả định SMS liên tục mỗi 5 phút (tệ nhất):

| Sự kiện | Lần/giờ | SMS/ngày | Năng lượng/SMS | Tổng/ngày |
|---------|---------|----------|----------------|-----------|
| SMS liên tục | 12 lần/giờ | 288 SMS | 0.0058 Ah | 1.67 Ah |
| Năng lượng (Wh @ 12V) | - | - | 0.07 Wh | **20 Wh** |
| **TỔNG CHẾ ĐỘ CẢNH BÁO LIÊN TỤC** | - | - | - | **~42 + 20 = 62 Wh/ngày** |

---

## 🔋 Thông Số Pin & Bộ Lưu Trữ Năng Lượng

### **Lựa Chọn 1: 2× Li-ion 18650 (Phiên Bản Sinh Viên)**

```
Cấu hình: 2 pin nối tiếp
Điện áp danh định: 2 × 3.7V = 7.4V
Dung lượng: 3000 mAh × 1 = 3000 mAh = 3 Ah
Năng lượng: 3 Ah × 7.4V = 22.2 Wh

Đặc điểm:
- Điện áp tối đa: 8.4V (mới sạc)
- Điện áp tối thiểu an toàn: 6.0V (cutoff)
- Điện áp làm việc bình thường: 7.4V
- Dòng phóng an toàn: 1-3 A (theo loại)
- Chu kỳ sạc: 300-500 lần
- Tự phóng: ~2-3% /tháng
- Tuổi thọ: 2-3 năm (sử dụng bình thường)
```

**Thời gian hoạt động với 18650:**

| Chế độ | Tiêu thụ/ngày | Pin 3Ah | Ngày hoạt động |
|-------|----------------|---------|-----------------|
| Bình thường | 42 Wh | 22.2 Wh | 0.5 ngày |
| Với backlight | 83 Wh | 22.2 Wh | 0.27 ngày |
| Có SMS (3-5 lần) | 42.3 Wh | 22.2 Wh | 0.52 ngày |

⚠️ **Kết luận:** 18650 chỉ đủ cho 12-24 giờ mà không có mặt trời. Không phù hợp cho chế độ độc lập lâu dài. Dùng cho demo/lab.

---

### **Lựa Chọn 2: LiFePO₄ 12V 10Ah (Phiên Bản Tiêu Chuẩn - ĐỀ XUẤT)**

```
Cấu hình: 4S (3.2V × 4) = 12.8V danh định
Dung lượng: 10 Ah
Năng lượng: 10 Ah × 12.8V = 128 Wh

Đặc điểm:
- Điện áp tối đa: 14V (chế độ sạc)
- Điện áp danh định: 12.8V
- Điện áp phóng tối thiểu: 9.5V (cutoff an toàn)
- Điện áp làm việc bình thường: 12V ± 0.5V
- Dòng phóng max: 50 A (pulses); 20 A constant
- Chu kỳ sạc: 2000-3000 lần
- Tự phóng: <2% /năm
- Tuổi thọ: 8-10 năm (điều kiện tốt)
- Nhiệt độ làm việc: -20°C đến +60°C
```

**Thời gian hoạt động với LiFePO₄ 10Ah @ 12.8V:**

| Chế độ | Tiêu thụ/ngày | Pin 128 Wh | Ngày hoạt động |
|-------|-----------------|-----------|-----------------|
| Bình thường (42 Wh) | 42 Wh | 128 Wh | 3.0 ngày |
| Với backlight (83 Wh) | 83 Wh | 128 Wh | 1.5 ngày |
| Có SMS (42.3 Wh) | 42.3 Wh | 128 Wh | 3.0 ngày |
| Cảnh báo liên tục (62 Wh) | 62 Wh | 128 Wh | 2.0 ngày |

✅ **Kết luận:** Cho phép 3-7 ngày độc lập mà không có mặt trời (tùy chế độ).

---

### **Lựa Chọn 3: LiFePO₄ 12V 20Ah (Phiên Bản Nâng Cao)**

```
Dung lượng: 20 Ah
Năng lượng: 20 Ah × 12.8V = 256 Wh
Thời gian hoạt động: 6 ngày bình thường, 10+ ngày nếu hemat
Sử dụng khi: Cần 2+ tuần độc lập, vùng ít mặt trời
```

---

## ☀️ Thông Số Panel Solar & MPPT

### **Panel Solar Options**

#### **5W Mini Panel (Demo/Lab)**

```
Công suất danh định: 5W
Điện áp VOC (hở mạch): ~6.5V
Điện áp VMP (điểm công suất tối đa): ~5.5V
Dòng ISC (ngắn mạch): ~0.95 A
Dòng IMP (tại VMP): ~0.91 A
Nhiệt độ hệ số: -0.23%/°C (vôn), -0.05%/°C (dòng)

Năng lượng sản xuất hàng ngày (4 giờ nắng trực tiếp):
- Ideal: 5W × 4h = 20 Wh
- Thực tế (80% hiệu suất): 16 Wh/ngày
```

#### **15W Standard Panel (ĐỀ XUẤT CHÍNH)**

```
Công suất danh định: 15W
Điện áp VOC: ~21V
Điện áp VMP: ~18V
Dòng ISC: ~0.93 A
Dòng IMP: ~0.83 A (tại 18V)
Loại: Monocrystalline
Kích thước: ~65 × 50 cm
Trọng lượng: ~2 kg

Năng lượng sản xuất hàng ngày (4 giờ nắng trực tiếp):
- Ideal: 15W × 4h = 60 Wh
- Thực tế (80% hiệu suất): 48 Wh/ngày
- Nước rút: ~0.5-0.6 Ah @ 12V
```

#### **45W Premium Panel (Phiên Bản Production)**

```
Công suất danh định: 45W
Điện áp VOC: ~21.5V
Điện áp VMP: ~18V
Dòng ISC: ~2.81 A
Dòng IMP: ~2.5 A
Loại: Monocrystalline
Kích thước: ~110 × 65 cm
Trọng lượng: ~5.5 kg

Năng lượng sản xuất hàng ngày (4 giờ nắng trực tiếp):
- Ideal: 45W × 4h = 180 Wh
- Thực tế (80% hiệu suất): 144 Wh/ngày
- Nước rút: ~1.5-1.8 Ah @ 12V
```

---

### **Bộ Điều Khiển MPPT - Thông Số**

```
Mục đích: Tối ưu hóa dòng điện sạc từ panel solar
Nguyên lý: Theo dõi điểm công suất tối đa (Maximum Power Point) của panel

Thông số MPPT tiêu chuẩn 12V/20A:
- Điện áp input: 12-48V
- Dòng input max: 20 A
- Điện áp output: 12/24V (tùy chọn)
- Dòng output max: 20 A
- Hiệu suất: 95-98%
- Nhiệt độ hoạt động: -20°C đến +60°C
- Kích thước: ~15 × 10 × 5 cm
- Chi phí: 1.5-2M VND
```

**So sánh: PWM vs MPPT**

| Tham số | PWM | MPPT |
|--------|-----|------|
| Hiệu suất | 60-75% | 95-98% |
| Sạc nhanh | Chậm | Nhanh hơn 20-40% |
| Kích thước panel | Nhỏ | Có thể lớn hơn |
| Chi phí | 500K | 1.5-2M |
| Khuyến cáo | Lab/demo | Production |

---

## 📐 Công Thức Tính Toán Hệ Thống

### **1. Tính Dung Lượng Pin Cần Thiết**

```
Công thức:
Battery_Capacity (Wh) = Daily_Consumption (Wh) × Days_Autonomy / DOD

Trong đó:
- Daily_Consumption: Tiêu thụ năng lượng 1 ngày (Wh)
- Days_Autonomy: Số ngày hoạt động mà không có mặt trời (thường 3-7 ngày)
- DOD (Depth of Discharge): Tỷ lệ phóng pin an toàn
  * LiFePO₄: 90-95% DOD
  * Li-ion: 80-90% DOD

Ví dụ với 42 Wh/ngày, 5 ngày autonomy, LiFePO₄ 90% DOD:
Battery = 42 × 5 / 0.90 = 233 Wh
→ Chọn pin 256 Wh (20 Ah @ 12.8V) hoặc 128 Wh (10 Ah)
```

### **2. Tính Dung Lượng Panel Solar Cần Thiết**

```
Công thức:
Panel_Power (W) = (Daily_Consumption (Wh) + Charging_Loss) × 1.25 / PSH

Trong đó:
- Daily_Consumption: 42-83 Wh (tùy chế độ)
- Charging_Loss: 5-10% (tổn thất MPPT + dây dẫn)
- PSH (Peak Sun Hours): Giờ nắng trực tiếp/ngày
  * Vùng miền núi Bắc: 3-4 giờ (mưa nhiều)
  * Vùng miền xuôi: 4-5 giờ
  * Vùng Nam Trung Bộ: 4-5 giờ
- 1.25: Hệ số an toàn (nhiệt độ, góc đặt, bụi)

Ví dụ với 42 Wh/ngày, PSH = 4 giờ, vùng miền xuôi:
Panel = (42 + 5) × 1.25 / 4 = 14.7 W
→ Chọn panel 15W (phù hợp!)

Ví dụ với 83 Wh/ngày (khi có backlight):
Panel = (83 + 8) × 1.25 / 4 = 28.4 W
→ Chọn panel 30W hoặc 2× 15W panel nối song song
```

### **3. Tính Dòng Sạc Pin từ Panel**

```
Công thức:
Charging_Current (A) = Panel_Power (W) / Battery_Voltage (V)

Ví dụ với panel 15W, pin 12V:
I_charge = 15 / 12 = 1.25 A

Tại điểm công suất tối đa:
I_charge = 15W / 18V = 0.83 A (điều kiện tối ưu)
```

### **4. Tính Thời Gian Sạc Pin Đầy**

```
Công thức (bỏ qua các tổn thất):
Charging_Time (h) = Battery_Capacity (Ah) / Charging_Current (A)

Ví dụ với pin 10Ah, dòng sạc 0.83 A:
T_charge = 10 / 0.83 = 12 giờ

Thực tế: 14-16 giờ (tính tổn thất + biến thiên điện áp)
```

### **5. Tính Thòi Gian Autonomy (Ngày Không Mặt Trời)**

```
Công thức:
Days_Autonomy = (Battery_Capacity_Wh × DOD) / Daily_Consumption_Wh

Ví dụ:
- Pin: 128 Wh (10 Ah @ 12.8V), LiFePO₄ 90% DOD
- Tiêu thụ: 42 Wh/ngày (chế độ bình thường)
Days = (128 × 0.90) / 42 = 2.74 ngày → ~3 ngày

- Tiêu thụ: 83 Wh/ngày (với backlight)
Days = (128 × 0.90) / 83 = 1.38 ngày → ~1 ngày
```

### **6. Tính Dây Dẫn & Fuse**

```
Công thức dây dẫn:
AWG = log(2 × L × I / (Ω × A)) × (-0.11268) + 0.128

Đơn giản hơn, dùng bảng:

Từ Solar Panel (1.25 A @ ~15V, 4m):
- AWG 10 (5.26 mm²): LoD 3% → OK
- Fuse: 15A

Từ Pin tới Hệ thống (0.9 A @ 12V, 10m):
- AWG 6 (13.3 mm²): LoD 3% → OK
- Fuse: 5A

Từ DC-DC (500mA output, 1m):
- AWG 8 (8.37 mm²) → OK
```

---

## 📏 Kích Thước & Dung Lượng Cần Thiết

### **Phiên Bản Demo/Lab (Phòng học)**

```
Pin: 2× Li-ion 18650 (3000 mAh @ 7.4V) = 22.2 Wh
  → Thích hợp: 1 ngày demo với solar 5W
  → Không phù hợp: Hoạt động độc lập lâu dài

Panel: 5W Monocrystalline
  → 4 giờ nắng = 20 Wh → chỉ đủ demo 1 ngày
  → Chậm, không thực tế
  
MPPT: PWM Controller 12V/10A
  → Đơn giản, chi phí thấp (500K VND)

Thích hợp cho: Bộ sưu tập, lab, giảng dạy
```

---

### **Phiên Bản Tiêu Chuẩn (Triển Khai Thực Tế) ✅**

```
Pin: LiFePO₄ 12V 10Ah (128 Wh)
  → Thích hợp: 3 ngày chế độ bình thường, 5-7 ngày nếu tiết kiệm
  → Tuổi thọ: 8-10 năm
  → Chi phí: 1.5-2M VND

Panel: 15W Monocrystalline
  → 4 giờ nắng = 48 Wh (hiệu suất 80%)
  → Có thể sạc pin 10Ah trong ~12 giờ (tối ưu)
  → Chi phí: 500-700K VND

MPPT: 12V/20A MPPT Controller
  → Hiệu suất 95-98%
  → Sạc nhanh hơn 20-40% so với PWM
  → Chi phí: 1.5-2M VND

**Tổng chi phí: 4-5M VND**
**Autonomy: 5-7 ngày (không mặt trời)**

Thích hợp cho: Triển khai làng nông thôn, thử nghiệm thực địa
```

---

### **Phiên Bản Nâng Cao (Production/Quy Mô Lớn)**

```
Pin: LiFePO₄ 12V 20Ah (256 Wh)
  → Autonomy: 10+ ngày chế độ bình thường
  → Phù hợp: Vùng thường xuyên mưa, ít nắng
  → Chi phí: 2.5-3.5M VND

Panel: 45W Monocrystalline (hoặc 2× 15W)
  → 4 giờ nắng = 144 Wh (hiệu suất 80%)
  → Sạc pin 20Ah trong ~6 giờ (tối ưu)
  → Chi phí: 1.5-2M VND

MPPT: 12V/40A MPPT Controller
  → Cho phép mở rộng thêm panel
  → Chi phí: 2-2.5M VND

BMS (Battery Management System):
  → Bảo vệ pin, cân bằng cell, khóa phóng quá dòng
  → Chi phí: 500K VND
  
**Tổng chi phí: 7-8.5M VND**
**Autonomy: 10-14 ngày (không mặt trời)**

Thích hợp cho: Mạng lưới cảnh báo quy mô, các khu vực bao quát lớn
```

---

## 📊 Bảng Tính Tóm Tắt

### **So Sánh 3 Phiên Bản**

| Yếu Tố | DEMO/LAB | TIÊU CHUẨN ✅ | NÂNG CAO |
|--------|----------|------------|---------|
| **PIN** |
| Loại | Li-ion 18650 | LiFePO₄ 12V 10Ah | LiFePO₄ 12V 20Ah |
| Dung lượng | 22.2 Wh | 128 Wh | 256 Wh |
| Chi phí | 100K | 1.5-2M | 2.5-3.5M |
| **PANEL SOLAR** |
| Công suất | 5W | 15W | 45W |
| Năng lượng/ngày (4h) | 16 Wh | 48 Wh | 144 Wh |
| Chi phí | 200K | 500-700K | 1.5-2M |
| **MPPT** |
| Loại | PWM 10A | MPPT 20A | MPPT 40A |
| Hiệu suất | 70% | 96% | 96% |
| Chi phí | 500K | 1.5-2M | 2-2.5M |
| **TỔNG CHI PHÍ** | 800K | 4-5M | 7-8.5M |
| **AUTONOMY (Ngày)** | 0.5 | 3-7 | 10-14 |
| **Phù Hợp** | Lab/Demo | Làng nông | Network |

---

### **Bảng Tiêu Thụ Năng Lượng Chi Tiết**

| Chế Độ | Tiêu Thụ/Ngày | Pin 128 Wh | Ngày Autonomy | Ghi Chú |
|--------|-----------------|-----------|----------------|---------|
| Bình thường (no backlight) | 42 Wh | 128 Wh | 3.0 | Hoạt động 24/7 |
| Bình thường + backlight 20h | 83 Wh | 128 Wh | 1.5 | Hiển thị liên tục |
| Có SMS (3-5 lần) | 42.3 Wh | 128 Wh | 3.0 | Khi có cảnh báo |
| Cảnh báo liên tục | 62 Wh | 128 Wh | 2.0 | Ngập lụt kéo dài |
| Sạc từ panel 15W | +48 Wh/ngày | - | **∞** | 4h nắng trực tiếp |

---

### **Điểm Ngưỡng Cảnh Báo Điện Áp**

Theo code Arduino, hệ thống giám sát điện áp pin liên tục:

| Trạng thái | Điện áp | Hành động | Ghi chú |
|----------|---------|----------|---------|
| Bình thường | ≥ 11.5V | Hoạt động toàn bộ | Tất cả module sử dụng |
| Cảnh báo nhẹ | 10.5-11.5V | Giảm backlight | Gửi SMS cảnh báo pin yếu |
| Cảnh báo nặng | 9.5-10.5V | Tắt LCD backlight | Chỉ giữ RTC + cảm biến |
| **CUTOFF** | **< 9.0V** | **Hibernate** | **Chỉ RTC hoạt động (1mA)** |

**Công thức ADC Arduino:**
```
Pin A0: Voltage divider R1=10kΩ, R2=2.2kΩ
Vout = Vin × (R2/(R1+R2)) = Vin × 0.18

Đọc ADC:
ADC_Value = analogRead(A0);  // 0-1023

Điện áp pin:
V_battery = (ADC_Value × 5.0 / 1023) / 0.18
          = ADC_Value × 0.0271 V/unit

Ví dụ:
- 12V pin: ADC ≈ 442 (thực: 442 × 0.0271 = 11.98V ✓)
- 10.5V pin: ADC ≈ 387
- 9.0V pin: ADC ≈ 332
```

---

## 🔧 Hướng Dẫn Chọn Hệ Thống

### **Chọn theo Khoảng Cách và Điều Kiện Thời Tiết**

**Khu vực có nắng ổn định (miền xuôi, 4-5 giờ nắng/ngày):**
```
→ Panel 15W + Pin 10Ah + MPPT 20A = Phiên bản Tiêu Chuẩn
→ Chi phí: 4-5M VND
→ Autonomy: 5-7 ngày
```

**Khu vực thường mưa nhiều (miền núi, 2-3 giờ nắng/ngày):**
```
→ Panel 30-45W + Pin 20Ah + MPPT 30-40A = Phiên bản Nâng Cao
→ Chi phí: 7-9M VND
→ Autonomy: 10+ ngày
```

**Mục đích Demo/Lab:**
```
→ Pin 18650 + Panel 5-10W + PWM 10A = Phiên bản Demo
→ Chi phí: 1M VND
→ Autonomy: 1-2 ngày
```

---

## 📞 Liên Hệ & Hỗ Trợ

**Dự án:** PRJ04 - DIY Hệ Thống Cảnh Báo Ngập Lụt  
**Tổ chức:** BLKLab / Samsung Solve for Tomorrow  
**Ngôn ngữ:** Tiếng Việt  
**Cập nhật:** 2026-06-20  

**Tài liệu liên quan:**
- [SOLAR_POWER_PROPOSAL.md](SOLAR_POWER_PROPOSAL.md)
- [FEASIBILITY_ASSESSMENT.md](FEASIBILITY_ASSESSMENT.md)
- [SIMPLIFIED_BUDGET.md](SIMPLIFIED_BUDGET.md)
- [BLKLab_Code_DIY_He_Thong_Canh_Bao_Ngap_Ung_Lu_Lut.ino](BLKLab_Tai_Lieu_Code/BLKLab_Code/BLKLab_Code_DIY_He_Thong_Canh_Bao_Ngap_Ung_Lu_Lut.ino)

---

**Phiên bản:** 1.0  
**Trạng thái:** Hoàn thành ✅  
**Định dạng:** Markdown (.md)  
**Mục đích sử dụng:** Thiết kế hệ thống năng lượng mặt trời
