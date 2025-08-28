#include "Arduino.h"
#include <MPU6050_tockn.h>
#include "LedControl.h"
#include "Delay.h"

#define  MATRIX_A  1
#define MATRIX_B  0

MPU6050 mpu6050(Wire);

// Ngưỡng gia tốc để xác định hướng
#define ACC_THRESHOLD_LOW -25
#define ACC_THRESHOLD_HIGH 25

// Ma trận LED
#define PIN_DATAIN 6
#define PIN_CLK 4
#define PIN_LOAD 5

// Rotary Encoder
#define PIN_ENC_1 3
#define PIN_ENC_2 2
#define PIN_ENC_BUTTON 7

// Buzzer báo động
#define PIN_BUZZER 14

// Cấu hình xoay LED matrix
#define ROTATION_OFFSET 90

// Chống dội nút
#define DEBOUNCE_THRESHOLD 500

#define DELAY_FRAME 100

#define DEBUG_OUTPUT 1

// Các chế độ hoạt động
#define MODE_HOURGLASS 0
#define MODE_SETMINUTES 1
#define MODE_SETHOURS 2

// Biến thời gian
byte delayHours = 0;
byte delayMinutes = 1;
int mode = MODE_HOURGLASS;
int gravity;
LedControl lc = LedControl(PIN_DATAIN, PIN_CLK, PIN_LOAD, 2);
NonBlockDelay d;
int resetCounter = 0;
bool alarmWentOff = false;

// ================== CÁC HÀM XỬ LÝ ==================

long getDelayDrop() {
  return delayMinutes + delayHours * 60;
}


#if DEBUG_OUTPUT
void printmatrix() {
  Serial.println(" 0123-4567 ");
  for (int y = 0; y<8; y++) {
    if (y == 4) {
      Serial.println("|----|----|");
    }
    Serial.print(y);
    for (int x = 0; x<8; x++) {
      if (x == 4) {
        Serial.print("|");
      }
      Serial.print(lc.getXY(0,x,y) ? "X" :" ");
    }
    Serial.println("|");
  }
  Serial.println("-----------");
}
#endif



coord getDown(int x, int y) {
  coord xy;
  xy.x = x-1;
  xy.y = y+1;
  return xy;
}
coord getLeft(int x, int y) {
  coord xy;
  xy.x = x-1;
  xy.y = y;
  return xy;
}
coord getRight(int x, int y) {
  coord xy;
  xy.x = x;
  xy.y = y+1;
  return xy;
}



bool canGoLeft(int addr, int x, int y) {
  if (x == 0) return false; 
  return !lc.getXY(addr, getLeft(x, y)); 
}
bool canGoRight(int addr, int x, int y) {
  if (y == 7) return false; 
  return !lc.getXY(addr, getRight(x, y)); 
}
bool canGoDown(int addr, int x, int y) {
  if (y == 7) return false; // not available
  if (x == 0) return false; // not available
  if (!canGoLeft(addr, x, y)) return false;
  if (!canGoRight(addr, x, y)) return false;
  return !lc.getXY(addr, getDown(x, y)); 
}



void goDown(int addr, int x, int y) {
  lc.setXY(addr, x, y, false);
  lc.setXY(addr, getDown(x,y), true);
}
void goLeft(int addr, int x, int y) {
  lc.setXY(addr, x, y, false);
  lc.setXY(addr, getLeft(x,y), true);
}
void goRight(int addr, int x, int y) {
  lc.setXY(addr, x, y, false);
  lc.setXY(addr, getRight(x,y), true);
}


int countParticles(int addr) {
  int c = 0;
  for (byte y=0; y<8; y++) {
    for (byte x=0; x<8; x++) {
      if (lc.getXY(addr, x, y)) {
        c++;
      }
    }
  }
  return c;
}


bool moveParticle(int addr, int x, int y) {
  if (!lc.getXY(addr,x,y)) {
    return false;
  }

  bool can_GoLeft = canGoLeft(addr, x, y);
  bool can_GoRight = canGoRight(addr, x, y);

  if (!can_GoLeft && !can_GoRight) {
    return false; // we're stuck
  }

  bool can_GoDown = canGoDown(addr, x, y);

  if (can_GoDown) {
    goDown(addr, x, y);
  } else if (can_GoLeft&& !can_GoRight) {
    goLeft(addr, x, y);
  } else if (can_GoRight && !can_GoLeft) {
    goRight(addr, x, y);
  } else if (random(2) == 1) { 
    goLeft(addr, x, y);
  } else {
    goRight(addr, x, y);
  }
  return true;
}



void fill(int addr, int maxcount) {
  int n = 8;
  byte x,y;
  int count = 0;
  for (byte slice = 0; slice < 2*n-1; ++slice) {
    byte z = slice<n ? 0 : slice-n + 1;
    for (byte j = z; j <= slice-z; ++j) {
      y = 7-j;
      x = (slice-j);
      lc.setXY(addr, x, y, (++count <= maxcount));
    }
  }
}



/**
 * Phát hiện hướng bằng cách sử dụng máy đo gia tốc
 *
 *     | up | right | left | down |
 * --------------------------------
 * 400 |    |       | y    | x    |
 * 330 | y  | x     | x    | y    |
 * 260 | x  | y     |      |      |
 */
int getGravity() {
  int x = mpu6050.getAngleX();
  int y = mpu6050.getAngleY();
  if (y < ACC_THRESHOLD_LOW)  { return 90;   }
  if (x > ACC_THRESHOLD_HIGH) { return 0;  }
  if (y > ACC_THRESHOLD_HIGH) { return 270; }
  if (x < ACC_THRESHOLD_LOW)  { return 180; }
}


int getTopMatrix() {
  return (getGravity() == 90) ? MATRIX_A : MATRIX_B;
}
int getBottomMatrix() {
  return (getGravity() != 90) ? MATRIX_A : MATRIX_B;
}



void resetTime() {
  for (byte i=0; i<2; i++) {
    lc.clearDisplay(i);
  }
  fill(getTopMatrix(), 60);
  d.Delay(getDelayDrop() * 1000);
}



/**
Duyệt qua ma trận và kiểm tra xem các hạt có cần di chuyển không
*/
bool updateMatrix() {
  int n = 8;
  bool somethingMoved = false;
  byte x,y;
  bool direction;
  for (byte slice = 0; slice < 2*n-1; ++slice) {
    direction = (random(2) == 1); 
    byte z = slice<n ? 0 : slice-n + 1;
    for (byte j = z; j <= slice-z; ++j) {
      y = direction ? (7-j) : (7-(slice-j));
      x = direction ? (slice-j) : j;
      // for (byte d=0; d<2; d++) { lc.invertXY(0, x, y); delay(50); }
      if (moveParticle(MATRIX_B, x, y)) {
        somethingMoved = true;
      };
      if (moveParticle(MATRIX_A, x, y)) {
        somethingMoved = true;
      }
    }
  }
  return somethingMoved;
}


boolean dropParticle() {
  if (d.Timeout()) {
    d.Delay(getDelayDrop() * 1000);
    if (gravity == 0 || gravity == 180) {
      if ((lc.getRawXY(MATRIX_A, 0, 0) && !lc.getRawXY(MATRIX_B, 7, 7)) ||
          (!lc.getRawXY(MATRIX_A, 0, 0) && lc.getRawXY(MATRIX_B, 7, 7))
      ) {
        // for (byte d=0; d<8; d++) { lc.invertXY(0, 0, 7); delay(50); }
        lc.invertRawXY(MATRIX_A, 0, 0);
        lc.invertRawXY(MATRIX_B, 7, 7);
        tone(PIN_BUZZER, 440, 10);
        return true;
      }
    }
  }
  return false;
}



void alarm() {
  for (int i=0; i<5; i++) {
    tone(PIN_BUZZER, 440, 200);
    delay(1000);
  }
}



void resetCheck() {
  int z = analogRead(A3);
  if (z > ACC_THRESHOLD_HIGH || z < ACC_THRESHOLD_LOW) {
    resetCounter++;
    Serial.println(resetCounter);
  } else {
    resetCounter = 0;
  }
  if (resetCounter > 20) {
    Serial.println("RESET!");
    resetTime();
    resetCounter = 0;
  }
}



void displayLetter(char letter, int matrix) {
  // Serial.print("Letter: ");
  // Serial.println(letter);
  lc.clearDisplay(matrix);
  lc.setXY(matrix, 1,4, true);
  lc.setXY(matrix, 2,3, true);
  lc.setXY(matrix, 3,2, true);
  lc.setXY(matrix, 4,1, true);

  lc.setXY(matrix, 3,6, true);
  lc.setXY(matrix, 4,5, true);
  lc.setXY(matrix, 5,4, true);
  lc.setXY(matrix, 6,3, true);

  if (letter == 'M') {
    lc.setXY(matrix, 4,2, true);
    lc.setXY(matrix, 4,3, true);
    lc.setXY(matrix, 5,3, true);
  }
  if (letter == 'H') {
    lc.setXY(matrix, 3,3, true);
    lc.setXY(matrix, 4,4, true);
  }
}



void renderSetMinutes() {
  fill(getTopMatrix(), delayMinutes);
  displayLetter('M', getBottomMatrix());
}
void renderSetHours() {
  fill(getTopMatrix(), delayHours);
  displayLetter('H', getBottomMatrix());
}




void knobClockwise() {
  Serial.println("Clockwise");
  if (mode == MODE_SETHOURS) {
    delayHours = constrain(delayHours+1, 0, 64);
    renderSetHours();
  } else if(mode == MODE_SETMINUTES) {
    delayMinutes = constrain(delayMinutes+1, 0, 64);
    renderSetMinutes();
  }
  Serial.print("Delay: ");
  Serial.println(getDelayDrop());
}
void knobCounterClockwise() {
  Serial.println("Counterclockwise");
  if (mode == MODE_SETHOURS) {
    delayHours = constrain(delayHours-1, 0, 64);
    renderSetHours();
  } else if (mode == MODE_SETMINUTES) {
    delayMinutes = constrain(delayMinutes-1, 0, 64);
    renderSetMinutes();
  }
  Serial.print("Delay: ");
  Serial.println(getDelayDrop());
}



volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;
long lastValue = 0;
void updateEncoder() {
  int MSB = digitalRead(PIN_ENC_1); 
  int LSB = digitalRead(PIN_ENC_2); 

  int encoded = (MSB << 1) |LSB; 
  int sum  = (lastEncoded << 2) | encoded; 

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue--;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue++;

  // Serial.print("Value: ");
  // Serial.println(encoderValue);
  if ((encoderValue % 4) == 0) {
    int value = encoderValue / 4;
    if (value > lastValue) knobClockwise();
    if (value < lastValue) knobCounterClockwise();
    lastValue = value;
  }
  lastEncoded = encoded; 
}


volatile unsigned long lastButtonPushMillis;
void buttonPush() {
  if((long)(millis() - lastButtonPushMillis) >= DEBOUNCE_THRESHOLD) {
    mode = (mode+1) % 3;
    Serial.print("Switched mode to: ");
    Serial.println(mode);
    lastButtonPushMillis = millis();

    if (mode == MODE_SETMINUTES) {
      lc.backup(); 
      renderSetMinutes();
    }
    if (mode == MODE_SETHOURS) {
      renderSetHours();
    }
    if (mode == MODE_HOURGLASS) {
      lc.clearDisplay(0);
      lc.clearDisplay(1);
      lc.restore();
      resetTime();
    }
  }
}


//----------------------Hàm SETUP-----------------------------
void setup() {
mpu6050.calcGyroOffsets(true);
  Serial.begin(9600);
  mpu6050.begin();
  pinMode(PIN_ENC_1, INPUT);
  pinMode(PIN_ENC_2, INPUT);
  pinMode(PIN_ENC_BUTTON, INPUT);
  digitalWrite(PIN_ENC_1, HIGH); 
  digitalWrite(PIN_ENC_2, HIGH); 
  digitalWrite(PIN_ENC_BUTTON, HIGH); 
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_1), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_2), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_BUTTON), buttonPush, RISING);

   Serial.println(digitalPinToInterrupt(PIN_ENC_1));
   Serial.println(digitalPinToInterrupt(PIN_ENC_2));
   Serial.println(digitalPinToInterrupt(PIN_ENC_BUTTON));

  randomSeed(analogRead(A0));

  // init displays
  for (byte i=0; i<2; i++) {
    lc.shutdown(i,false);
    lc.setIntensity(i,0);
  }

  resetTime();
}


//----------------------Hàm LOOP-----------------------------
void loop() {
  mpu6050.update();
  Serial.println("angleX : ");
  Serial.println(mpu6050.getAngleX());
  Serial.println("\tangleY : ");
  Serial.println(mpu6050.getAngleY());
 
  delay(DELAY_FRAME);
    gravity = getGravity();
  lc.setRotation((ROTATION_OFFSET + gravity) % 360);

  if (mode == MODE_SETMINUTES) {
    renderSetMinutes(); return;
  } else if (mode == MODE_SETHOURS) {
    renderSetHours(); return;
  }

  bool moved = updateMatrix();
  bool dropped = dropParticle();

  if (!moved && !dropped && !alarmWentOff && (countParticles(getTopMatrix()) == 0)) {
    alarmWentOff = true;
    alarm();
  }
  // reset alarm flag next time a particle was dropped
  if (dropped) {
    alarmWentOff = false;
  }
}
