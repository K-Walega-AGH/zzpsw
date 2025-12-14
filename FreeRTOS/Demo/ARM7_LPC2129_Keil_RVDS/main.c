#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "string.h"
#include "keyboard.h"
#include "semphr.h"

xSemaphoreHandle xSemaphore;

void RtosTransmitterSendString(void){
	while (Transmiter_GetStatus()!=FREE){		
		
	};
	xSemaphoreGive(xSemaphore);
	vTaskDelay(300);

}

void LettersTx (void *pvParameters){
while(1){
	if ((xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE )) {
		Transmiter_SendString("-ABCDEEFGH-\n");
		RtosTransmitterSendString();
		}
	}
}

void KeyboardTx (void *pvParameters){
while(1){
		if((eKeyboardRead() != RELASED) && (xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE )){
			Transmiter_SendString("-Keyboard-\n");
			RtosTransmitterSendString();
		}
	}
}


int main( void ){
UART_InitWithInt(300);
KeyboardInit();
vSemaphoreCreateBinary( xSemaphore );
xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL );
vTaskStartScheduler();
while(1);
}
