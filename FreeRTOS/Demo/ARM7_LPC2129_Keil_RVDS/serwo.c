#include <LPC21xx.H>
#include "serwo.h"
#include "led.h"
#include "timer_interrupts.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#define PINDET_BM (1<<10)

#define QUEUE_LEN   8

xSemaphoreHandle xSemaphore;
QueueHandle_t xQueue;

enum ServoState {CALLIB, OFFSET_CALLIB, IDLE, IN_PROGRESS};
enum DetectorState{ACTIVE, INACTIVE};
unsigned int uiServoFreq = 0;

struct Servo {
	enum ServoState eState; 
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

static struct Servo sServo;

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
	unsigned int uiNewPos;
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
					xSemaphoreGive(xSemaphore);

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

				if (xQueueReceive(xQueue, &uiNewPos, 0) == pdTRUE)
				{
					sServo.uiDesiredPosition = uiNewPos;
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
					sServo.eState = IDLE;
					xSemaphoreGive(xSemaphore);

				}
				break;
		}
		vTaskDelay(uiServoFreq);
	}
}

void Servo_Init(unsigned int uiServoFrequency)
{
	uiServoFreq = 1000/uiServoFrequency;
	vSemaphoreCreateBinary(xSemaphore);
	xQueue = xQueueCreate(QUEUE_LEN, sizeof(unsigned int));
	Led_Init();
	DetectorInit();
	xTaskCreate(Automat, NULL, 128, NULL, 1, NULL);

	Servo_Callib();
}

void Servo_Callib(void)
{
	sServo.eState = CALLIB;
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
}

void Servo_GoTo(unsigned int uiPosition)
{
	xQueueSend(xQueue, &uiPosition, portMAX_DELAY);
	//sServo.eState = IN_PROGRESS;
	xSemaphoreTake(xSemaphore, portMAX_DELAY);

}

void Servo_Wait(unsigned int uiTickToWait){
	vTaskDelay(uiTickToWait);
}
