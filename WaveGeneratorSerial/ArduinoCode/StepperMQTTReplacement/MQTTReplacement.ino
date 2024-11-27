#include<stdio.h>
#include <string.h>

#include <AccelStepper.h>

// STEPPER CODE /////////////////////////////////////////////
#define DEG_PER_STEP 1.8
#define STEP_PER_REVOLUTION (360 / DEG_PER_STEP)

AccelStepper stepper(AccelStepper::FULL4WIRE, 7, 6, 5, 4);

/////CONSTANTS

int wavetype;
int speed;
int distance;

int value ;

const int powerLed = 13;
const int statusLed = 11;
const int MAX_LINE_LENGTH = 100;

char buffer[MAX_LINE_LENGTH];

bool run = true;

String param = "";

long moveToPosition = STEP_PER_REVOLUTION;

/////

int readLine()
{
  int cc = 0; //Number of chars read
  bool done = false;
  while (Serial.available() && (!done) && (cc < MAX_LINE_LENGTH - 1))
  {
    char cur = Serial.read(); //Read a char
    if (cur == '\n') done = true; //If the received char is \n then we are done
    else buffer[cc++] = cur; //Append to buffer and increment the index
  }
  buffer[cc] = 0; //Terminate the line with a \0
  return cc; //Return line length
}

int clamp(int a,int b,int x)
{
  return max(a,min(b,x));
}

void read() {
  if (readLine() > 0)
  {
    char *token = strtok(buffer, " ");

    param = String(token);
    token = strtok(NULL, " ");

    // if (String(token) == ""){
    //   Serial.println("Missing argument \n");
    // }

    value = String(token).toInt();

    if (param == "run"){
      wavetype = clamp(1,3,value);
      run = true;
    }
    else if (param == "stats"){
      Serial.println("  Wavetype: " + String(wavetype));
      Serial.println("  Distance: " + String(distance));
      Serial.println("  Speed: " + String(speed));
      Serial.println("");
    }

    else if (param == "wavetype"){
      wavetype = clamp(1,3,value);
      Serial.println("Set: wavetype = " + String(wavetype) + "\n");
    }
    else if (param == "speed"){
      speed = clamp(0,5000,value);
      Serial.println("Set: speed = " + String(speed) + "\n");
    }
    else if (param == "distance"){
      distance = clamp(0,200,value);
      Serial.println("Set: distance = " + String(distance) + "\n");
    }
    else if (param != "") {
      Serial.println("Invalid command \n");
    }
    else {
      //do nothing
    }

  }

}

void setup() {
  Serial.begin(9600);
  pinMode(powerLed, OUTPUT);
  pinMode(statusLed, OUTPUT);
  digitalWrite(powerLed, HIGH);  

  // Wave Tank
  stepper.setCurrentPosition(0);
  wavetype = 2;
  speed = 2000;
  distance = 150;
}

void loop() {

  read();

  if (run) {
    
    Serial.println("  Wavetype: " + String(wavetype));
    Serial.println("  Distance: " + String(distance));
    Serial.println("  Speed: " + String(speed));

    Serial.println("Running mode "+ String(wavetype));
    Serial.println("");

    if (wavetype == 1){
      const int steps_per_rev = 200;

      // stepper.setMaxSpeed(100);
      // stepper.setAcceleration(20);
      // stepper.moveTo(500);
      // if (stepper.distanceToGo() == 0)
      // stepper.moveTo(-stepper.currentPosition());

      // stepper.run();  
    }
    if (wavetype == 2){ 
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
    if (wavetype == 3){
    
      for (int i=1; i<=5; i++){
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
        while (stepper.currentPosition() != 0) // Full speed bask to 0
          stepper.run();
        stepper.stop(); // Stop as fast as possible: sets new target
        stepper.runToPosition(); 
        // Now stopped after quickstop
      }
    }
    run = !run;
  }






}
