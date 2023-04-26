#include <ezButton.h>
#include <AccelStepper.h>

ezButton button1(0);
ezButton button2(1);
ezButton button3(3);

#define DEG_PER_STEP 1.8
#define STEP_PER_REVOLUTION (360 / DEG_PER_STEP)
AccelStepper stepper(AccelStepper::FULL4WIRE, 7, 6, 5, 4);
const int steps_per_rev = 200;

bool oldb1,newb1,oldb2,newb2,oldb3,newb3;
int mode;
bool hold;
bool b1toggled,b2toggled,b3toggled;

void setup() {
  Serial.begin(9600);
  button1.setDebounceTime(50);
  button2.setDebounceTime(50);
  button3.setDebounceTime(50);
  pinMode(8,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(2,OUTPUT);
  stepper.setCurrentPosition(0);
}
void varpulse(int speed1, int speed2, int distance, int iterations){
  stepper.moveTo(distance);
  for (int i = 0; i < iterations; i++){
    stepper.setMaxSpeed(speed1);
    stepper.setAcceleration(speed1);
    stepper.moveTo(distance);
    while (stepper.currentPosition() != distance){
     stepper.run();
    }
    stepper.stop();
    stepper.runToPosition();
    stepper.setMaxSpeed(speed2);
    stepper.setAcceleration(speed2);
    stepper.moveTo(0);
    while (stepper.currentPosition() != 0){ 
      stepper.run();
    }
    stepper.stop();
    stepper.runToPosition();
  }
}

void modes(int m){
  if (m == 0){
    varpulse(5000,5000,150,1);
  }
  if (m == 1){
    varpulse(5000,1000,150,1);
  }
  if (m == 2){
    varpulse(2000,2000,150,5);
  }
}

void loop() {
  button1.loop();
  button2.loop();
  button3.loop();
  oldb1 = newb1;
  oldb2 = newb2;
  oldb3 = newb3;
  newb1 = button1.isPressed();
  newb2 = button2.isPressed();
  newb3 = button3.isPressed();
  b1toggled = (oldb1 == true && newb1 == false);
  b2toggled = (oldb2 == true && newb2 == false);
  b3toggled = (oldb3 == true && newb3 == false);
  digitalWrite(2,LOW);
  int potentout = floor(analogRead(A1)*4/1023);
  int mode = potentout;
  if (mode == 0){
    digitalWrite(8,HIGH);
    digitalWrite(10,LOW);
    digitalWrite(12,LOW);
  }
  if (mode == 1){
    digitalWrite(8,LOW);
    digitalWrite(10,HIGH);
    digitalWrite(12,LOW);
  }
  if (mode == 2){
    digitalWrite(8,LOW);
    digitalWrite(10,LOW);
    digitalWrite(12,HIGH);
  }
  if (b3toggled){
    digitalWrite(2,HIGH);
    modes(mode);
    while (true){
      if (button3.getStateRaw() == 1){
        break;
      }
      modes(mode);
    }
    digitalWrite(2,LOW);
  }
}
