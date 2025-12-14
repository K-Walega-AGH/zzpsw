
enum eRecieverStatus {EMPTY, READY, OVERFLOW};

void UART_InitWithInt(unsigned int uiBaudRate);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
