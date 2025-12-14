#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"

xSemaphoreHandle xSemaphore;

void PulseTrigger (void *pvParameters){
	while(1){
		vTaskDelay(1000);
		xSemaphoreGive(xSemaphore);

	}
}

void SecondPulseTrigger (void *pvParameters){
	vTaskDelay(333);
	while(1){
		vTaskDelay(333);
		xSemaphoreGive(xSemaphore);

	}
}


void Pulse_LED(void *pvParameters){
	unsigned char ucLedCtr = *((unsigned char*)pvParameters);
	while(1){
			if(xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE ){
				LedOn(ucLedCtr);
				vTaskDelay(100);
				LedOff(ucLedCtr);

		}		
	}	
}



int main( void )
{
	static unsigned char ucLed_0 = 0;
	static unsigned char ucLed_1 = 1;
	vSemaphoreCreateBinary( xSemaphore );
	Led_Init();
	xTaskCreate(PulseTrigger, NULL , 100 ,NULL , 2 , NULL );
	xTaskCreate(SecondPulseTrigger, NULL , 100 ,NULL , 2 , NULL );
	xTaskCreate(Pulse_LED, NULL ,100 ,&ucLed_0 ,2 ,NULL);
	xTaskCreate(Pulse_LED, NULL ,100 ,&ucLed_1 ,2 ,NULL);
	vTaskStartScheduler();
	while(1);
}

