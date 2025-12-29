#ifndef TIMER_INTERRUPTS_H
#define TIMER_INTERRUPTS_H

#define mCOUNTER_ENABLE 0x00000001
#define mCOUNTER_RESET  0x00000002

// CompareMatch
#define mINTERRUPT_ON_MR0 0x00000001
#define mRESET_ON_MR0     0x00000002
#define mMR0_INTERRUPT    0x00000001

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_TIMER0_CHANNEL_NR 4
#define VIC_TIMER1_CHANNEL_NR 5

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020

/**********************************************/

void Timer0Interrupts_Init(unsigned int uiPeriod, void (*pInterruptFunction)(void));
void Timer1Interrupts_Init(unsigned int uiPeriod, void (*pInterruptFunction)(void));

#endif //TIMER_INTERRUPTS_H
