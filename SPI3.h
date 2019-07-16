#ifndef __SPI3_H__
	#define SPI3_H
	
	#ifndef LMF4120H5QR_H
		#include "LMF4120H5QR.h"
	#endif
	
	void SPI3_ini(int Master,unsigned long Baud_Rate);
	char SPI3_Send(char Data);
#endif
	