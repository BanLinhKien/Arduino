#define BLYNK_PRINT Serial
// You should get Auth Token in the Blynk App.
#define BLYNK_TEMPLATE_ID           "TMPL6VHV8uDUc"
#define BLYNK_TEMPLATE_NAME         "testBlynkMQ2"
char BLYNK_AUTH_TOKEN[32]   =   "";

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include "def.h"
#include "config.h"
#include "mybutton.h"
#include <SimpleKalmanFilter.h>
#include <ESP32Servo.h>

//-------------------- Khai báo Kalman Filter----------------------
SimpleKalmanFilter kfilter(2, 2, 0.001);
//-------------------- Khai báo Button-----------------------------
// Khai báo chân các nút nhấn
#define buttonPinMENU    5
#define buttonPinDOWN    18
#define buttonPinUP      19
#define buttonPinONOFF   21
#define BUTTON1_ID  1
#define BUTTON2_ID  2
#define BUTTON3_ID  3
#define BUTTON4_ID  4
Button buttonMENU;
Button buttonDOWN;
Button buttonUP;
Button buttonONOFF;
void button_press_short_callback(uint8_t button_id);
void button_press_long_callback(uint8_t button_id);

//----------------------Khai báo LCD1602---------------------------------
// Create An LCD Object. Signals: [ RS, EN, D4, D5, D6, D7 ]
#define LCD_RS  15
#define LCD_EN  13
#define LCD_D4  12
#define LCD_D5  14
#define LCD_D6  27
#define LCD_D7  26
LiquidCrystal My_LCD(15, 13, 12, 14, 27, 26);

//------------------------- Khai báo cảm biến ---------------------------
#define SENSOR_MQ2  35
#define SENSOR_FIRE   34
#define SENSOR_FIRE_ON    0
#define SENSOR_FIRE_OFF   1
//-------------------------Khai báo còi ---------------------------------
#define BUZZER  23
#define BUZZER_ON 1
#define BUZZER_OFF 0
//------------------------- Khai báo relay ------------------------------
#define ON 1
#define OFF 0
#define AUTO 1
#define MANUAL 0
bool relay1State = OFF;
bool relay2State = OFF;
bool autoManual = AUTO;    // luu vao eeprom
int  mq2Thresshold = 40;   // luu vao epprom
#define THRESSHOLD  4000

// ------------------------ Khai báo servo -----------------------------
Servo myservo1; 
Servo myservo2; 
int windowState = OFF;
//------------------------- Khai báo wifi -------------------------------
WebServer server(80);//Specify port 
WiFiClient client;
#define AP_MODE 0
#define STA_MODE 1
bool AP_STA_MODE = 1;

#define MODE_WIFI  0
#define MODE_NOWIFI  1
bool modeWIFI = MODE_NOWIFI;
bool tryCbWIFI = MODE_NOWIFI;

//---------------------- Nguyên mẫu hàm  ---------------------------------
void TaskButton(void *pvParameters);
void TaskBlynk(void *pvParameters);
void TaskMainDisplay(void *pvParameters);
void TaskSwitchAPtoSTA(void *pvParameters);
void D_AP_SER_Page();
void Get_Req();
void readEEPROM();
void ClearEeprom();
void connectSTA();
int  readMQ2();
void sendDatatoBlynk();
void LCD1602_Init();
void LCDPrint(int hang, int cot, char *text, int clearOrNot);
void controlRelay(int relay, int state);
void closeWindow();
void openWindow();
void printRelayState();
void printMode();
void controlWindow(int onoff);

//-------------------- Khai báo biến freeRTOS ----------------------------
TaskHandle_t TaskMainDisplay_handle = NULL;
TaskHandle_t TaskButton_handle = NULL;

void setup() {  
  Serial.begin(115200);
  EEPROM.begin(512);
  LCD1602_Init();
  delay(2000);
  My_LCD.clear();
  //---------- Khai báp WiFi ---------
  Serial.println("Configuring access point...");
  WiFi.mode(WIFI_AP_STA);           //Both in Station and Access Point Mode

  //---------- Khai báo relay --------
  pinMode(RELAY1, OUTPUT);                 
  pinMode(RELAY2, OUTPUT);
  controlRelay(RELAY1, OFF);
  controlRelay(RELAY2, OFF);

  //---------- Khai báo BUZZER --------
  pinMode(BUZZER, OUTPUT);  
  digitalWrite(BUZZER, BUZZER_OFF);

  //----------- Khai báo cảm biến lửa -------
  pinMode(SENSOR_FIRE, INPUT_PULLUP);  
  digitalWrite(SENSOR_FIRE, BUZZER_OFF);
  // ---------- Khai báo Servo --------------
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo1.setPeriodHertz(50);  
  myservo2.setPeriodHertz(50);  
  myservo1.attach(SERVO1, 500, 2400);
  myservo2.attach(SERVO2, 500, 2400);
  closeWindow();
  // ---------- Kết nối WiFi ---------
  readEEPROM();
  connectSTA();

  //----------- Read thresshold from EEPROM --------
  mq2Thresshold = EEPROM.read(202) * 100 + EEPROM.read(203);
  Serial.println(" mq2Thresshold ");
  Serial.println(mq2Thresshold);

  if(mq2Thresshold > 9999) mq2Thresshold = THRESSHOLD;

  //----------- Read AUTO/MANUAL EEPROM --------
  autoManual = EEPROM.read(201);
  if(autoManual > 1) autoManual = AUTO;
  Serial.println(autoManual);
  // ---------- Khai báo hàm FreeRTOS ---------
 
  xTaskCreatePinnedToCore(TaskMainDisplay,    "TaskMainDisplay" ,  1024*4 ,  NULL,  5 ,  &TaskMainDisplay_handle  , 0 );
  xTaskCreatePinnedToCore(TaskSwitchAPtoSTA,  "TaskSwitchAPtoSTA" , 1024*4 ,  NULL,  5 ,  NULL ,  0);
  xTaskCreatePinnedToCore(TaskBuzzer,         "TaskBuzzer" , 1024*2 ,  NULL,  5 ,  NULL ,  0);
  xTaskCreatePinnedToCore(TaskButton,         "TaskButton" ,       1024*4 ,  NULL,  5 ,  &TaskButton_handle ,  0);
  
}
void loop() {
     vTaskDelete(NULL);
}

//------------------------------------------------------------------------------
//---------------------------Task Switch AP to STA -----------------------------

//---------------------------If IP is Hitted in Browser ------------------------
void D_AP_SER_Page() {
    int Tnetwork=0,i=0,len=0;
    String st="",s="";
    Tnetwork = WiFi.scanNetworks();//Scan for total networks available
    st = "<ul>";
    for (int i = 0; i < Tnetwork; ++i) {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st +=i + 1;
      st += ": ";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*";
      st += "</li>";
    }
    st += "</ul>";
   IPAddress ip = WiFi.softAPIP();             //Get ESP IP Adress
        //String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        s = "<ul>";
        s = "\n\r\n<!DOCTYPE HTML>\r\n<html><h1>GAS DETECTION SYSTEM</h1> ";
        //s += ipStr;
        s += "<p>";
        s += st;
        s += "<h1> Config Wifi and Token Blynk</h1>";
        s += "<form method='get' action='a'><label style='font-size:30px'>SSID:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</label><input style='font-size: 30px; height:80px'  name='ssid' length=32> <br> <br> <label style='font-size:30px'>Password:</label><input style='font-size: 30px; height:80px' name='pass' length=64> <br><br><label style='font-size:30px'>Tocken:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</label><input style='font-size: 30px; height:80px' name='token' length=64> <br><br><input  style='font-size:30px;height:60px; width: 100px' type='submit'></form>";
        s += "</ul>";
        s += "</html>\r\n\r\n";
      
    server.send( 200 , "text/html", s);
}
//--------------------------- Get SSID & Password  --------------------------- 
void Get_Req() {
  vTaskSuspend(TaskMainDisplay_handle);
  if (server.hasArg("ssid") && server.hasArg("pass") && server.hasArg("token")) {  
     sssid=server.arg("ssid");//Get SSID
     passs=server.arg("pass");//Get Password
     token=server.arg("token");//Get token
     Serial.println(sssid);
     Serial.println(passs);
     Serial.println(token);  
  }
  LCDPrint(0,0,"config Wifi STA",1);
  LCDPrint(1,0," please check ",0);
  delay(2000);
  LCDPrint(0,0,(char*)sssid.c_str(),1);
  LCDPrint(1,0,(char*)passs.c_str(),0);
  delay(5000);
  if(sssid.length()>1 && passs.length()>1 && token.length()>1) {
     ClearEeprom();//First Clear Eeprom
     delay(10);
     for (int i = 0; i < sssid.length(); ++i)
         EEPROM.write(i, sssid[i]);  
     for (int i = 0; i < passs.length(); ++i)
        EEPROM.write(32+i, passs[i]);
     for (int i = 0; i < token.length(); ++i)
        EEPROM.write(64+i, token[i]);
    
     EEPROM.commit();
        
     String s = "\r\n\r\n<!DOCTYPE HTML>\r\n<html><h1>GAS DETECTION SYSTEM</h1> ";
     s += "<p>Password Saved... Reset to boot into new wifi</html>\r\n\r\n";
     server.send(200,"text/html",s);
   }
   LCDPrint(0,1," RESTART ",1);
   delay(2000);
   LCDPrint(1,1,"   DONE ",0);
   delay(2000);
   ESP.restart();
}

//--------------------------- connect STA mode and switch AP Mode if connect fail --------------------------- 
bool blynkConnect = true;
BlynkTimer timer; 
void connectSTA() {
      if ( Essid.length() > 1 ) {  
      Serial.println(Essid);        //Print SSID
      Serial.println(Epass);        //Print Password
      Serial.println(Etoken);        //Print token
      Etoken = Etoken.c_str();
      WiFi.begin(Essid.c_str(), Epass.c_str());   //c_str()
      int countConnect = 0;
      String dotConnect = "";
      while (WiFi.status() != WL_CONNECTED) {
          LCDPrint(0,0,"WiFi connecting ",0);
          delay(500);
          dotConnect += ".";
          if(dotConnect.length() > 15) {
              dotConnect = "";
              My_LCD.clear();
          }
          LCDPrint(1,0,(char*)dotConnect.c_str(),0);       
          if(countConnect++  == 20) {
            Serial.println("Connect fail, please check ssid and pass");
            LCDPrint(0,0,"Disconnect Wifi",1);
            LCDPrint(1,0," check again",0);
            delay(2000);
            LCDPrint(0,0,"connect WF:ESP32",1);
            LCDPrint(1,0,"192.168.4.1",0);
            delay(2000);
            break;
          }
      }
      Serial.println("");
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected.");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP()); 
        LCDPrint(0,1,"WiFi Connected",1);
        LCDPrint(1,0,(char*)Essid.c_str(),0);
        delay(2000);
        strcpy(BLYNK_AUTH_TOKEN,Etoken.c_str());
        
        Blynk.config(BLYNK_AUTH_TOKEN);
        blynkConnect = Blynk.connect();
        if(blynkConnect == false) {
            My_LCD.clear();
            LCDPrint(0,0,"disconnect BLYNK",0);
            LCDPrint(1,0,"  check again",0);
            delay(3000);
            switchAPMode(); 
        }
        else {
            My_LCD.clear();
            LCDPrint(0,0,"BLYNK connected ",0);
            delay(100);
            timer.setInterval(2000L, myTimer); 
            
        }
        xTaskCreatePinnedToCore(TaskBlynk, "TaskBlynk" ,   1024*4 ,  NULL,  5 ,  NULL ,  0);
        AP_STA_MODE = STA_MODE;
      }
      else
        switchAPMode(); 
    }
}

//--------------------------- switch AP Mode --------------------------- 
void switchAPMode() {
   WiFi.softAP(ssid, pass);          //AP SSID and Password(Both in Ap and Sta Mode-According to Library)
   delay(100);                            //Stable AP
   server.on("/",D_AP_SER_Page);
   server.on("/a",Get_Req); 
   Serial.println("In Ap Mode");
   server.begin();  
   delay(300);
}
//--------------------------- Read Eeprom  ------------------------------------
void readEEPROM() {
    for (int i = 0; i < 32; ++i)     //Reading SSID
        Essid += char(EEPROM.read(i)); 
    for (int i = 32; i < 64; ++i)   //Reading Password
        Epass += char(EEPROM.read(i)); 
    for (int i = 64; i < 96; ++i)   //Reading Password
        Etoken += char(EEPROM.read(i)); 
}
//--------------------------- Clear Eeprom  ----------------------------------
void ClearEeprom() {
     Serial.println("Clearing Eeprom");
     for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
}
void writeThresHoldEEPROM(int mq2Thresshold)
{
    int firstTwoDigits = mq2Thresshold / 100;  // lấy 2 số hàng nghìn và trăm
    int lastTwoDigits  = mq2Thresshold % 100;  // lấy 2 số hàng chục và đơn vị
    EEPROM.write(202, firstTwoDigits);         // lưu 2 số hàng nghìn và trăm vào flash
    EEPROM.write(203, lastTwoDigits);          // lưu 2 số hàng chục và đơn vị vào flash
    EEPROM.commit();
    Serial.println(mq2Thresshold);
}
//---------------------------Task TaskSwitchAPtoSTA---------------------------
void TaskSwitchAPtoSTA(void *pvParameters) {
    while(1) {
        server.handleClient();   
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

//-----------------------------------------------------------------------------
//---------------------------Task Blynk----------------------------------------

//----------------------- Send MQ2 value to Blynk every 2 seconds--------------

void myTimer() {
    int sensorVal = readMQ2();
    Blynk.virtualWrite(V0, readMQ2());  
}
//----------------------- Read Relay state from Blynk----------------------------
// 0 -> RL1 = 0, RL2 = 0
// 1 -> RL1 = 1, RL2 = 0
// 2 -> RL1 = 0, RL2 = 1
// 3 -> RL1 = 1, RL2 = 1
BLYNK_WRITE(V1) {
  int relayState = param.asInt();
  switch(relayState) {
      case 0:
        relay1State = 0;relay2State = 0;
        digitalWrite(RELAY1,relay1State);
        digitalWrite(RELAY2,relay2State);
        break;
      case 1:
        relay1State = 1;relay2State = 0;
        digitalWrite(RELAY1,relay1State);
        digitalWrite(RELAY2,relay2State);
        break;
      case 2:
        relay1State = 0;relay2State = 1;
        digitalWrite(RELAY1,relay1State);
        digitalWrite(RELAY2,relay2State);
        break;
      case 3:
        relay1State = 1;relay2State = 1;
        digitalWrite(RELAY1,relay1State);
        digitalWrite(RELAY2,relay2State);
        break;
  }

  printRelayState();
  autoManual = MANUAL;
  EEPROM.write(201, MANUAL);EEPROM.commit();
  printMode();
  Blynk.virtualWrite(V4, autoManual);  
}
//----------------------- Read servo from Blynk---------------------------------
BLYNK_WRITE(V2) {
  int servoState = param.asInt();
  windowState = servoState; 
  controlWindow(windowState);
  autoManual = MANUAL;
  EEPROM.write(201, MANUAL);EEPROM.commit();
  //printMode();
  Blynk.virtualWrite(V4, autoManual); 

  printWindowState(windowState);

}

//----------------------- Read thresshold mq2 sensor from Blynk------------------
BLYNK_WRITE(V3) {
    mq2Thresshold = param.asInt();
    writeThresHoldEEPROM(mq2Thresshold);

    vTaskSuspend(TaskMainDisplay_handle);
    My_LCD.clear();
    delay(100);
    LCDPrint(0,0,"  Thresshold ",0);
    char str[20];
    sprintf(str,"%d",mq2Thresshold);
    LCDPrint(1,6,str,0);
    delay(1000);
    My_LCD.clear(); delay(100);
    printRelayState();printMQ2();printMode();
    delay(100);
    vTaskResume(TaskMainDisplay_handle);
}

//----------------------- Read state Auro/Manual from Blynk-----------------------
// state Auro/Manual 
BLYNK_WRITE(V4) {
    autoManual = param.asInt();
    EEPROM.write(201, autoManual); EEPROM.commit();
    Serial.println(autoManual);
    printMode();
}

//----------------------- Send Data to Blynk-------------------------------------
void sendDatatoBlynk() {
    Blynk.virtualWrite(V0, readMQ2());  
    Blynk.virtualWrite(V3, mq2Thresshold);
    Blynk.virtualWrite(V4, autoManual);  
}
//----------------------- Task Blynk---------------------------------------------

void TaskBlynk(void *pvParameters)  {
   sendDatatoBlynk();
   while(1) {
      if(blynkConnect == true) {
          Blynk.run();
          timer.run();
      }
      vTaskDelay(100/portTICK_PERIOD_MS);
   }
}

//----------------------------------------------------------------
//----------------------------LCD1602 Init------------------------
void LCDPrint(int hang, int cot, char *text, int clearOrNot) {
   if(clearOrNot == 1)
      My_LCD.clear();
   My_LCD.setCursor(cot, hang);
   My_LCD.print(text);
}


void LCD1602_Init() {
   My_LCD.begin(16, 2);
   My_LCD.clear();
   LCDPrint(0,2, "Gas Detection",0);
   LCDPrint(1,5, "System",0);
}

void printMode() {
   if(autoManual == AUTO)
      LCDPrint(0,15,"A" ,0);
    else
      LCDPrint(0,15,"M" ,0); 
}
void printWindow() {
   if(autoManual == AUTO)
      LCDPrint(0,15,"A" ,0);
    else
      LCDPrint(0,15,"M" ,0); 
}
//-----------------------  Print Relay State LCD ---------------------------
void printRelayState() {
    if(relay1State == 0)
      LCDPrint(1,0,"RL1:OFF ", 0);
    else
      LCDPrint(1,0,"RL1:ON " , 0); 
    if(relay2State == 0)
      LCDPrint(1,9,"RL2:OFF " ,0);
    else
      LCDPrint(1,9,"RL2:ON " , 0);
}


//-----------------------  Read MQ2 value ---------------------------
int readMQ2() {
    float MQ2_Value = analogRead(SENSOR_MQ2);
    MQ2_Value = kfilter.updateEstimate(MQ2_Value);
    MQ2_Value = map(MQ2_Value, 0 , 4095, 0, 10000 );
    Serial.println(MQ2_Value);
    return MQ2_Value;
}

//-----------------------  Read Fire sensor value ---------------------------
int readFireSensor() {
    int Fire_Value = digitalRead(SENSOR_FIRE);
    Serial.println(Fire_Value);
    return Fire_Value;
}
//-----------------------Print MQ2 value LCD-------------------------
void printMQ2() {
    float MQ2_Value = readMQ2();
    LCDPrint(0,0,(char*)("GAS:" + String((int)MQ2_Value) + "ppm ").c_str(),0); 
}

void printWindowState(int windowState)
{
    vTaskSuspend(TaskMainDisplay_handle);
    My_LCD.clear();
    if(windowState == 1)
      LCDPrint(0,2,"OPEN WINDOW",0); 
    else
      LCDPrint(0,2,"CLOSE WINDOW",0); 
    delay(1000);
    My_LCD.clear();
    printMode();printRelayState();printMQ2() ;
    vTaskResume(TaskMainDisplay_handle);
}

//-----------------------Task Main Display and Control Device----------
int checkSensor = 0;
int buzzerON = 0;
int sendNotificationsOnce = 0;
void TaskMainDisplay(void *pvParameters) {
    //----------- Khởi tạo LCD ------------------
    delay(10000);
    // My_LCD.clear();
    // LCDPrint(0,0, "WAIT FOR SENSORS",0 );
    // LCDPrint(1,0, "TO START",0 );
    // for(int i = 60; i >= 10 ; i --) {
    //     My_LCD.setCursor(12, 1);
    //     My_LCD.print(" ");
    //     My_LCD.setCursor(13, 1);
    //     My_LCD.print(i);
    //     delay(1000);
    // }  

    // LCDPrint(0,0, "WAIT FOR SENSOR",0 );
    // LCDPrint(1,0, "TO START",0 );
    // for(int i = 9; i >= 0 ; i --) {
    //     My_LCD.setCursor(13, 1);
    //     My_LCD.print(" ");
    //     My_LCD.setCursor(14, 1);
    //     My_LCD.print(i);
    //     delay(1000);
    // } 

    My_LCD.clear();
    printRelayState();
    printMode();
    printMQ2(); 
    
    while(1) {
         if(autoManual == AUTO) {
           
              if(readMQ2() > mq2Thresshold ) {
                 buzzerON = 1;
                 relay1State = ON;  relay2State = OFF; 
                 controlRelay(RELAY1, relay1State);
                 controlRelay(RELAY2, relay2State);
                 Blynk.virtualWrite(V1, 3); 
                 Blynk.virtualWrite(V2, 1);  
                 windowState = 1; controlWindow(windowState);
                 delay(3000);
              }
              else if(readFireSensor() == SENSOR_FIRE_ON) {
                 buzzerON = 1;
                 relay1State = OFF;  relay2State = ON; 
                 controlRelay(RELAY1, relay1State);
                 controlRelay(RELAY2, relay2State);
                 Blynk.virtualWrite(V1, 1); 
                 delay(3000);
              }
              else if(readMQ2() < mq2Thresshold - 100  && readFireSensor() == SENSOR_FIRE_OFF ) {
                 relay1State = OFF;  relay2State = OFF; 
                 controlRelay(RELAY1, relay1State);
                 controlRelay(RELAY2, relay2State);
                 Blynk.virtualWrite(V1, 0); 
                 Blynk.virtualWrite(V2, 0);  
                 windowState = 0; controlWindow(windowState);
              }
         }
          if(readMQ2() > mq2Thresshold && readFireSensor() == SENSOR_FIRE_ON) {
            if(sendNotificationsOnce == 0)
              Blynk.logEvent("gas_fire_detection","Fire & Gas detected");
            sendNotificationsOnce = 1;
            My_LCD.clear();
            LCDPrint(0,4, "WARNING",0 );
            LCDPrint(1,2, "GAS DETECTED",0 );
            buzzerON = 1;
            delay(500);
            My_LCD.clear();
            Serial.println("WARNING Fire & Gas detected");
            LCDPrint(0,4, "WARNING",0 );
            LCDPrint(1,2, "FIRE DETECTED",0 );
            delay(500);
          }
          else if(readMQ2() > mq2Thresshold && readFireSensor() == SENSOR_FIRE_OFF) {
            if(sendNotificationsOnce == 0)
              Blynk.logEvent("gas_fire_detection","Gas exceeds permissible limits");
            sendNotificationsOnce = 1;
            Serial.println("WARNING Gas exceeds permissible limits");
            My_LCD.clear();
            delay(500);
            LCDPrint(0,4, "WARNING",0 );
            LCDPrint(1,2, "GAS DETECTED",0 );
            buzzerON = 1;
          }
          else if(readMQ2() < mq2Thresshold - 100 && readFireSensor()== SENSOR_FIRE_ON) {
            if(sendNotificationsOnce == 0)
              Blynk.logEvent("gas_fire_detection","Fire detected");
            sendNotificationsOnce = 1;
            My_LCD.clear();
            delay(500);
            Serial.println("WARNING Fire Fire");
            LCDPrint(0,4, "WARNING",0 );
            LCDPrint(1,2, "FIRE DETECTED",0 );
            buzzerON = 1;
          }
          else if(readMQ2() < mq2Thresshold - 100 && readFireSensor() == SENSOR_FIRE_OFF ) {
            My_LCD.clear(); delay(20);
            printRelayState(); delay(20);
            printMode(); delay(20);
            printMQ2(); delay(20);
            buzzerON = 0;
            sendNotificationsOnce = 0;
         }  
         delay(2000);

    }
}
void TaskBuzzer(void *pvParameters) {
   while(1) {
      if(buzzerON == 1)
        buzzerWarning();
      else
        digitalWrite(BUZZER, BUZZER_OFF);
      delay(10);
   }
}

//----------------------------------------------------------------
//----------------------Task Button-------------------------------
void TaskButton(void *pvParameters) {
    pinMode(buttonPinMENU, INPUT);
    pinMode(buttonPinDOWN, INPUT);
    pinMode(buttonPinUP, INPUT);
    pinMode(buttonPinONOFF, INPUT);
    // ---------- Khởi tạo BUTTON --------------
    button_init(&buttonMENU, buttonPinMENU, BUTTON1_ID);
    button_init(&buttonDOWN, buttonPinDOWN, BUTTON2_ID);
    button_init(&buttonUP,   buttonPinUP,   BUTTON3_ID);
    button_init(&buttonONOFF,buttonPinONOFF,BUTTON4_ID);
    button_pressshort_set_callback((void *)button_press_short_callback);
    button_presslong_set_callback((void *)button_press_long_callback);

    while(1) {
      handle_button(&buttonMENU);
      handle_button(&buttonDOWN);
      handle_button(&buttonUP);
      handle_button(&buttonONOFF);
      vTaskDelay(10/portTICK_PERIOD_MS);
   }
}

//-----------------------Hàm điều khiển relay -------------------------
void controlRelay(int relay, int state) {
    digitalWrite(relay,state);
}

//----------------------- Hàm điều khiển servo ----------------------------

void openWindow() {
    myservo1.write(0); 
    myservo2.write(180);
}
void closeWindow() {
    myservo1.write(90); 
    myservo2.write(90);
}
void controlWindow(int onoff) {
    if(onoff == 0)
      closeWindow();
    else
      openWindow();
}

// ---------------------- Hàm điều khiển còi -----------------------------
void buzzerBip() {
    //digitalWrite(BUZZER, BUZZER_ON);delay(300);
    //digitalWrite(BUZZER, BUZZER_OFF);
}

void buzzerWarning() {
    digitalWrite(BUZZER, BUZZER_ON); delay(2000);
    digitalWrite(BUZZER, BUZZER_OFF);delay(500);
}

void sendRelayStateToBlynk(int relay1State, int relay2State)
{
    if(relay1State == 0 && relay2State == 0)
        Blynk.virtualWrite(V1, 0);  
    else if(relay1State == 1 && relay2State == 0)
        Blynk.virtualWrite(V1, 1); 
    else if(relay1State == 0 && relay2State == 1)
        Blynk.virtualWrite(V1, 2); 
    else if(relay1State == 1 && relay2State == 1)
        Blynk.virtualWrite(V1, 3); 
}
//----------------------- Hàm xử lí nút nhấn nhả -------------------------
int modeSetThresHold   = 0;

void button_press_short_callback(uint8_t button_id) {
      switch(button_id) {
        case BUTTON1_ID :  // Menu
          Serial.println("bt1 press short");
          buzzerBip();
          modeSetThresHold = 1 - modeSetThresHold;
          if(modeSetThresHold == 1) {
              vTaskSuspend(TaskMainDisplay_handle);
              My_LCD.clear();
              delay(100);
              LCDPrint(0,0," SET Thresshold ",0);
              char str[20];
              sprintf(str,"%d",mq2Thresshold);
              LCDPrint(1,6,str,0);
              delay(100);
          }
          else {
              LCDPrint(1,1," SUCCESSFULLY ", 0);  
              delay(1000);
              My_LCD.clear();
              delay(100);
              printMQ2(); delay(20);
              printRelayState(); delay(20);
              printMode(); delay(20);
              Blynk.virtualWrite(V3, mq2Thresshold); 
              writeThresHoldEEPROM(mq2Thresshold);
              vTaskResume(TaskMainDisplay_handle);
          }
          break;
        case BUTTON2_ID :
          Serial.println("bt2 press short");
          buzzerBip();
          if(modeSetThresHold == 1) {
              My_LCD.clear();
              delay(100);
              LCDPrint(0,0," SET Thresshold ",0);
              char str[20];
              mq2Thresshold += 50;
               if(mq2Thresshold > 9999) mq2Thresshold = 9999;
              sprintf(str,"%d",mq2Thresshold);
              LCDPrint(1,6,str,0);
          }
          else {
            if(autoManual == AUTO)
              autoManual = MANUAL;
            EEPROM.write(201, MANUAL);EEPROM.commit();
            relay1State = 1 - relay1State;
            controlRelay(RELAY1,relay1State);
            sendRelayStateToBlynk(relay1State, relay2State);
            Blynk.virtualWrite(V4, MANUAL); 
            My_LCD.clear(); delay(20);
            printRelayState(); delay(20);
            printMode(); delay(20);
            printMQ2(); delay(20);
          }
         
          break;
        case BUTTON3_ID :
          Serial.println("bt3 press short");
          buzzerBip();
          if(modeSetThresHold == 1) {
              My_LCD.clear();
              delay(100);
              LCDPrint(0,0," SET Thresshold ",0);
              char str[20];
              mq2Thresshold -= 50;
              if(mq2Thresshold < 200) mq2Thresshold = 200;
              sprintf(str,"%d",mq2Thresshold + 100);
              LCDPrint(1,6,str,0);
          }
          else {
              if(autoManual == AUTO)
                autoManual = MANUAL;
              EEPROM.write(201, MANUAL);EEPROM.commit();
              relay2State = 1 - relay2State;
              controlRelay(RELAY2,relay2State);
              sendRelayStateToBlynk(relay1State, relay2State);
              Blynk.virtualWrite(V4, MANUAL); 
              My_LCD.clear(); delay(20);
              printRelayState(); delay(20);
              printMode(); delay(20);
              printMQ2(); delay(20);
          }
          break;  
        case BUTTON4_ID :
          Serial.println("bt4 press short");
          buzzerBip();
          if(modeSetThresHold == 0) {
            if(autoManual == AUTO)
              autoManual = MANUAL;
            Blynk.virtualWrite(V4, MANUAL); 
            windowState = 1 - windowState;
            Blynk.virtualWrite(V2, windowState);  
            controlWindow(windowState);
            printWindowState(windowState);
          }
          
          break;     
     } 
} 

//-----------------Hàm xử lí nút nhấn giữ ----------------------
void button_press_long_callback(uint8_t button_id)
{
    switch(button_id)
   {  
      case BUTTON1_ID :
        buzzerBip();
        if(modeSetThresHold == 0) {
          AP_STA_MODE = 1 - AP_STA_MODE;
          switch(AP_STA_MODE) {
            case AP_MODE:
              vTaskSuspend(TaskMainDisplay_handle);
              LCDPrint(0,1,"WiFi AP Mode",1);
              delay(2000); 
              LCDPrint(0,0,"connect WF:ESP32",1);
              LCDPrint(1,1,"192.168.4.1",0);
              // Switch to AP mode
              switchAPMode();
              break;
            case STA_MODE:
              ESP.restart();
              break;
          }
        }
        break;
      case BUTTON2_ID :
        buzzerBip();
        break;
      case BUTTON3_ID :
        buzzerBip();
        break;  
      case BUTTON4_ID :
        buzzerBip();
        if(modeSetThresHold == 0) {
          autoManual = 1 - autoManual;
          Blynk.virtualWrite(V4, autoManual);
          EEPROM.write(201, autoManual);EEPROM.commit();
          printMode();
        }
        break; 
   }   
}
