/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

const int gasPin = A0;     // Chân Analog đọc cảm biến gas
const int ledPin = 9;      // Chân Digital điều khiển LED
const int buzzerPin = 8;   // Chân Digital điều khiển Còi xung

int gasValue = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(gasPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  Serial.println("He thong dang khoi dong...");
  
  // Test còi và led (Kêu một tiếng bip ngắn bằng hàm tone)
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 1000, 200); // Tần số 1000Hz trong 200ms
  delay(500);
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
}

void loop() {
  gasValue = analogRead(gasPin);

  Serial.print("Nong do Gas: ");
  Serial.print(gasValue);

  if (gasValue > 400) {
    // ====================================================
    // MUC NGUY HIEM: HU COI TU CAO XUONG THAP
    // ====================================================
    Serial.println(" => [NGUY HIEM] DANG HU COI!");
    
    // --- PHA XUỐNG: Hú từ cao xuống thấp ---
    for (int freq = 1300; freq >= 500; freq -= 5) {
      tone(buzzerPin, freq);
      
      // Nháy LED theo tần số âm thanh để tạo hiệu ứng cảnh báo mạnh
      if (freq % 100 < 5) digitalWrite(ledPin, !digitalRead(ledPin));
      
      delay(2); 
    }

    noTone(buzzerPin);          // Tắt âm thanh sau mỗi chu kỳ hú
    digitalWrite(ledPin, LOW);  // Đảm bảo tắt đèn lúc nghỉ
    delay(50); 
  } 
  else if (gasValue > 50) {
    // MUC CANH BAO: Bip... Bip... ngắt quãng
    Serial.println(" => [CANH BAO] Phat hien mui gas!");
    
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 800);  // Phát tiếng bip tần số 800Hz
    delay(200);
    
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);     // Tắt còi
    delay(200);
  } 
  else {
    // AN TOAN
    Serial.println(" => [AN TOAN]");
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin); 
  }

  // Lưu ý: delay(500) ở cuối loop gốc có thể làm chậm phản ứng của vòng lặp for hú còi
  // Tôi giảm xuống còn 100 để hệ thống nhạy hơn
  delay(100); 
}