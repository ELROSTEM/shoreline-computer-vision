#include <ezButton.h>
#include <AccelStepper.h>

ezButton button4(3);

#define DEG_PER_STEP 1.8
#define STEP_PER_REVOLUTION (360 / DEG_PER_STEP)
AccelStepper stepper(AccelStepper::FULL4WIRE, 7, 6, 5, 4);
const int steps_per_rev = 200;

int oldb4,newb4;
int mode;
bool hold;
bool b4toggled;

void setup() {
  Serial.begin(9600);
  button4.setDebounceTime(50);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
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

void modes(){
  if (mode == 0){
    varpulse(5000,5000,150,1);
  }
  if (mode == 1){
    varpulse(5000,1000,150,1);
  }
  if (mode == 2){
    varpulse(2000,2000,150,5);
  }
}

void loop() {
  button4.loop();
  oldb4 = newb4;
  newb4 = button4.isPressed();
  b4toggled = (oldb4 == true && newb4 == false);
  digitalWrite(2,LOW);
  int potentout = floor(analogRead(A1)*3.9/1023);
  int mode = potentout;
  Serial.println(mode);
  if (mode == 0){
    digitalWrite(8,HIGH);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
  }
  if (mode == 1){
    digitalWrite(8,LOW);
    digitalWrite(9,HIGH);
    digitalWrite(10,LOW);
  }
  if (mode == 2){
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,HIGH);
  }
  if (b4toggled){
    digitalWrite(2,HIGH);
    modes();
    while (true){
      if (button4.getStateRaw() == 1){
        break;
      }
      modes();
    }
    digitalWrite(2,LOW);
  }
}
