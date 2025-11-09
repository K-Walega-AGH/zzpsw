#include <LPC21xx.H>
#include "serwo.h"
#include "led.h"
#include "timer_interrupts.h"

#define DETECTOR_bm (1<<10)

enum ServoState {CALLIB, IDDLE, IN_PROGRESS};
struct Servo
{
enum ServoState eState;
unsigned int uiCurrentPosition;
unsigned int uiDesiredPosition;
};
struct Servo sServo;

enum State {ACTIVE, INACTIVE};
void DetectorInit(void){

	IO0DIR=IO0DIR&(~DETECTOR_bm);
}

enum State eReadDetector (void){

	if((IO0PIN&DETECTOR_bm) == 0){
		return(INACTIVE);
	}else{
		return(ACTIVE);
	}
}

void Servo_Callib(void){

	sServo.eState = CALLIB;
	while(eReadDetector()==INACTIVE);
}

void Servo_GoTo(unsigned int uiPosition){

	sServo.eState = IN_PROGRESS;
	sServo.uiDesiredPosition = uiPosition;
}

void SerwoAutomat(){

	 switch(sServo.eState){
		case IDDLE: 	
			if(sServo.uiCurrentPosition != sServo.uiDesiredPosition){
				sServo.eState = IN_PROGRESS;
			}else{
				sServo.eState = IDDLE;
			}
			break;
		case IN_PROGRESS: 
			if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){
				Led_StepLeft();
				sServo.eState = IN_PROGRESS;
				sServo.uiCurrentPosition--;
			}else if(sServo.uiCurrentPosition < sServo.uiDesiredPosition){
				sServo.eState = IN_PROGRESS;
				Led_StepRight();
				sServo.uiCurrentPosition++;
			}else{
				sServo.eState = IDDLE;
			}
			break;
		case CALLIB: 
			if(eReadDetector()==INACTIVE){
				Led_StepRight();
			}else{
				sServo.eState = IDDLE;
				sServo.uiCurrentPosition = 0;
				sServo.uiDesiredPosition = 0;				
			}	
			break;
	 }
}
void Servo_Init(unsigned int uiServoFrequency){

	Timer0Interrupts_Init(1000000/uiServoFrequency, &SerwoAutomat);
	LedInt();
	DetectorInit();
	Servo_Callib();
}
