#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

TaskHandle_t xLedBlinkHandle = NULL;



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
	
	while(1){
		vTaskSuspend(xLedBlinkHandle);
		vTaskDelay (1000);
		vTaskResume(xLedBlinkHandle);
		vTaskDelay (1000);

	}
	

	
}




int main( void )
{
	Led_Control Led = {1,0};
	Led_Init();
	xTaskCreate(LedBlink, NULL , 100 , &Led, 2 , &xLedBlinkHandle );
	xTaskCreate(LedCtrl, NULL ,100 ,&Led ,2 ,NULL);
	vTaskStartScheduler();
	while(1);
}

