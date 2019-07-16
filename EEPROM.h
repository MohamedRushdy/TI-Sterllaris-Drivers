#ifndef __EEPROM_H__
	
	#ifndef __LM4F120H5QR_H__
		#include "LM4F120H5QR.h"
	#endif
	
	#define __EEPROM_H__
	
	void EEPROM_ini(void);
	void Write_To_EEPROM(int8_t Block_Number,uint32_t * Data);
	void Write_Command(char Command,int32_t Time);
	void Write_All_Commands(int8_t Block_Number);
        void Reset(void);
        
	struct Commands
	{
		char Command;
		int32_t Time;
	};
#endif