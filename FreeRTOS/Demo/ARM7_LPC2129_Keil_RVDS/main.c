#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

typedef struct{
	unsigned char ucBlinkingFreq;
	unsigned char ucLedNr;
}Led_Control;

void LedBlink( void *pvParameters ){
		Led_Control *pxCtrl = (Led_Control*)pvParameters;
	while(1){
		Led_Toggle(pxCtrl->ucLedNr);
		vTaskDelay((1000/(pxCtrl->ucBlinkingFreq))/2);
	}
}

void LedCtrl (void *pvChangingFreq){
	Led_Control *pxCtrl = (Led_Control*)pvChangingFreq;
	unsigned char ucCounter = 0;
	while(1){
		vTaskDelay(1000);
		pxCtrl -> ucBlinkingFreq =(pxCtrl -> ucBlinkingFreq + 1);
		ucCounter++;
			if (ucCounter %2){
				pxCtrl -> ucLedNr = ((pxCtrl -> ucLedNr + 1) %4);
			}
			else{}
	}

	
}


int main( void )
{
	Led_Control Led = {1,0};
	Led_Init();
	xTaskCreate(LedBlink, NULL , 100 , &Led, 2 , NULL );
	xTaskCreate(LedCtrl, NULL , 100 , &Led, 2 , NULL );
	vTaskStartScheduler();
	while(1);
}

