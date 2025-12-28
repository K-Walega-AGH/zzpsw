#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"
#include "semphr.h"
#include "stdio.h"
#include "led.h"


xSemaphoreHandle xSemaphore;

#define QUEUE_LEN   5
#define STR_LEN     32

QueueHandle_t xQueue;



void Rtos_Transmiter_SendString (void *pvParameters){
	while(1){
		char cBuffer[STR_LEN];
		if(xQueueReceive(xQueue, cBuffer, portMAX_DELAY) == pdPASS){
				xSemaphoreTake(xSemaphore, portMAX_DELAY);
				Transmiter_SendString(cBuffer);
				while (eTransmiter_GetStatus()!=FREE){};
				xSemaphoreGive(xSemaphore);
		}
	}
}

void LettersTx (void *pvParameters){
	char cBuffer[STR_LEN];
	unsigned int uiPassedTime=0;
	while(1){
		uiPassedTime = (unsigned int)xTaskGetTickCount();
		if (xQueueSend(xQueue, cBuffer, 0) == errQUEUE_FULL){
			Led_Toggle(0);
		}
		uiPassedTime = (unsigned int)xTaskGetTickCount() - uiPassedTime;
		CopyString("-ABCDEEFGH-:",cBuffer);
		AppendUIntToString(uiPassedTime,cBuffer);
		AppendString("\n",cBuffer);		
		vTaskDelay(1000);
	}
}

void KeyboardTx (void *pvParameters) {
	enum KeyboardState eLastState = RELASED;
	while(1){
		if (eKeyboardRead() != RELASED && eLastState == RELASED){
			xQueueSend(xQueue, "-Keyboard-\n", portMAX_DELAY);
		}
		eLastState = eKeyboardRead();
		vTaskDelay(50);
	}
}

int main( void ){
	Led_Init();
	UART_InitWithInt(300);
	vSemaphoreCreateBinary(xSemaphore);
	xQueue = xQueueCreate(QUEUE_LEN, STR_LEN);
	
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(Rtos_Transmiter_SendString, NULL, 128, NULL, 1, NULL );

	vTaskStartScheduler();
	while(1);
}
