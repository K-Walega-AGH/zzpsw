#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"
#include "semphr.h"
#include "stdio.h"

#define QUEUE_LEN   5
#define STR_LEN     32

QueueHandle_t xQueue;

void UartTxTask(void *pvParameters)
{
    char cBuffer[STR_LEN];

    while (1)
    {
        if (xQueueReceive(xQueue, cBuffer, portMAX_DELAY) == pdTRUE)
        {
            Transmiter_SendString(cBuffer);
            while (eTransmiter_GetStatus() != FREE) {}
        }
    }
}

void LettersTx (void *pvParameters){
	char cBuffer[STR_LEN];
	unsigned int uiPassedTime=0;
	while(1){
		uiPassedTime = (unsigned int)xTaskGetTickCount();
		xQueueSend(xQueue, cBuffer, 0);
		uiPassedTime = (unsigned int)xTaskGetTickCount() - uiPassedTime;
		CopyString("-ABCDEEFGH-:",cBuffer);
		AppendUIntToString(uiPassedTime,cBuffer);
		AppendString("\n",cBuffer);
		vTaskDelay(300);
	}
}

void KeyboardTx (void *pvParameters) {
	enum KeyboardState eLastState = RELASED;
	char cBuffer[STR_LEN] = "-Keyboard-\n";
	while(1){
		if (eKeyboardRead() != RELASED && eLastState == RELASED){
			xQueueSend(xQueue, cBuffer, 0);
		}
		eLastState = eKeyboardRead();
		vTaskDelay(300);
	}
}

int main( void ){
	UART_InitWithInt(300);
	xQueue = xQueueCreate(QUEUE_LEN, STR_LEN);
	xTaskCreate(UartTxTask,  NULL, 128, NULL, 2, NULL);
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
	vTaskStartScheduler();
	while(1);
}
