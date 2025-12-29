#include <LPC21xx.H>
#include "timer_interrupts.h"


static void (*ptrTimer0InterruptFunction)(void);
static void (*ptrTimer1InterruptFunction)(void);

//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer0IRQHandler()
{

	T0IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	ptrTimer0InterruptFunction();		// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}

__irq void Timer1IRQHandler()
{

	T1IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	ptrTimer1InterruptFunction();		// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer0Interrupts_Init(unsigned int uiPeriod, void (*pInterruptFunction)(void)) // microseconds
{
				// interupt function assigment
	ptrTimer0InterruptFunction = pInterruptFunction;
        // interrupts

	VICVectCntl0  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 0 interrupt channel
	VICVectAddr0  =(unsigned long)Timer0IRQHandler; 	   // Set to Slot 0 Address of Interrupt Service Routine 
	VICIntEnable |= (0x1 << VIC_TIMER0_CHANNEL_NR);            // Enable Timer 0 interrupt channel 
	
        // match module

	T0MR0 = 15000 * uiPeriod;                 	      // value 
	T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer
	//T0PR ^= 0x0;
	T0TCR |=  mCOUNTER_ENABLE; // start 
}

void Timer1Interrupts_Init(unsigned int uiPeriod, void (*pInterruptFunction)(void)) // microseconds
{
				// interupt function assigment
	ptrTimer1InterruptFunction = pInterruptFunction;
        // interrupts
	
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_TIMER1_CHANNEL_NR;  // Enable Slot 1 and assign it to Timer 0 interrupt channel
	VICVectAddr1  =(unsigned long)Timer1IRQHandler; 	   // Set to Slot 1 Address of Interrupt Service Routine 
	VICIntEnable |= (0x1 << VIC_TIMER1_CHANNEL_NR);            // Enable Timer 1 interrupt channel 
	
        // match module

	T1MR0 = 15000 * uiPeriod;                 	      // value 
	T1MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer
	//T1PR ^= 0x0;
	T1TCR |=  mCOUNTER_ENABLE; // start 

}
