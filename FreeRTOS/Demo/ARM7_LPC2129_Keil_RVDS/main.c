#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "serwo.h"

//	#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )	--- informacja z pliku konfiguracyjnego -> 1 tick oznacza wtedy 1ms

void Keyboard (void *pvParameters){
	enum KeyboardState eCurrentState;
	enum KeyboardState ePreviousState = RELASED;
	while(1){
		eCurrentState = eKeyboardRead();
		if((eCurrentState != RELASED) && (ePreviousState == RELASED)){
			switch(eCurrentState){
				case BUTTON_0: 
				Servo_Callib();
				break;
				
				case BUTTON_1:
				Servo_GoTo(50);
				break;
				
				case BUTTON_2: 
				Servo_GoTo(100);
				break;
				
				case BUTTON_3: 
				Servo_GoTo(150);
				break;
			
				default: 
				break;
			}
		}
		ePreviousState = eCurrentState;
		vTaskDelay(100);
	}
}



int main( void ){
	__enable_irq();
	KeyboardInit();
	Servo_Init(200);
	xTaskCreate(Keyboard, NULL, 128, NULL, 1, NULL);
	vTaskStartScheduler();
	while(1);
}
