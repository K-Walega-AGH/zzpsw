#include <LPC210X.H>
#include "uart.h"
#include "string.h"


/************ UART ************/

#define RECIEVER_SIZE 10
#define TRANSMITER_SIZE 60
#define TERMINATOR '\r'

// PIN CONNECT BLOCK
#define SET_P01_RXD 0x00000004
#define SET_P00_TXD 0x00000001

// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

////////////// Wlasne typy zmiennych ///////

struct RecieverBuffer
{
	char cData[RECIEVER_SIZE];
	unsigned char ucCharCtr;
	enum eRecieverStatus eStatus;
};

struct TransmiterBuffer
{
	char cData[TRANSMITER_SIZE];
	enum eTransmiterStatus eStatus;
	unsigned char fLastCharacter;
	unsigned char cCharCtr;
};

////////////// Zmienne globalne ////////////
char cOdebranyZnak;
struct RecieverBuffer sReciever_Buffer;
struct TransmiterBuffer sTransmiterBuffer;

////////////// Prototypy ////////////////

void Reciever_PutCharacterToBuffer(char cCharacter);
char Transmiter_GetCharacterFromBuffer(void);

///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
   // jesli przerwanie z odbiornika (Rx)
   
   unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
   {
     cOdebranyZnak = U0RBR;
		 Reciever_PutCharacterToBuffer(cOdebranyZnak);
   } 
   
   if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
   {
     if (sTransmiterBuffer.eStatus == BUSY)
		 {
			 U0THR = Transmiter_GetCharacterFromBuffer();
		 }
   }

   VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
/*void UART_InitWithInt(unsigned int uiBaudRate){

   // UART0
   PINSEL0 = PINSEL0 | SET_P01_RXD | SET_P00_TXD;                             // ustawic pina na odbiornik uart0
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = (((15000000)/16)/uiBaudRate);                      // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
   U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE;             // przerwanie, gdy jest mozliwy odczyt danych z rejestru rx buffer
	 U0IER  |= mTHRE_INTERRUPT_ENABLE;													// przerwanie, gdy jest mozliwe odczyt danych z rejestru rx buffer

   // INT
   VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
   VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
} */

void UART_InitWithInt(unsigned int uiBaudRate){
	
	unsigned long ulDivisor, ulWantedClock;
	ulWantedClock=uiBaudRate*16;
	ulDivisor=15000000/ulWantedClock;
	// UART
	PINSEL0 = PINSEL0 | 0x55;                                     // ustawic piny uar0 odbiornik nadajnik
	U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // d�ugosc s�owa, DLAB = 1
	U0DLL = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	ulDivisor >>= 8;
	U0DLM = ( unsigned char ) ( ulDivisor & ( unsigned long ) 0xff );
	U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
	U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE ;               

	// INT
	VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
	VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel

}

void Reciever_PutCharacterToBuffer(char cCharacter)
{
	if (cCharacter == TERMINATOR)
	{
		sReciever_Buffer.cData[sReciever_Buffer.ucCharCtr] = 0;
		sReciever_Buffer.ucCharCtr = 0;
		sReciever_Buffer.eStatus = READY;
	}
	else if (sReciever_Buffer.ucCharCtr == RECIEVER_SIZE-1)
	{
		sReciever_Buffer.eStatus = OVERFLOW;
	}
	else
	{
		sReciever_Buffer.cData[sReciever_Buffer.ucCharCtr] = cCharacter;
		sReciever_Buffer.ucCharCtr++;
	}
}

enum eRecieverStatus eReciever_GetStatus(void)
{
	return sReciever_Buffer.eStatus;
}

void Reciever_GetStringCopy(char * ucDestination)
{
	CopyString(sReciever_Buffer.cData, ucDestination);
	sReciever_Buffer.eStatus = EMPTY;
}

////////////////////////////////////////////////////////////////

char Transmiter_GetCharacterFromBuffer(void)
{
	if (sTransmiterBuffer.fLastCharacter == 1)
	{
		sTransmiterBuffer.eStatus = FREE;
		sTransmiterBuffer.fLastCharacter = 0;
		sTransmiterBuffer.cCharCtr = 0;
		return '\0';
	}
	else
	{
		if (sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr] == '\0')
		{
			sTransmiterBuffer.fLastCharacter = 1;
			return TERMINATOR;
		}
		else
		{
			return sTransmiterBuffer.cData[sTransmiterBuffer.cCharCtr++];
		}
	}
}

void Transmiter_SendString(char cString[])
{
	CopyString((char *)cString, sTransmiterBuffer.cData);
	sTransmiterBuffer.eStatus = BUSY;
	U0THR = sTransmiterBuffer.cData[0];
	sTransmiterBuffer.cCharCtr = 1;
}

enum eTransmiterStatus eTransmiter_GetStatus(void)
{
	return sTransmiterBuffer.eStatus;
}
