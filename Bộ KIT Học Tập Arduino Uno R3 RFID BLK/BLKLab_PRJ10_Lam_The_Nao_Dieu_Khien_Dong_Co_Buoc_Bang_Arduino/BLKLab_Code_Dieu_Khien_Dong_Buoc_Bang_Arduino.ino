/*=========================================================================================
 *                                BANLINHKIEN.COM                                         *
 *=========================================================================================
 *                   HUuongs Dẫn Điều KHiển Động Cơ Bước Sử Dung Module ULN2003                      *
 *=========================================================================================
 * Ngoại vi: ULN2003 + StepMotor
 * Link tham khảo:https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/
*/
/*
 * Đấu nối: 
 *  Động cơ bước      ULN2003       Arduino
 *     Đỏ                VDD           5V
 *     Cam               D             D11
 *     Vàng              C             D10
 *     Hồng              B             D9
 *     Xanh Dương        A             D8
 *     GND               GND           GND
*/
// Cấu hình chân IN1, IN2, IN3, IN4 lần lượt 8,9,10,11
// Include the AccelStepper library:
#include <AccelStepper.h>

// Motor pin definitions:
#define motorPin1  8      // IN1 on the ULN2003 driver
#define motorPin2  9      // IN2 on the ULN2003 driver
#define motorPin3  10     // IN3 on the ULN2003 driver
#define motorPin4  11     // IN4 on the ULN2003 driver

// Define the AccelStepper interface type; 4 wire motor in half step mode:
#define MotorInterfaceType 8

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper library with 28BYJ-48 stepper motor:
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  // Set the maximum steps per second:
  stepper.setMaxSpeed(1000);
}

void loop() {
  // Set the speed of the motor in steps per second:
  stepper.setSpeed(1000);
  // Step the motor with constant speed as set by setSpeed():
  stepper.runSpeed();
}
