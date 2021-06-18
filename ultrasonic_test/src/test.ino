#include <Arduino.h>
#include "NewPing.h"
#define TRIGGER_PIN 15
#define ECHO_PIN 7
#define MAX_DISTANCE 1000
NewPing sonar (TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

float distance;
int cycleCount = 0;
int numLEDPins = 8;
int LEDpins [] = {27, PIN_A5, PIN_A4, PIN_A2, PIN_A1, 16, 11, 30};
int batteryPIN = PIN_A3;
bool shouldFlash = false;
void setup() {
  // put your setup code here, to run once:
  
  
    for(int i = 0; i < numLEDPins; i++){
      pinMode(LEDpins[i], OUTPUT);
      digitalWrite(LEDpins[i],  HIGH);
    }
  
 
  Serial.begin(9600);

}

void loop() {


  Serial.println(millis());

  // put your main code here, to run repeatedly:
    if(cycleCount % 300 == 0){
      distance = sonar.ping_cm();

      //Serial.print("Distance = ");
      if(distance >= 400 || distance <= 2){
        //Serial.println("out of range");
        Serial.println(500);
        
      }
      else{
        Serial.println(distance);
        //Serial.println(" cm");
      }
  
    }
  delay(1);
  cycleCount++;
}