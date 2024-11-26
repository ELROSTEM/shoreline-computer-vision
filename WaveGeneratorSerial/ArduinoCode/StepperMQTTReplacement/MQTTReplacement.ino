
#include<stdio.h>
#include <string.h>

#include <AccelStepper.h>

// STEPPER CODE /////////////////////////////////////////////
#define DEG_PER_STEP 1.8
#define STEP_PER_REVOLUTION (360 / DEG_PER_STEP)

AccelStepper stepper(AccelStepper::FULL4WIRE, 7, 6, 5, 4);

int wavetype = 0;
int speed = 0;
int distance = 0;

const int powerLed = 13;
const int statusLed = 11;

String dstring = "";

long moveToPosition = STEP_PER_REVOLUTION;


void setup() {
  Serial.begin(9600);
  pinMode(powerLed, OUTPUT);
  pinMode(statusLed, OUTPUT);
  digitalWrite(powerLed, HIGH);  

  // Wave Tank
  stepper.setCurrentPosition(0);
  wavetype = 2;
  speed = 100;
  distance = 50;
}

void loop() {
  
  if (Serial.available()) {
    dstring = Serial.readStringUntil('\n');
    dstring.trim();
    distance = max(0,min(200,dstring.toInt()));

    Serial.print("Distance:");
    Serial.println(distance); 

    if(wavetype == 1){

      const int steps_per_rev = 200;

      // stepper.setMaxSpeed(100);
      // stepper.setAcceleration(20);
      // stepper.moveTo(500);
      // if (stepper.distanceToGo() == 0)
      // stepper.moveTo(-stepper.currentPosition());

      // stepper.run();   
    }
    if(wavetype == 2){

      digitalWrite(statusLed, HIGH);
      stepper.setMaxSpeed(speed);
      stepper.setAcceleration(speed);

      stepper.moveTo(distance);
      while (stepper.currentPosition() != distance) // Full speed up to 300
        stepper.run();
      stepper.stop(); // Stop as fast as possible: sets new target
      stepper.runToPosition(); 
      // Now stopped after quickstop
      // Now go backwards
      stepper.moveTo(0);
      while (stepper.currentPosition() != 0) // Full speed basck to 0
        stepper.run();
      stepper.stop(); // Stop as fast as possible: sets new target
      stepper.runToPosition(); 
      // Now stopped after quickstop

      digitalWrite(statusLed, LOW);
      delay(100);
    }
    if(wavetype == 3){
      stepper.setMaxSpeed(speed);
      stepper.setAcceleration(speed);

      stepper.moveTo(distance);
      while (stepper.currentPosition() != distance) // Full speed up to 300
        stepper.run();
      stepper.stop(); // Stop as fast as possible: sets new target
      stepper.runToPosition(); 
      // Now stopped after quickstop
    
      // Now go backwards
      stepper.moveTo(0);
      while (stepper.currentPosition() != 0) // Full speed basck to 0
        stepper.run();
      stepper.stop(); // Stop as fast as possible: sets new target
      stepper.runToPosition(); 
      // Now stopped after quickstop
    }
  }






}
