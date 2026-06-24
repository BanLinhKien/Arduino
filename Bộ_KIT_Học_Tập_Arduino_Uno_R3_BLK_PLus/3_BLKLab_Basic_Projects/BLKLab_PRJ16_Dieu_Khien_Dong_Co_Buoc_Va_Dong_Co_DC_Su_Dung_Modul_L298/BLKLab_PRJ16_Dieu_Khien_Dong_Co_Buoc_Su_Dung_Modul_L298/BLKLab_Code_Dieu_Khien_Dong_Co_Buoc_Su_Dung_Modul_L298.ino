#include <Stepper.h>                    
const int STEPS_PER_REV = 2048;         
const int motorPin1 = 9;                
const int motorPin2 = 10;                
const int motorPin3 = 11;                
const int motorPin4 = 12;  
      
const int REVOLUTIONS = 5; 
Stepper myStepper(STEPS_PER_REV, motorPin1, motorPin3, motorPin2, motorPin4);
void setup() {
  myStepper.setSpeed(18);               
  Serial.begin(9600);                   
}

void loop(){
  Serial.println("Quay Thuận....");
  myStepper.step(STEPS_PER_REV * REVOLUTIONS);
  delay(1000);
  Serial.println("Quay Nghịch....");
  myStepper.step(-STEPS_PER_REV * REVOLUTIONS);
  delay(1000);
}
