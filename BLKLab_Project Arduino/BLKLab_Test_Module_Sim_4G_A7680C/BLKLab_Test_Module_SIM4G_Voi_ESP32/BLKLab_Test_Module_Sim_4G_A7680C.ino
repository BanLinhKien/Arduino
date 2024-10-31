#include <HardwareSerial.h>

#define simSerial               Serial2
#define MCU_SIM_BAUDRATE        115200
#define MCU_SIM_TX_PIN              17
#define MCU_SIM_RX_PIN              16
#define MCU_SIM_EN_PIN              15

#define PHONE_NUMBER                "+8498*****"

void sim_at_wait()
{
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

void sent_sms()
{
    sim_at_cmd("AT+CMGF=1");
    String temp = "AT+CMGS=\"";
    temp += (String)PHONE_NUMBER;
    temp += "\"";
    sim_at_cmd(temp);
    sim_at_cmd("ESP32-SIM7680X From balinhkien.com");

    sim_at_send(0x1A);    // End charactor for SMS
}

void call()
{
    String temp = "ATD";
    temp += PHONE_NUMBER;
    temp += ";";
    sim_at_cmd(temp); 

    delay(20000);
    sim_at_cmd("ATH"); 
}

void setup() 
{
    /*  Power enable  */
    pinMode(MCU_SIM_EN_PIN,OUTPUT); 
    digitalWrite(MCU_SIM_EN_PIN,LOW);

    delay(20);
    Serial.begin(115200);
    Serial.println("\n\n\n\n-----------------------\nSystem started!!!!");

    delay(8000);                // Delay 8s for power on
    simSerial.begin(MCU_SIM_BAUDRATE, SERIAL_8N1, MCU_SIM_RX_PIN, MCU_SIM_TX_PIN);
    
    sim_at_cmd("AT");           // Check AT Command

    sim_at_cmd("ATI");          // Product infor

    sim_at_cmd("AT+CPIN?");     // Check SIM Slot

    sim_at_cmd("AT+CSQ");       // Check Signal Quality

    sim_at_cmd("AT+CIMI");

    pinMode(2,OUTPUT); 
    digitalWrite(2,HIGH);

    sent_sms();
    delay(5000);  // Delay 5s 
    call();
}

void loop() 
{     
    if (Serial.available()){
        char c = Serial.read();
        simSerial.write(c);
    }
    sim_at_wait();
}
