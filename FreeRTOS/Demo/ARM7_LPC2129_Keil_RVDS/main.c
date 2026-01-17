#include "FreeRTOS.h"
#include "task.h"
#include "keyboard.h"
#include "serwo.h"

//	#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )	--- informacja z pliku konfiguracyjnego -> 1 tick oznacza wtedy 1ms
// komentarz aby oddac zadanie 3 - po sprawdzeniu define w pliku konfiguracyjym wystarczy tylko zmienic servoinit na 200 w 

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
				Servo_Speed(80);
				Servo_GoTo(12);
				Servo_Wait(1000);
				Servo_Speed(40);
				Servo_GoTo(24);
				Servo_Wait(1000);
				Servo_Speed(20);
				Servo_GoTo(36);
				Servo_Wait(1000);
				Servo_Speed(10);
				Servo_GoTo(0);
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
