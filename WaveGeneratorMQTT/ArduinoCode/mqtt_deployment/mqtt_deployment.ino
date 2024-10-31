
#include<stdio.h>
#include <string.h>

#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <AccelStepper.h>

#include "arduino_secrets.h"

// WIFI MQTT CODE /////////////////////////////////////////////
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "wave/motor";

// STEPPER CODE /////////////////////////////////////////////
#define DEG_PER_STEP 1.8
#define STEP_PER_REVOLUTION (360 / DEG_PER_STEP)

AccelStepper stepper(AccelStepper::FULL4WIRE, 7, 6, 5, 4);

int wavetype = 0;
int speed = 0;
int distance = 0;

long moveToPosition = STEP_PER_REVOLUTION;


void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(12, HIGH);  
  // attempt to connect to WiFi network:
  Serial.println("Attempting to connect");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    digitalWrite(13, LOW);

    Serial.println("failed");    
    delay(5000);

    digitalWrite(13, HIGH);
  }

  if (!mqttClient.connect(broker, port)) {
    while (1);
  }

  // subscribe to a topic
  mqttClient.subscribe(topic);

  digitalWrite(13, HIGH);
  // Wave Tank
  stepper.setCurrentPosition(0);
  wavetype = 1;
  speed = 100;
  distance = 50;
}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    // we received a message, print out the topic and contents

    // Stream in characters one by one into string message
    char message[100] = "";
    while (mqttClient.available()) {
      char ch = (char)mqttClient.read();
      strncat(message, &ch, 1);
    }

    char * token = strtok(message, " ");
    wavetype = atoi(token);

    token = strtok(NULL, " ");
    speed = atoi(token);

    token = strtok(NULL, " ");
    distance = atoi(token);
    
    Serial.print("Type:");
    Serial.println(wavetype);  
    /*MonoPulse Mode*/
    // value of 255: pulse mode

    if(wavetype == 1){

      const int steps_per_rev = 200;

      // stepper.setMaxSpeed(100);
      // stepper.setAcceleration(20);
      // stepper.moveTo(500);
      // if (stepper.distanceToGo() == 0)
      // stepper.moveTo(-stepper.currentPosition());
 
      // stepper.run();

      pinMode(13, OUTPUT);
      digitalWrite(13, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(1000);                      // wait for a second
      digitalWrite(13, LOW);   // turn the LED off by making the voltage LOW
      delay(1000);    
    }
    if(wavetype == 2){
      stepper.setMaxSpeed(speed);
      stepper.setAcceleration(speed);

      stepper.moveTo(distance);
      while (stepper.currentPosition() != distance) // Full speed up to 300
        stepper.run();
      stepper.stop(); // Stop as fast as possible: sets new target
      stepper.runToPosition(); 
      // Now stopped after quickstop
      Serial.println("Reverse");
      // Now go backwards
      stepper.moveTo(0);
      while (stepper.currentPosition() != 0) // Full speed basck to 0
        stepper.run();
      stepper.stop(); // Stop as fast as possible: sets new target
      stepper.runToPosition(); 
      // Now stopped after quickstop
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
