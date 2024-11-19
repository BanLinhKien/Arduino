/*                                                                                 *
 * ================================================================================*
 *                            BANLINHKIEN.COM                                      * 
 * ================================================================================*                           
 *                                                                                 *
 *          Hướng Dẫn Bật /Tắt thiết bị bằng module cảm biến rung,                 *
 *                  cảm biến ánh sáng, cảm biến âm thanh                           *
 * Đấu nối:                                                                        *
 *-D2: nối chân DO trên cảm biến ánh sáng                                          *
 *-D4: nối chân DO trên cảm biến âm thanh                                          *
 *-D3: nối chân OUT trên cảm biến rung                                             *    
 *-D12: nối chân IN trên Relay                                                     *
 *=================================================================================*/
#define ANH_SANG 2     
#define CB_RUNG 3     
#define AM_THANH 4     
#define RELAY 12        

bool relay_State = false;    // Trạng thái LED
bool buttonState[3] = {false, false, false};              // Trạng thái hiện tại của các CB
bool lastButtonState[3] = {false, false, false};          // Trạng thái trước đó của các CB
unsigned long pressStartTime[3] = {0, 0, 0};              // Thời gian nhấn giữ của từng CB
const unsigned long longPressTime = 3000;                 // Thời gian nhận diện nhấn giữ (ms)

const int buttonPins[3] = {ANH_SANG, CB_RUNG, AM_THANH};

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);                 // Sử dụng điện trở kéo lên (Pull-up)
  }
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    bool currentButtonState = !digitalRead(buttonPins[i]);  // Đọc trạng thái CB (đảo vì dùng PULLUP)

    if (currentButtonState && !lastButtonState[i]) {        
      pressStartTime[i] = millis();
    }

    if (!currentButtonState && lastButtonState[i]) {        
      unsigned long pressDuration = millis() - pressStartTime[i];
      if (pressDuration >= longPressTime) {                 // Xử lý nhấn giữ
        Serial.print("CB ");
        Serial.print(i + 1);
        Serial.println("  giữ!");
      } else {                                              
        relay_State = !relay_State;
        digitalWrite(RELAY, relay_State ? HIGH : LOW);
        Serial.print(relay_State ? "LED Bật " : "LED Tắt ");
        Serial.println(i + 1);
      }
    }
    lastButtonState[i] = currentButtonState;              // Lưu trạng thái để so sánh ở lần lặp tiếp theo
  }
}
