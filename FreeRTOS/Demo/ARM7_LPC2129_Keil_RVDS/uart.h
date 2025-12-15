
enum eRecieverStatus {EMPTY, READY, OVERFLOW};
enum eTransmiterStatus {FREE, BUSY};

void UART_InitWithInt(unsigned int uiBaudRate);

enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);

void Transmiter_SendString(char cString[]);
enum eTransmiterStatus eTransmiter_GetStatus(void);
