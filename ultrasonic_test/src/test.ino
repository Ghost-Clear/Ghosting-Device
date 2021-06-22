#include <Arduino.h>
#include "NewPing.h"
#define TRIGGER_PIN 15
#define ECHO_PIN 7
unsigned int EchoPin = 7;   // The Arduino's the Pin2 connection to US-100 Echo / RX
unsigned int TrigPin = 15;
unsigned long Time_Echo_us = 0; 
unsigned long Len_mm  = 0;  
void setup() { 
  
 //Initialize 
 Serial.begin(9600);         //  The measurement results through the serial output to the serial port on the PC monitor
 pinMode(EchoPin, INPUT);    //  The set EchoPin input mode.
 pinMode(TrigPin, OUTPUT);   //  The set TrigPin output mode.
}  

void loop() {   
  
  // By the Trig / Pin sending pulses trigger US-100 ranging
 digitalWrite(TrigPin, HIGH);                         // Send pulses begin by Trig / Pin
 delayMicroseconds(50);                               // Set the pulse width of 50us (> 10us)
 digitalWrite(TrigPin, LOW);                          // The end of the pulse    
 Time_Echo_us = pulseIn(EchoPin, HIGH);               // A pulse width calculating US-100 returned
 if((Time_Echo_us < 60000) && (Time_Echo_us > 1)) {   // Pulse effective range (1, 60000).
  // Len_mm = (Time_Echo_us * 0.34mm/us) / 2 (mm) 
  Len_mm = (Time_Echo_us*34/100)/2;                   // Calculating the distance by a pulse width.   
               // Output to the serial port monitor 
  Serial.println(Len_mm, DEC);                          // Output to the serial port monitor   
                            // Output to the serial port monitor
 }  
 delay(30);                                         // Per second (1000ms) measured
} 