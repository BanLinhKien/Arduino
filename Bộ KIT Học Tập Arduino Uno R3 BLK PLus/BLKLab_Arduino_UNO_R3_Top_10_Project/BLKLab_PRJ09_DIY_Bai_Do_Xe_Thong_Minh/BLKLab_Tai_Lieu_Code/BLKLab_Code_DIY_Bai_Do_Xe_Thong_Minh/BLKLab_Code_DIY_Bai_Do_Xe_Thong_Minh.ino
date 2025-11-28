
/* =================================================================================================*
 *                                          BANLINHKIEN.COM                                         *
 *==================================================================================================*                        
 *                                PRJ09: DIY Bãi Đỗ Xe Thông Minh
 * 
 * Đấu nối: 
 * Arduino UNO R3 vs LCD 16x2 I2C
 * 5V <-> VCC  ;   GND <-> GND  ;   A4 <-> SDA  ;   A5 <-> SCL 
 *
 * Arduino UNO R3 vs Module RC522 (RFID)
 * 3.3V <-> VCC  ;   GND <-> GND
 * D10 <-> SDA   ;   D13 <-> SCK  ;   D11 <-> MOSI  ;   D12 <-> MISO  ;   D9  <-> RST
 *
 * Arduino UNO R3 vs Động cơ servo 
 * 5V <-> Dây đỏ  ;   GND <-> Dây đen  ;   D8 <-> Dây Vàng
 * 
 * Arduino UNO R3 vs Module Cảm biến phát hiện lửa
 * 5V <-> VCC  ;   GND <-> GND  ;   D4 <-> D0
 * 
 * Arduino UNO R3 vs Module Cảm Biến Khí Gas MQ2
 * 5V <-> VCC  ;   GND <-> GND  ;   D3 <-> D0
 */

// --- KHAI BÁO CÁC THƯ VIỆN CẦN THIẾT ---
#include <Wire.h>               // Thư viện giao tiếp I2C (dùng cho màn hình LCD và module thời gian RTC)
#include <RTClib.h>             // Thư viện xử lý thời gian thực cho module DS1307 hoặc DS3231
#include <SPI.h>                // Thư viện giao tiếp chuẩn SPI (dùng cho module RFID)
#include <MFRC522.h>            // Thư viện điều khiển module đọc thẻ từ RFID RC522
#include <LiquidCrystal_I2C.h>  // Thư viện điều khiển màn hình LCD qua giao tiếp I2C
#include <Servo.h>              // Thư viện điều khiển động cơ Servo (đóng/mở barrier)
#include <EEPROM.h>             // Thư viện đọc/ghi bộ nhớ EEPROM nội của vi điều khiển

// ==========================================================================================
//  PHẦN 1: CẤU HÌNH HỆ THỐNG VÀ ĐỊNH NGHĨA CHÂN KẾT NỐI
// ==========================================================================================

// --- Cấu hình tham số hệ thống ---
#define MASTER_UID        0x4B9C0705  // Mã UID của thẻ Admin (Thẻ chủ dùng để thêm/xóa VIP và Reset)
#define EEPROM_SIGNATURE  0xC0DE      // Chữ ký đặc biệt để kiểm tra xem bộ nhớ đã được khởi tạo chưa

const int MAX_CARS      = 4;          // Tổng số chỗ đỗ xe tối đa của bãi
const int MAX_VIP_SIZE  = 2;          // Số lượng thẻ VIP tối đa được phép đăng ký
const long FEE_PER_UNIT = 50;         // Đơn giá gửi xe (50 VNĐ cho mỗi giây )
const unsigned long BARRIER_DELAY = 3000; // Thời gian giữ barrier mở (3000ms = 3 giây)

// --- Định nghĩa chân kết nối (Pin Mapping) ---
#define PIN_RFID_SS     10            // Chân SS (SDA) của module RFID
#define PIN_RFID_RST    9             // Chân RST của module RFID
#define PIN_SERVO       8             // Chân tín hiệu điều khiển Servo
#define PIN_LED_GREEN   7             // Chân đèn LED Xanh (Báo thành công/Mở cổng)
#define PIN_LED_RED     6             // Chân đèn LED Đỏ (Báo lỗi/Thất bại)
#define PIN_BUZZER      5             // Chân còi chip (Buzzer) báo âm thanh
#define PIN_FLAME       4             // Chân cảm biến lửa (Báo cháy)
#define PIN_MQ2         3             // Chân cảm biến khí Gas/Khói
#define PIN_RESET_BTN   A0            // Chân nút nhấn cứng (Dùng để Reset dữ liệu gốc)

// --- Khởi tạo các đối tượng điều khiển ---
RTC_DS1307 rtc;                       // Đối tượng quản lý thời gian thực
MFRC522 mfrc522(PIN_RFID_SS, PIN_RFID_RST); // Đối tượng quản lý đầu đọc thẻ RFID
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Đối tượng màn hình LCD 
Servo barrierServo;                   // Đối tượng điều khiển thanh chắn barrier

// ==========================================================================================
//  PHẦN 2: BIẾN TOÀN CỤC (LƯU TRỮ TRẠNG THÁI HỆ THỐNG)
// ==========================================================================================

uint32_t parkedUIDs[MAX_CARS];        // Mảng lưu UID của các xe đang gửi trong bãi
uint32_t parkedTimestamps[MAX_CARS];  // Mảng lưu thời gian (Unix time) lúc xe vào
int parkedCount = 0;                  // Biến đếm tổng số xe đang có trong bãi
int vipParkedCount = 0;               // Biến đếm số lượng xe VIP đang gửi

uint32_t vipList[MAX_VIP_SIZE];       // Mảng danh sách các thẻ VIP đã đăng ký
int currentVipCount = 0;              // Số lượng thẻ VIP hiện có trong danh sách

bool isGasWarningActive = false;      // Cờ báo hiệu trạng thái cảnh báo khí Gas (True = Đang báo động)

// ==========================================================================================
//  PHẦN 3: CÁC HÀM HỖ TRỢ HIỂN THỊ VÀ ĐIỀU KHIỂN CƠ BẢN
// ==========================================================================================

// Hàm in số có 2 chữ số (ví dụ: 5 -> 05) để hiển thị giờ đẹp hơn
void printTwoDigits(int number) {
    if (number < 10) lcd.print("0");
    lcd.print(number);
}

// Hàm hiển thị thông báo lên màn hình LCD (có thể xóa màn hình và chờ)
void showMessage(const char* line1, const char* line2, int delayMs) {
    lcd.clear();                      // Xóa nội dung cũ
    lcd.setCursor(0, 0); lcd.print(line1); // In dòng 1
    lcd.setCursor(0, 1); lcd.print(line2); // In dòng 2
    if (delayMs > 0) delay(delayMs);  // Chờ một khoảng thời gian nếu cần
}

// Hàm tạo tiếng bíp từ còi chip
void beep(int times) {
    for (int i = 0; i < times; i++) {
        digitalWrite(PIN_BUZZER, HIGH); delay(100); // Bật còi
        digitalWrite(PIN_BUZZER, LOW);  delay(100); // Tắt còi
    }
}

// Hàm điều khiển đóng mở cổng Barrier
void controlBarrier(bool success) {
    if (success) {
        // Trường hợp thành công: Mở cổng
        digitalWrite(PIN_LED_GREEN, HIGH); 
        digitalWrite(PIN_LED_RED, LOW);
        
        barrierServo.attach(PIN_SERVO); // Kết nối servo
        barrierServo.write(90);         // Góc 90 độ: Mở cổng lên
        delay(BARRIER_DELAY);           // Giữ cổng mở trong thời gian quy định
        barrierServo.write(0);          // Góc 0 độ: Đóng cổng xuống
        delay(500);                     // Chờ servo chạy xong
        barrierServo.detach();          // Ngắt kết nối để tránh rung servo
        
        digitalWrite(PIN_LED_GREEN, LOW);
    } else {
        // Trường hợp thất bại: Báo lỗi
        for (int i = 0; i < 3; i++) {
            digitalWrite(PIN_LED_RED, HIGH); beep(1); 
            digitalWrite(PIN_LED_RED, LOW); delay(100);
        }
    }
}

// Hàm cập nhật màn hình trạng thái chờ (Hiển thị số chỗ trống)
void updateIdleScreen() {
    lcd.clear();
    // Dòng 1: Hiển thị chỗ trống cho xe thường
    lcd.setCursor(0, 0); lcd.print("Slot Thuong:"); 
    lcd.print(parkedCount - vipParkedCount); 
    lcd.print("/"); 
    lcd.print(MAX_CARS - currentVipCount);
    
    // Dòng 2: Hiển thị trạng thái khu vực VIP
    lcd.setCursor(0, 1); lcd.print("Slot VIP   :"); 
    if (currentVipCount == 0) {
        lcd.print("0/0"); // Chưa có VIP nào đăng ký
    } else {
        lcd.print(vipParkedCount); lcd.print("/"); lcd.print(currentVipCount);
    }
}

// ==========================================================================================
//  PHẦN 4: QUẢN LÝ BỘ NHỚ EEPROM (LƯU DỮ LIỆU KHI MẤT ĐIỆN)
// ==========================================================================================

// Hàm lưu toàn bộ dữ liệu quan trọng vào EEPROM
void saveData() {
    int addr = 0;
    // Sử dụng hàm put để tự động tính toán kích thước kiểu dữ liệu
    EEPROM.put(addr, EEPROM_SIGNATURE);   addr += sizeof(int); // Lưu chữ ký
    EEPROM.put(addr, parkedCount);        addr += sizeof(int); // Lưu số lượng xe
    EEPROM.put(addr, vipParkedCount);     addr += sizeof(int); // Lưu số xe VIP
    EEPROM.put(addr, parkedUIDs);         addr += sizeof(parkedUIDs); // Lưu mảng UID xe
    EEPROM.put(addr, parkedTimestamps);   addr += sizeof(parkedTimestamps); // Lưu mảng thời gian
    EEPROM.put(addr, currentVipCount);    addr += sizeof(int); // Lưu số lượng thẻ VIP
    EEPROM.put(addr, vipList);            // Lưu danh sách thẻ VIP
}

// Hàm tải dữ liệu từ EEPROM khi khởi động lại
void loadData() {
    int addr = 0, signature;
    EEPROM.get(addr, signature); addr += sizeof(int);
    
    // Kiểm tra chữ ký: Nếu khác nhau tức là hệ thống mới lắp hoặc chưa có dữ liệu -> Reset về 0
    if (signature != EEPROM_SIGNATURE) {
        parkedCount = 0; vipParkedCount = 0; currentVipCount = 0;
        memset(parkedUIDs, 0, sizeof(parkedUIDs));
        memset(parkedTimestamps, 0, sizeof(parkedTimestamps));
        memset(vipList, 0, sizeof(vipList));
        saveData(); // Ghi đè dữ liệu trắng vào
    } else {
        // Nếu chữ ký đúng, tiến hành đọc dữ liệu cũ ra
        EEPROM.get(addr, parkedCount);        addr += sizeof(int);
        EEPROM.get(addr, vipParkedCount);     addr += sizeof(int);
        EEPROM.get(addr, parkedUIDs);         addr += sizeof(parkedUIDs);
        EEPROM.get(addr, parkedTimestamps);   addr += sizeof(parkedTimestamps);
        EEPROM.get(addr, currentVipCount);    addr += sizeof(int);
        EEPROM.get(addr, vipList);
        
        // Kiểm tra an toàn: Nếu dữ liệu đọc ra bị sai lệch logic , reset lại danh sách VIP
        if (currentVipCount < 0 || currentVipCount > MAX_VIP_SIZE || currentVipCount > MAX_CARS) {
            currentVipCount = 0; memset(vipList, 0, sizeof(vipList));
        }
    }
}

// ==========================================================================================
//  PHẦN 5: LOGIC NGHIỆP VỤ & QUẢN TRỊ (PHẦN QUAN TRỌNG NHẤT)
// ==========================================================================================

// Kiểm tra xem một UID có phải là thẻ VIP hay không
bool isVipCard(uint32_t uid) {
    if (uid == MASTER_UID) return true; // Thẻ Admin cũng được coi là quyền cao nhất
    for (int i = 0; i < MAX_VIP_SIZE; i++) {
        if (vipList[i] == uid) return true;
    }
    return false;
}

// Kiểm tra điều kiện có thể thêm VIP mới không
// Trả về: 0 (OK); 1 (Đầy bộ nhớ VIP); 2 (Không còn slot trong bãi)
int checkVipAddStatus() {
    if (currentVipCount >= MAX_VIP_SIZE) return 1; // Lỗi: Đã đủ số lượng VIP tối đa
    // Logic: Nếu thêm 1 VIP thì tổng số xe hiện tại + số VIP mới phải nhỏ hơn tổng chỗ của bãi
    // Ngăn chặn việc xe thường đang chiếm chỗ khiến VIP mới đăng ký không có chỗ đỗ
    if ((parkedCount - vipParkedCount) + (currentVipCount + 1) > MAX_CARS) return 2; 
    return 0;
}

// --- HÀM KIỂM TRA XE CÓ ĐANG GỬI KHÔNG ---
// Mục đích: Ngăn chặn việc thay đổi quyền (Thêm/Xóa VIP) khi xe đang nằm trong bãi
bool isCarCurrentlyParked(uint32_t checkUID) {
    for (int i = 0; i < MAX_CARS; i++) {
        // Chỉ kiểm tra những vị trí có dữ liệu (UID khác 0)
        if (parkedUIDs[i] != 0 && parkedUIDs[i] == checkUID) {
            return true; // Tìm thấy xe trong bãi
        }
    }
    return false; // Không tìm thấy
}

// --- HÀM XỬ LÝ THÊM THẺ VIP AN TOÀN ---
void tryAddVip(uint32_t newUID) {
    // Bước 1: Kiểm tra xe đang gửi -> Cấm đổi trạng thái để tránh lỗi tính tiền
    if (isCarCurrentlyParked(newUID)) {
        showMessage("LOI: XE DANG GUI", "RA KHOI BAI TRUOC",0);
        beep(3);
        return;
    }

    // Bước 2: Kiểm tra các điều kiện logic khác (đầy bộ nhớ, hết chỗ)
    int status = checkVipAddStatus();
    if (status == 0) {
        vipList[currentVipCount] = newUID; // Thêm vào danh sách
        currentVipCount++;
        saveData(); // Lưu ngay vào EEPROM
        digitalWrite(PIN_LED_GREEN, HIGH); beep(1);
        showMessage("DA THEM VIP", "THANH CONG", 1000); 
        digitalWrite(PIN_LED_GREEN, LOW);  
    } else {
        // Báo lỗi tương ứng
        if (status == 1) showMessage("LOI: BO NHO FULL", "GIOI HAN VIP!", 0);
        else if (status == 2) showMessage("LOI: KHONG THEM", "XE THUONG CHIEM!", 0);
        beep(3);
    }
}

// --- HÀM XỬ LÝ XÓA THẺ VIP AN TOÀN ---
void tryRemoveVip(int indexToRemove, uint32_t uidToRemove) {
    // Bước 1: Kiểm tra xe đang gửi -> Cấm xóa VIP khi xe chưa ra bãi
    if (isCarCurrentlyParked(uidToRemove)) {
        showMessage("LOI: XE DANG GUI", "RA KHOI BAI TRUOC", 2000);
        beep(3);
        return;
    }

    // Bước 2: Xóa khỏi mảng và dồn danh sách lên (thuật toán xóa phần tử mảng)
    vipList[indexToRemove] = 0; 
    for (int j = indexToRemove; j < MAX_VIP_SIZE - 1; j++) vipList[j] = vipList[j+1];
    vipList[MAX_VIP_SIZE-1] = 0; // Xóa phần tử cuối cùng
    currentVipCount--;
    
    saveData(); // Cập nhật EEPROM
    digitalWrite(PIN_LED_RED, HIGH); beep(1); 
    showMessage("DA XOA VIP", "THANH CONG", 1000);
    digitalWrite(PIN_LED_RED, LOW);   
}

// --- CHẾ ĐỘ QUẢN TRỊ VIÊN (ADMIN MODE) ---
void handleMasterMode() {
    showMessage("ADMIN MODE", "NUT: RESET ALL", 0); 
    beep(3);

    while (true) {
        // Chức năng 1: Reset toàn bộ dữ liệu bằng nút bấm cứng
        if (digitalRead(PIN_RESET_BTN) == LOW) {
            delay(50); // Chống rung phím
            if (digitalRead(PIN_RESET_BTN) == LOW) {
                showMessage("DANG RESET...", " ", 1000);
                // Xóa sạch các biến đếm và mảng
                parkedCount = 0; vipParkedCount = 0; currentVipCount = 0;
                memset(parkedUIDs, 0, sizeof(parkedUIDs));
                memset(parkedTimestamps, 0, sizeof(parkedTimestamps));
                memset(vipList, 0, sizeof(vipList));
                saveData(); // Lưu trạng thái trống vào EEPROM
                showMessage("DA XOA DU LIEU", "HE THONG SACH SE", 1000);
                while(digitalRead(PIN_RESET_BTN) == LOW); // Chờ nhả nút
                return;
            }
        }

        // Chức năng 2: Quẹt thẻ để quản lý (Thêm/Xóa VIP hoặc Thoát)
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
            uint32_t newUID = 0;
            // Chuyển đổi UID từ dạng byte sang số nguyên 32 bit
            for (byte i = 0; i < 4; i++) newUID = (newUID << 8) | mfrc522.uid.uidByte[i];
            mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1();

            // Nếu quẹt lại thẻ Admin -> Thoát chế độ Admin
            if (newUID == MASTER_UID) {
                showMessage("ADMIN MODE", "KET THUC!", 0);
                digitalWrite(PIN_LED_RED, HIGH); beep(3); digitalWrite(PIN_LED_RED, LOW);  
                return; 
            }

            // Kiểm tra xem thẻ vừa quẹt đã là VIP chưa
            int foundIndex = -1;
            for (int i = 0; i < MAX_VIP_SIZE; i++) {
                if (vipList[i] == newUID) { foundIndex = i; break; }
            }

            // Nếu đã là VIP -> Thực hiện Xóa. Nếu chưa -> Thực hiện Thêm
            if (foundIndex != -1) { 
                tryRemoveVip(foundIndex, newUID);
            } else { 
                tryAddVip(newUID);
            }
            showMessage("ADMIN MODE", "NUT: RESET ALL", 0); // Hiển thị lại menu
        }
    }
}

// ==========================================================================================
//  PHẦN 6: AN TOÀN CHÁY NỔ & QUY TRÌNH ĐỖ XE
// ==========================================================================================

// Hàm kiểm tra an toàn (Cháy và Khí Gas) - Được gọi liên tục trong vòng lặp
void checkFireSafety() {
    // 1. Cảm biến lửa (Ưu tiên cao nhất)
    if (digitalRead(PIN_FLAME) == LOW) { 
        barrierServo.attach(PIN_SERVO);
        barrierServo.write(90); // Mở toang cổng ngay lập tức để thoát hiểm

        // Vòng lặp cảnh báo, chặn mọi hoạt động khác đến khi hết lửa
        while (digitalRead(PIN_FLAME) == LOW) {
            lcd.clear();
            lcd.setCursor(0, 0); lcd.print("!!! CHAY !!!");
            lcd.setCursor(0, 1); lcd.print("DANG MO CONG...");
            digitalWrite(PIN_BUZZER, HIGH); digitalWrite(PIN_LED_RED, HIGH);
            delay(50);
            digitalWrite(PIN_BUZZER, LOW); digitalWrite(PIN_LED_RED, LOW);
            delay(50);
        }

        barrierServo.write(0); delay(500); barrierServo.detach(); // Đóng cổng sau khi an toàn
        isGasWarningActive = false;
        updateIdleScreen(); // Cập nhật lại màn hình chính
    }
    // 2. Cảm biến khí Gas (Cảnh báo mức độ 2)
    else if (digitalRead(PIN_MQ2) == LOW) {
        isGasWarningActive = true;
        lcd.clear();
        lcd.setCursor(0, 0); lcd.print("! CANH BAO GAS !");
        lcd.setCursor(0, 1); lcd.print("NGUY HIEM !!!");
        digitalWrite(PIN_BUZZER, HIGH); delay(200);
        digitalWrite(PIN_BUZZER, LOW);  delay(200);
    }
    // 3. Trạng thái bình thường
    else {
        if (isGasWarningActive) { // Nếu vừa hết báo động Gas thì reset lại màn hình
            isGasWarningActive = false;
            updateIdleScreen();
        }
    }
}

// Hàm đọc thẻ RFID và trả về UID (trả về 0 nếu không có thẻ)
uint32_t getRFID() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return 0;
    uint32_t uid = 0;
    for (byte i = 0; i < 4; i++) uid = (uid << 8) | mfrc522.uid.uidByte[i];
    mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1();
    return uid;
}

// Hàm xử lý chính: Quản lý xe vào/ra
void handleParking(uint32_t rfid) {
    beep(1); // Báo đã nhận thẻ
    
    // Nếu là thẻ Admin -> Vào chế độ quản trị
    if (rfid == MASTER_UID) {
        handleMasterMode();
        updateIdleScreen(); 
        return;
    }

    DateTime now = rtc.now(); // Lấy thời gian hiện tại
    uint32_t currentUnix = now.unixtime();
    
    // Tìm xem xe này đã có trong bãi chưa
    int index = -1;
    for (int i = 0; i < MAX_CARS; i++) if (parkedUIDs[i] == rfid) index = i;
    bool isVIP = isVipCard(rfid); // Kiểm tra quyền VIP

    // --- TRƯỜNG HỢP XE VÀO (Nếu không tìm thấy trong bãi) ---
    if (index == -1) { 
        bool allowEntry = false;
        
        // Logic kiểm tra còn chỗ không
        if (isVIP) {
            // Xe VIP: Chỉ cần tổng số xe chưa đầy là được vào (kể cả chiếm slot thường)
            if (parkedCount < MAX_CARS) allowEntry = true;
        } else {
            // Xe Thường: Phải còn slot và KHÔNG được lấn vào phần slot dự trữ cho VIP
            if ((parkedCount - vipParkedCount) < (MAX_CARS - currentVipCount) && parkedCount < MAX_CARS) {
                allowEntry = true;
            }
        }

        // Từ chối nếu hết chỗ
        if (!allowEntry) {
            showMessage("!!! HET CHO", "VUI LONG CHO", 1000);
            controlBarrier(false); updateIdleScreen(); return;
        }

        // Tìm slot trống đầu tiên để lưu xe
        for (int i = 0; i < MAX_CARS; i++) {
            if (parkedUIDs[i] == 0) { // Slot trống
                parkedUIDs[i] = rfid; // Lưu UID
                parkedTimestamps[i] = currentUnix; // Lưu giờ vào
                parkedCount++;
                if (isVIP) vipParkedCount++;
                
                saveData(); // Lưu EEPROM phòng khi mất điện
                
                // Hiển thị thông tin chào mừng
                lcd.clear(); 
                lcd.setCursor(0,0); lcd.print(isVIP ? "VIP (VAO)" : "XE THUONG (VAO)");
                lcd.setCursor(0,1);
                printTwoDigits(now.hour()); lcd.print(":"); printTwoDigits(now.minute());
                lcd.print(" ");
                printTwoDigits(now.day()); lcd.print("/"); printTwoDigits(now.month());
                
                controlBarrier(true); // Mở cổng
                updateIdleScreen(); return;
            }
        }
    
    // --- TRƯỜNG HỢP XE RA (Nếu đã tìm thấy trong bãi) ---
    } else { 
        unsigned long duration = currentUnix - parkedTimestamps[index]; // Tính thời gian gửi (giây)
        
        // Xóa dữ liệu xe
        parkedUIDs[index] = 0; parkedTimestamps[index] = 0;
        parkedCount--; if (isVIP) vipParkedCount--;
        saveData();

        lcd.clear();
        if (isVIP) {
            // VIP: Miễn phí
            lcd.setCursor(0,0); lcd.print("VIP: MIEN PHI");
            lcd.setCursor(0,1); lcd.print("TG: "); lcd.print(duration / 60.0, 1); lcd.print("p");
        } else {
            // Thường: Tính tiền
            lcd.setCursor(0, 0); lcd.print("TG : "); lcd.print(duration / 60.0, 1); lcd.print(" phut");
            lcd.setCursor(0, 1); lcd.print("PHI: "); lcd.print(duration * FEE_PER_UNIT); lcd.print(" VND");
        }
        
        controlBarrier(true); // Mở cổng cho ra
        updateIdleScreen();
    }
}

// ==========================================================================================
//  PHẦN 7: KHỞI TẠO (SETUP) VÀ VÒNG LẶP CHÍNH (LOOP)
// ==========================================================================================

void setup() {
    Wire.begin(); // Khởi động giao tiếp I2C
    SPI.begin();  // Khởi động giao tiếp SPI
    
    // Thiết lập chế độ cho các chân IO
    pinMode(PIN_RESET_BTN, INPUT_PULLUP); // Nút nhấn dùng trở kéo lên nội bộ
    pinMode(PIN_FLAME, INPUT);            // Cảm biến là đầu vào
    pinMode(PIN_MQ2, INPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);       // Đèn/Còi là đầu ra
    pinMode(PIN_LED_RED, OUTPUT); 
    pinMode(PIN_BUZZER, OUTPUT); 

    // Khởi động màn hình LCD
    lcd.init(); lcd.backlight(); 
    showMessage("HE THONG GIU XE", "--BANLINHKIEN--", 1000);
    
    // Khởi động module thời gian thực RTC
    rtc.begin(); 
    if (!rtc.isrunning()) {
        rtc.adjust(DateTime(__DATE__, __TIME__)); // Cập nhật giờ từ máy tính nếu RTC chưa chạy
    }

    mfrc522.PCD_Init(); // Khởi động module RFID
    
    // Đặt trạng thái ban đầu cho Servo (Đóng cổng)
    barrierServo.attach(PIN_SERVO); 
    barrierServo.write(0); 
    delay(500); 
    barrierServo.detach();

    loadData(); // Tải dữ liệu cũ từ EEPROM lên
    updateIdleScreen(); // Hiển thị màn hình chờ
}

void loop() {
    checkFireSafety(); // Luôn ưu tiên kiểm tra an toàn cháy nổ trước
    
    // Chỉ xử lý quẹt thẻ khi không có báo động khí Gas
    if (!isGasWarningActive) {
        uint32_t uid = getRFID(); 
        if (uid != 0) handleParking(uid); // Nếu có thẻ thì xử lý vào/ra
    }
}
