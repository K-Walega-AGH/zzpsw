#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

unsigned char ucBlinkingFreq = 1;


void LedBlink( void *pvParameters ){
	unsigned char *pucFreq = (unsigned char*)pvParameters;
	while(1){
		Led_Toggle(0);
		vTaskDelay((1000/(*pucFreq))/2);
	}
}

void LedCtrl (void *pvChangingFreq){
	unsigned char ucFreq = *((unsigned char*)pvChangingFreq);
	while(1){
		vTaskDelay(1000);
		ucFreq = ucFreq +1;
		ucBlinkingFreq = ucFreq;
	}

	
}


int main( void )
{
	Led_Init();
	xTaskCreate(LedBlink, NULL , 100 , &ucBlinkingFreq, 2 , NULL );
	xTaskCreate(LedCtrl, NULL , 100 , &ucBlinkingFreq, 2 , NULL );
	vTaskStartScheduler();
	while(1);
}

