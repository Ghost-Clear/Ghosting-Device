#include <Arduino.h>
#include "NewPing.h"
#define TRIGGER_PIN 15
#define ECHO_PIN 7
#define MAX_DISTANCE 1000
NewPing sonar (TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

float distance;
int cycleCount = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {

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