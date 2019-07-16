#ifndef UART7_H
	#include "LM4F120H5QR.h"
	#include <math.h>
    #include <String.h>
	#define UART7_H
	
	void UART7_ini(int Enable_ReceiveInterrupt);
	void UART7_SendChar(char Char);
	void UART7_SendNumber(float Number,int Precision);
	void UART7_SendString(char* String);
	char UART7_Receive(void);
	
#endif
