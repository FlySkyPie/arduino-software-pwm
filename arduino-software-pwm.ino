#include <stdint.h>

#include "TimerOne.h"

#include "PwmManager.h"
#include "ArduinoHardware.h"

ArduinoHardware arduinoHardware;
PwmManager pwmManager(&arduinoHardware);

void doPwm(){
  uint32_t delay = pwmManager.doDate();

  Timer1.initialize(delay);
  Timer1.attachInterrupt(doPwm); 
}


void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  Timer1.initialize(2000);
  Timer1.attachInterrupt(doPwm); 
}

void setPower(uint8_t power){
  String msg = "power: " + String(power) + "\n";
  //Serial.write(msg.c_str());
  for(uint8_t j=0;j<6;j++){
   pwmManager.setPower(j,power);
  }
}

uint64_t time = 0;
void loop() {
  time +=20;
  for(uint8_t i=0;i<6;i++){
    float degree = (time + i * 60) *3.14159 /180;
    uint8_t power = ((sin(degree/2) +1) * 255 /2 );
    pwmManager.setPower(i,power);
  }
  delay(200);
  
}
