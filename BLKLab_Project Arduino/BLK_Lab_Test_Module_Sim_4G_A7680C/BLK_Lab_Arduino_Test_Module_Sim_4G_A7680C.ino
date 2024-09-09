/*==================================================================================================================
 *                                                   BANLINHKIEN.COM                                               * 
 *                                  Hướng Dẫn TEST Module SIM 4G A7680X bằng Arduino UNO                           *
 *==================================================================================================================                       
 *LƯU Ý: - Tốc độ baudrate mặc định của module SIM 4G A7680X là 115200                                             *
 *       - Để thay đổi tốc đô baudrate cho module SIM 4G A7680X cần:                                               *
 *            + Bước 1: thay đổi SET_BAUDRATE (dòng 20) về tốc dộ mong muốn (VD:"AT+IPREX=9600")                   *
 *            + Bước 2: nạp code xuống MCU để module SIM 4G A7680X nhận tốc độ baudrate mới.                       *
 *            + Bước 3: thay đổi MCU_SIM_BAUDRATE (dòng 21) bằng với SETBAUDRATE.                                  *           
 *            + Bước 4: nạp code xuống MCU để hoàn tất quá trình thay đổi tốc độ baudrate cho module.              *
 *            + Bước 5: mở Serial Monitor, chọn baudrate bằng với MCU_SIM_BAUDRATEvừa thay đổ.                     *
 *       - Thay đổi số điện thoại gọi đi và nhận tin nhắn ở dòng 19                                                *
 *       - Thẻ SIM được gắn ở Module SIM 4G A7680X và thẻ SIM số điện thoại gọi đi đều phải được đăng ký VoLTE:    *
 *            + Viettel: HDCALL  gửi 191                                                                           *
 *            + Vina:    HD CALL gửi 888                                                                           *
 *==================================================================================================================*/
#include <SoftwareSerial.h>
SoftwareSerial simSerial(10, 11); // TX 10 ; RX 11

#define PHONE_NUMBER      "0393973793"
#define SET_BAUDRATE      "AT+IPREX=115200"         // thay đổi badrate (VD: "AT+IPREX=115200")
#define MCU_SIM_BAUDRATE  115200

void sim_at_wait(){
    delay(100);
    while (simSerial.available()) {
        Serial.write(simSerial.read());
    }
}

bool sim_at_cmd(String cmd){
    simSerial.println(cmd);
    sim_at_wait();
    return true;
}

bool sim_at_send(char c){
    simSerial.write(c);
    return true;
}

void sent_sms() {             // Hàm thực hiện gửi tin nhắn
    sim_at_cmd("AT+CMGF=1");
    String temp = "AT+CMGS=\"";
    temp += (String)PHONE_NUMBER;
    temp += "\"";
    sim_at_cmd(temp);
    sim_at_cmd("Arduino-SIM7680X From balinhkien.com");
    sim_at_send(0x1A);    // End charactor for SMS
}

void call(){                   // Hàm thực hiện cuộc gọi
    String temp = "ATD";
    temp += PHONE_NUMBER;
    temp += ";";
    sim_at_cmd(temp); 

    delay(20000);
    sim_at_cmd("ATH"); 
}

void setup() {
    Serial.begin(MCU_SIM_BAUDRATE);
    delay(200);
    simSerial.begin(MCU_SIM_BAUDRATE);
    delay(200);
    
    Serial.println("\n\n\n\n-----------------------\nSystem started!!!!");
    
    
    sim_at_cmd("AT");           // Kiểm tra AT Command
    delay(200);
    sim_at_cmd("AT+IPREX?");    // Kiểm tra tốc độ baud rate
    delay(200);
    sim_at_cmd(SET_BAUDRATE);   // Cài tốc độ baudrate
    delay(200);
    sim_at_cmd("ATI");          // Thông tin module SIM
    delay(200);
    sim_at_cmd("AT+CPIN?");     // Kiểm tra trạng thái của thẻ SIM
    delay(200);
    sim_at_cmd("AT+CSQ");       // Kiểm tra tín hiệu mạng
    delay(200);
    sim_at_cmd("AT+CIMI");
    delay(200);
    pinMode(13,OUTPUT); 
    digitalWrite(13,HIGH);

    sent_sms();
    delay(5000);                // Delay 5s 
    call();
}

void loop() {     
    if (Serial.available()){
        char c = Serial.read();
        simSerial.write(c);
    }
    sim_at_wait();
}
