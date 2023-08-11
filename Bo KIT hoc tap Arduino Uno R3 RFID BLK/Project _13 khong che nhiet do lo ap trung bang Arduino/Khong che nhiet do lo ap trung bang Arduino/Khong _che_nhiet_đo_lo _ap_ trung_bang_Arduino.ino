/*
 * ================================================================================*
 *                            BANLINHKIEN.COM
 * ================================================================================*                           
 * Code: Giao tiếp Keypad 4x4 với Arduino - How to interface Keypad 4x4 with Arduino
 * Đấu nối: 
  -D2: nối với chân cảm biến DHT22
  -D3: nối với chân relay
 */

#include <DHT.h>

const int DHTPIN = 2;                         // Chân kết nối cảm biến DHT22
const int RELAYPIN = 3;                       // Chân kết nối relay

DHT dht(DHTPIN, DHT22);

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
}

void loop() 
{
  float temperature = dht.readTemperature();  // Đọc nhiệt độ từ cảm biến DHT22

  if (!isnan(temperature)) 
  {
    Serial.print("Nhiệt độ: ");
    Serial.println(temperature);

    if (temperature < 37.5)                   // Nhiệt độ thấp hơn ngưỡng, bật relay
    {  
      digitalWrite(RELAYPIN, HIGH);
    } 
    else 
    {
      digitalWrite(RELAYPIN, LOW);            // Nhiệt độ đủ cao, tắt relay
    }
  } 
  else 
  {
    Serial.println("Lỗi đọc cảm biến");
  }
  delay(5000);                                // Đợi 5 giây trước khi đọc nhiệt độ tiếp theo
}
