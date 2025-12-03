/*=========================================================================
 *                      BANLINHKIEN.COM                                   *
 *=========================================================================                     
 *          TẠO KÝ TỰ ĐƠN GIẢN HIỂN THỊ TRÊN LED MATRIX 8x8               *
 *=========================================================================
 * ĐẤU NỐI:
 * Arduino  ---  Chân Matrix|   Arduino  ---  Chân Matrix
 * -------------------------|---------------------------
 * D2     -->     24      |     D6     -->     20 hoặc 5
 * D3     -->     23      |     D7     -->     19 hoặc 6
 * D4     -->     22      |     D8     -->     18 hoặc 7
 * D5     -->     21      |     D9     -->     17 hoặc 8
 * A2     -->     4       |     D10    -->     16 hoặc 9
 * A3     -->     3       |     D11    -->     15 hoặc 10
 * A4     -->     2       |     D12    -->     14 hoặc 11
 * A5     -->     1       |     D13    -->     13 hoặc 12
 * -------------------------------------------------
 */          
 
// Các chân điều khiển Hàng 
const int rowPins[8] = {2, 3, 4, 5, A2, A3, A4, A5};  //Tương ứng hàng 1, 2, 3, 4, 5, 6, 7, 8

// Các chân điều khiển Cột 
const int colPins[8] = {6, 7, 8, 9, 10, 11, 12, 13};  //Tương ứng cột 1, 2, 3, 4, 5, 6, 7, 8

// Thời gian duy trì mỗi ký tự (đơn vị: mili giây)
const int displayTime = 500; 

// Mảng chứa phông chữ

const byte uniqueChars[8][8] = {
  
  // Index [0]: Chữ 'B'
  {
    0b00000000, //  . . . . . . . .  
    0b01111000, //  . # # # # . . .  
    0b01000100, //  . # . . . # . .
    0b01000100, //  . # . . . # . .
    0b01111000, //  . # # # # . . .  
    0b01000100, //  . # . . . # . .
    0b01000100, //  . # . . . # . .
    0b01111000  //  . # # # # . . .  
  },
  
  // Index [1]: Chữ 'A'
  {
    0b00000000, //  . . . . . . . .
    0b00111000, //  . . # # # . . .
    0b01000100, //  . # . . . # . .
    0b01000100, //  . # . . . # . .
    0b01111100, //  . # # # # # . .  
    0b01000100, //  . # . . . # . .
    0b01000100, //  . # . . . # . .
    0b01000100  //  . # . . . # . .
  },

  // Index [2]: Chữ 'N'
  {
    0b00000000, //  . . . . . . . .
    0b01000100, //  . # . . . # . .
    0b01000100, //  . # . . . # . .
    0b01100100, //  . # # . . # . .  
    0b01010100, //  . # . # . # . .
    0b01001100, //  . # . . # # . .
    0b01000100, //  . # . . . # . .
    0b01000100  //  . # . . . # . .
  },

  // Index [3]: Chữ 'L'
  {
    0b00000000, //  . . . . . . . .
    0b01000000, //  . # . . . . . .
    0b01000000, //  . # . . . . . .
    0b01000000, //  . # . . . . . .
    0b01000000, //  . # . . . . . .
    0b01000000, //  . # . . . . . .
    0b01000000, //  . # . . . . . .
    0b01111100  //  . # # # # # . .  
  },

  // Index [4]: Chữ 'I'
  {
    0b00000000, //  . . . . . . . .
    0b01111100, //  . # # # # # . .  
    0b00010000, //  . . . # . . . .
    0b00010000, //  . . . # . . . .
    0b00010000, //  . . . # . . . .
    0b00010000, //  . . . # . . . .
    0b00010000, //  . . . # . . . .
    0b01111100  //  . # # # # # . .  
  },

  // Index [5]: Chữ 'H'
  {
    0b00000000, //  . . . . . . . .
    0b01000100, //  . # . . . # . .
    0b01000100, //  . # . . . # . .
    0b01000100, //  . # . . . # . .
    0b01111100, //  . # # # # # . .  
    0b01000100, //  . # . . . # . .
    0b01000100, //  . # . . . # . .
    0b01000100  //  . # . . . # . .
  },

  // Index [6]: Chữ 'K'
  {
    0b00000000, //  . . . . . . . .
    0b01000100, //  . # . . . # . .
    0b01001000, //  . # . . # . . .
    0b01010000, //  . # . # . . . .
    0b01100000, //  . # # . . . . .
    0b01010000, //  . # . # . . . .
    0b01001000, //  . # . . # . . .
    0b01000100  //  . # . . . # . .
  },

  // Index [7]: Chữ 'E'
  {
    0b00000000, //  . . . . . . . .
    0b01111100, //  . # # # # # . .  
    0b01000000, //  . # . . . . . .
    0b01000000, //  . # . . . . . .
    0b01111100, //  . # # # # # . .  
    0b01000000, //  . # . . . . . .
    0b01000000, //  . # . . . . . .
    0b01111100  //  . # # # # # . . 
  }
};

// ---KỊCH BẢN HIỂN THỊ ---
// Mapping: 0=B, 1=A, 2=N, 3=L, 4=I, 5=H, 6=K, 7=E
const int messageSequence[11] = {
  0, 1, 2, 3, 4, 2, 5, 6, 4, 7, 2  // "B A N L I N H K I E N"
};

// =========================================================================
// =========================================================================

void setup() {
  // Khởi tạo toàn bộ chân GPIO
  for (int i = 0; i < 8; i++) {
    pinMode(rowPins[i], OUTPUT);
    pinMode(colPins[i], OUTPUT);
    
    // Trạng thái ban đầu: Tắt toàn bộ LED (Tránh lóe sáng khi khởi động)
    digitalWrite(rowPins[i], LOW);  // Row OFF
    digitalWrite(colPins[i], HIGH); // Col OFF
  }
}

void loop() {
  // Duyệt qua từng ký tự trong chuỗi thông điệp
  for (int i = 0; i < 11; i++) {
    
    int charIndex = messageSequence[i]; // Lấy chỉ số font chữ
    unsigned long startTime = millis(); // Ghi mốc thời gian
    
    // Duy trì hiển thị ảnh trong khoảng thời gian displayTime
    while (millis() - startTime < displayTime) {
      drawScreen(uniqueChars[charIndex]);
    }
  }
}

// -------------------------------------------------------------------------
//Hàm đọc dữ liệu và điều khiển hiển thị
// -------------------------------------------------------------------------
void drawScreen(const byte character[]) {
  // Quét từng hàng (Row scanning)
  for (int i = 0; i < 8; i++) {
    
    // 1. Kích hoạt hàng hiện tại
    digitalWrite(rowPins[i], HIGH); 
    // Bit 1 = Sáng (Kéo lên HIGH), Bit 0 = Tắt (Kéo xuống LOW)
    // 2. Đẩy dữ liệu vào các cột
    for (int j = 0; j < 8; j++) {
      // Kiểm tra bit tương ứng trong byte dữ liệu
      // Bit 1 = Sáng (Kéo cột xuống LOW), Bit 0 = Tắt (Kéo cột lên HIGH)
      if (bitRead(character[i], 7 - j) == 1) {
        digitalWrite(colPins[j], LOW);
      } else {
        digitalWrite(colPins[j], HIGH);
      }
    }

    // 3. Delay để mắt kịp lưu ảnh 
    delay(2); 

    // 4. Tắt hàng hiện tại để chuyển sang hàng kế tiếp
    digitalWrite(rowPins[i], LOW); 
    
    // 5. Chống bóng mờ: Kích toàn bộ cột lên mức HIGH để tránh đèn không tắt hẳn
    for (int j = 0; j < 8; j++) { 
       digitalWrite(colPins[j], HIGH);
    }
  }
}