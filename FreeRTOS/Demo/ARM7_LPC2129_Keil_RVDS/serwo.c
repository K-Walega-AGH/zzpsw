#include <LPC21xx.H>
#include "serwo.h"
#include "led.h"
#include "timer_interrupts.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#define PINDET_BM (1<<10)

#define QUEUE_LEN   8

QueueHandle_t xQueue; 

enum ServoState {CALLIB, OFFSET_CALLIB, IDLE, IN_PROGRESS};
enum DetectorState{ACTIVE, INACTIVE};
static unsigned int uiServoFreq;
static unsigned int uiServoInitFreq;


struct Servo {
	enum ServoState eState; 
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

static struct Servo sServo;

enum DataType {POSITION, WAIT_TIME, SPEED,CALIBRATION};

struct Control {
	enum DataType eDataType;
	unsigned int uiData;
};

static void DetectorInit(void)
{
	IO0DIR &= ~PINDET_BM;
}

static enum DetectorState eReadDetector(void)
{
	if ((IO0PIN & PINDET_BM) != 0)
	{
		return INACTIVE;
	}
	else
	{
		return ACTIVE;
	}
}

 void Automat(void *pvParameters)
{
	struct Control sData;
	while(1){
		switch(sServo.eState)
		{
			case CALLIB:
				sServo.uiCurrentPosition = 0;
				sServo.uiDesiredPosition = 0;
				
				if (eReadDetector() == INACTIVE)
				{
					LedStepLeft();
				}
				else
				{
					sServo.eState = IDLE;

				}
				break;
			case OFFSET_CALLIB:

				if (sServo.uiCurrentPosition == 12)
				{
					sServo.uiCurrentPosition = 0;
					sServo.uiDesiredPosition = 0;
					sServo.eState = IDLE;
				}
				else
				{
					sServo.uiCurrentPosition++;
					LedStepRight();
				}
				break;
			case IDLE:
				
				if (xQueueReceive(xQueue, &sData,portMAX_DELAY) == pdTRUE)
				{
					sServo.uiDesiredPosition = (sData.eDataType == POSITION) ? sData.uiData : sServo.uiDesiredPosition;
					vTaskDelay((sData.eDataType == WAIT_TIME) ? sData.uiData : 0);
					uiServoFreq = (sData.eDataType == SPEED) ? sData.uiData : uiServoFreq;
					sServo.eState = (sData.eDataType == CALIBRATION) ? CALLIB : IDLE;
				}
				
				if (sServo.uiCurrentPosition != sServo.uiDesiredPosition)
				{
					sServo.eState = IN_PROGRESS;
				}
				
				break;
				
			case IN_PROGRESS:

				if (sServo.uiCurrentPosition < sServo.uiDesiredPosition)
				{
					sServo.uiCurrentPosition++;
					LedStepRight();
				}
				else if (sServo.uiCurrentPosition > sServo.uiDesiredPosition)
				{
					sServo.uiCurrentPosition--;
					LedStepLeft();
				}
				else
				{
					uiServoFreq = uiServoInitFreq;
					sServo.eState = IDLE;
				}
				break;
		}
		vTaskDelay(uiServoFreq);
	}
}

void Servo_Init(unsigned int uiServoFrequency)
{
	uiServoFreq = 1000/uiServoFrequency;
	uiServoInitFreq = uiServoFreq;
	xQueue = xQueueCreate(QUEUE_LEN, sizeof(struct Control));
	Led_Init();
	DetectorInit();

	xTaskCreate(Automat, NULL, 256, NULL, 1, NULL);
	Servo_Callib();
	
}

void Servo_Callib(void)
{
	struct Control sData;
	sData.eDataType = CALIBRATION;
	xQueueSend(xQueue,&sData,portMAX_DELAY);
}

void Servo_GoTo(unsigned int uiPosition)
{
	struct Control sData;
	sData.eDataType = POSITION;
	sData.uiData = uiPosition;
	xQueueSend(xQueue,&sData,portMAX_DELAY);
}

void Servo_Wait(unsigned int uiTickToWait){
	struct Control sData;
	sData.eDataType = WAIT_TIME;
	sData.uiData = uiTickToWait;
	xQueueSend(xQueue,&sData,portMAX_DELAY);
}

void Servo_Speed(unsigned int uiTicksPerStep)
{
	struct Control sData;
	sData.eDataType = SPEED;
	sData.uiData = uiTicksPerStep;
	xQueueSend(xQueue,&sData,portMAX_DELAY);
}
