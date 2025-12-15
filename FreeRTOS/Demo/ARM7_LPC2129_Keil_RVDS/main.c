#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"
#include "semphr.h"
#include "stdio.h"

xSemaphoreHandle xSemaphore;

void Rtos_Transmiter_SendString (char cString[]){
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	Transmiter_SendString(cString);
	while (eTransmiter_GetStatus()!=FREE){};
	xSemaphoreGive(xSemaphore);
}

void LettersTx (void *pvParameters){
	char cBuffer[22];
	unsigned short int usiPassedTime=0;
	while(1){
		usiPassedTime = (unsigned short int)xTaskGetTickCount();
		Rtos_Transmiter_SendString(cBuffer);
		usiPassedTime = (unsigned short int)xTaskGetTickCount() - usiPassedTime;
		CopyString("-ABCDEEFGH-:",cBuffer);
		AppendUIntToString(usiPassedTime,cBuffer);
		AppendString("\n",cBuffer);
		vTaskDelay(300);
	}
}

void KeyboardTx (void *pvParameters) {
	enum KeyboardState eLastState = RELASED;
	
	while(1){
		if (eKeyboardRead() != RELASED && eLastState == RELASED){
			Rtos_Transmiter_SendString("-Keyboard-\n");
		}
		eLastState = eKeyboardRead();
		vTaskDelay(30);
	}
}

int main( void ){
	UART_InitWithInt(300);
	vSemaphoreCreateBinary(xSemaphore);
	
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	while(1);
}
