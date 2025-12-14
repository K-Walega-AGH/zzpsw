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


void Pulse_LED_0 (void *pvParameters){
	while(1){
			if(xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE ){
				LedOn(0);
				vTaskDelay(100);
				LedOff(0);

		}		
	}
}

void Pulse_LED_1 (void *pvParameters){
	while(1){
			if(xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE ){
				LedOn(1);
				vTaskDelay(100);
				LedOff(1);

		}		
	}
}


int main( void )
{
	vSemaphoreCreateBinary( xSemaphore );
	Led_Init();
	xTaskCreate(PulseTrigger, NULL , 100 ,NULL , 2 , NULL );
	xTaskCreate(SecondPulseTrigger, NULL , 100 ,NULL , 2 , NULL );
	xTaskCreate(Pulse_LED_0, NULL ,100 ,NULL ,2 ,NULL);
	xTaskCreate(Pulse_LED_1, NULL ,100 ,NULL ,2 ,NULL);
	vTaskStartScheduler();
	while(1);
}

