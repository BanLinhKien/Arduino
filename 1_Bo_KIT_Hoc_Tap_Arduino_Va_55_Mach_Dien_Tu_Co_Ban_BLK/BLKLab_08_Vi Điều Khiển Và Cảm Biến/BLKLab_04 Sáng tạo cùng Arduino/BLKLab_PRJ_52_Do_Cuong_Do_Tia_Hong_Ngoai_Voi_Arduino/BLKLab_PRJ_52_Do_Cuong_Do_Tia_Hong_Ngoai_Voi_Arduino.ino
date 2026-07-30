/*=================================================================================================
 * BANLINHKIEN.COM                                            *
 ==================================================================================================
*/

const int flamePin = A0;   // Chân Analog đọc cảm biến lửa
const int ledPin = 9;      // Chân Digital điều khiển LED
const int buzzerPin = 8;   // Chân Digital điều khiển Còi xung

int flameValue = 0;

// Các ngưỡng giá trị (Cần điều chỉnh tùy loại cảm biến của bạn)
const int Threshold_Danger = 100;  // Lửa rất gần (Giá trị thấp = Lửa mạnh)
const int Threshold_Warning = 600; // Có dấu hiệu tia hồng ngoại/lửa xa

void setup() {
  Serial.begin(9600);
  
  pinMode(flamePin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  Serial.println("He thong canh bao lua dang san sang...");
  
  // Test còi và led khi mới khởi động (Kêu một tiếng bip bằng hàm tone)
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 1000, 300); 
  delay(500);
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
}

void loop() {
  flameValue = analogRead(flamePin);

  Serial.print("Gia tri cam bien: ");
  Serial.print(flameValue);

  // LOGIC: Gia tri Analog cang THAP thi lua cang MANH
  if (flameValue < Threshold_Danger) {
    // ====================================================
    // MUC NGUY HIEM: HU COI SIREN (Tương tự dự án Gas)
    // ====================================================
    Serial.println(" => [NGUY HIEM] CO LUA LON!");
    
    // --- PHA XUỐNG: Hú từ cao xuống thấp ---
    for (int freq = 1300; freq >= 500; freq -= 5) {
      tone(buzzerPin, freq);
      
      // Nháy LED nhanh theo tần số âm thanh
      if (freq % 100 < 5) digitalWrite(ledPin, !digitalRead(ledPin));
      
      delay(2); 
    }

    noTone(buzzerPin); 
    digitalWrite(ledPin, LOW); // Đảm bảo tắt LED lúc nghỉ
    delay(50); 
  } 
  else if (flameValue < Threshold_Warning) {
    // MUC CANH BAO: Bip... Bip... ngắt quãng
    Serial.println(" => [CANH BAO] Phat hien ngon lua!");
    
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 800);  // Phát tiếng bip tần số 800Hz
    delay(100);            // Nháy nhanh hơn gas để cảnh báo lửa gấp
    
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
    delay(100);
  } 
  else {
    // AN TOAN
    Serial.println(" => [BINH THUONG]");
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin); 
  }

  // Tốc độ phản hồi nhanh để bắt kịp tia lửa
  delay(100); 
}