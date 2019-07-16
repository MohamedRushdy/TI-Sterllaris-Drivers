#ifndef UART0_H
	#include "LM4F120H5QR.h"
	#include <math.h>
    #include <String.h>
	#define UART0_H
	
	void UART0_ini(int Enable_ReceiveInterrupt);
	void UART_SendChar(char Char);
	void UART_SendNumber(float Number,int Precision);
	void UART_SendString(char* String);
	char UART_Receive(void);
	
#endif