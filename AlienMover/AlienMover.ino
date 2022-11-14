#include <Stepper.h>
/**
 This program is a first test version for controlling Alien.
 Program starts when systemStartButton is pressed.
 program will initialize alien postition by turning step motor down 
 untill PIN_ALIEN_DOWN_SWITC indikates voltage > 0
 then it lift it ALIEN_HOMESEEK_STEPLENGTH steps per round until
 PIN_ALIEN_DOWN_SWITCH indikates 0 voltage (this is home position of Alien)
 Then alien is lifted by 90 steps, being there for 5 seconds and brought back down 90 steps.
*/
const int TOTAL_STEPS = 100; //stp motor total steps
const int PIN_STEPPER_A = 10;
const int PIN_STEPPER_B = 9;
const int PIN_ALIEN_ENABLER = 8;
const int ALIEN_SPEED = 30; //alien motor speed
const int PIN_ALIEN_DOWN_SWITCH = 0;
const int PIN_SYSTEM_START_BTN = 4;

//once alien is on bottom and alien switch is down
//how long steps we move up untill the aliendown switch is released
const int ALIEN_HOMESEEK_STEPLENGTH = 1;


int alienDownSensorVal = 0;
bool alienHomeRunDone = false;
bool systemStarted = false;

Stepper alienMover(TOTAL_STEPS,PIN_STEPPER_A,PIN_STEPPER_B);


void setup() {
  //pinMode(PIN_SYSTEM_START_BTN, INPUT);
  //pinMode(PIN_ALIEN_DOWN_SWITCH,INPUT);
  alienMover.setSpeed(ALIEN_SPEED); // we set motor speed at 30

}

void doAlien(){
  digitalWrite(PIN_ALIEN_ENABLER , HIGH); //enable alien
  if (!alienHomeRunDone){
      while(alienDownSensorVal ==0){
        alienMover.step(-1); 
        alienDownSensorVal = analogRead(PIN_ALIEN_DOWN_SWITCH);
        delay(1);
      }      
      //is alien on bottom
      if(alienDownSensorVal >0 ){
        while(analogRead(PIN_ALIEN_DOWN_SWITCH) >0){
          alienMover.step(ALIEN_HOMESEEK_STEPLENGTH); //move alien up untill switch releases
          delay(1);
        }
        alienHomeRunDone =true;
      }
    }else{
      alienMover.step(90); //now bring alien up
      delay(5000); //let alien be up
      alienMover.step(-90); //bring alien back down
    }
    digitalWrite(PIN_ALIEN_ENABLER , LOW); //disable alien
}

void loop() {
  //init alien
  //move down untill alienswitch gives a signal
  int buttonState = digitalRead(PIN_SYSTEM_START_BTN);
  if(buttonState >0 && !systemStarted){
    systemStarted = true;
  }
  if(systemStarted){
     doAlien();
     systemStarted =false;
  }
  delay(1); 
}
