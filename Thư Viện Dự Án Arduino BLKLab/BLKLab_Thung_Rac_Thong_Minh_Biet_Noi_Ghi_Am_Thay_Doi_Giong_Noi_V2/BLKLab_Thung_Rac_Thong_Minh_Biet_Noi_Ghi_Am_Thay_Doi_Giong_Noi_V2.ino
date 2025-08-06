// BANLINHKIEN.COM
// Code Thung rac thong minh+8qp

#include "mybutton.h"
#include <SoftwareSerial.h>
#include <JQ6500_Serial.h>
#include <DistanceSRF04.h>
#include <Servo.h>
#include <avr/wdt.h>
#include <EEPROM.h>
JQ6500_Serial mp3(4,5); // rx tx

// Khai báo chân module MP3

int countKey = 0;
// Khai báo led
#define LED 13
// Khai báo nút nhấn
#define BUTTON_DISTANCE_PIN  6
#define BUTTON_PLAY_PIN      7

#define BUTTON1_ID  1
#define BUTTON2_ID  2

Button buttonDISTANCE;
Button buttonPLAY;

DistanceSRF04 Dist;
Servo myservo;

#define SERVO       11
#define SRF04_ECHO  3
#define SRF04_TRIG  2


#define GOC_DONG 130 //Đây là góc đóng của servo
#define GOC_MO 0 //Đây là góc mở của servo

int distance;
unsigned long previousMillis = 0;
unsigned char autoTrigger = 0;
unsigned long autoMillis = 0;
int sensorValueCBRung = 0; 
uint8_t k = 0;
typedef enum
{
  IDLE_STATE,
  OPEN_STATE,
  CLOSE_STATE
}MODE_STATE;
uint8_t modeRun = IDLE_STATE;
uint32_t timeMillis = 0;
uint8_t timeOpen  = 4;
int THRESHOLD = 20;

void setup() {
  Serial.begin(9600);
  //echo, trigger
  Dist.begin(3, 2);
  myservo.attach(11);
  myservo.write(GOC_DONG);

  // Khởi tạo Module MP3
  mp3.begin(9600);
  mp3.reset();
  mp3.setVolume(30);
  mp3.setLoopMode(MP3_LOOP_NONE);
  // Khởi tạo LED
  pinMode(LED, 13);
  digitalWrite(LED, 0);
  // Khởi tạo nút nhấn
  pinMode(BUTTON_DISTANCE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PLAY_PIN, INPUT_PULLUP);
  button_init(&buttonDISTANCE, BUTTON_DISTANCE_PIN, BUTTON1_ID);
  button_init(&buttonPLAY,     BUTTON_PLAY_PIN,     BUTTON2_ID);

  button_pressshort_set_callback((void *)button_press_short_callback);
  THRESHOLD = EEPROM.read(0);
  if(THRESHOLD == 255 ) THRESHOLD = 20;
  Serial.print("THRESHOLD get: ");
  Serial.println(THRESHOLD);
  blinkLED(THRESHOLD/10);
}

void loop() {
  Serial.println(sensorValueCBRung);
  switch(modeRun)
  {
    case IDLE_STATE:
      if(readSRF04() == 1 )
      {
        modeRun = OPEN_STATE;
        myservo.write(GOC_MO);
        digitalWrite(13, HIGH);
        timeMillis = millis();
        playMp3(0);
      }
      break;
    case OPEN_STATE:
      if(millis() - timeMillis > timeOpen*1000) {
         timeMillis = millis() ;
         wdt_reset();
         modeRun = CLOSE_STATE;
      }
      if(readSRF04() == 1 ){
        timeMillis = millis();
      }  
      break;
    case CLOSE_STATE:
       myservo.write(GOC_DONG);
       digitalWrite(13, LOW);
       playMp3(1);
       wdt_reset();
       delay(500);
       modeRun = IDLE_STATE;
      break;
  }
  handle_button(&buttonDISTANCE);
  handle_button(&buttonPLAY);
}

uint8_t readSRF04() {
      // Phần previousMillis >= 100 đây chính là thời gian lấy mẫu của cảm biến siêu âm 100ms
      if (millis() - previousMillis >= 10 ){
        previousMillis = millis();
        distance = Dist.getDistanceCentimeter();
        // Phần distance < 15 đây là phần cài đặt khoảng cách cảm biến nhận được kích hoạt mở thùng rác
        if (distance < THRESHOLD && distance > 1) 
            return 1;
        return 0;
      }
}

void blinkLED(int number) {
  for(int i = 0; i < number; ++i) {
    digitalWrite(LED, 1); delay(200);
    digitalWrite(LED, 0); delay(200);
  }
}

void playMp3(int number) {
  if(number%2 == 0) 
      mp3.playFileByIndexNumber(1);
  else if(number%2 == 1)
      mp3.playFileByIndexNumber(2);

}
void button_press_short_callback(uint8_t button_id) {
    switch(button_id) {
      case BUTTON1_ID :  
        THRESHOLD += 10;
        if(THRESHOLD > 50) THRESHOLD = 10;
        blinkLED(THRESHOLD/10);
        EEPROM.write(0, THRESHOLD);
        Serial.print("THRESHOLD set: ");
        Serial.println(THRESHOLD);
        break;
      case BUTTON2_ID :
        Serial.println("keyMP3 press short");
        Serial.println(countKey);
        playMp3(countKey);
        countKey++;
        break;
    } 
} 
