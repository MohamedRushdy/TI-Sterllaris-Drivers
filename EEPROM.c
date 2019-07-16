#include "EEPROM.h"

unsigned int Counter_ = 0;
struct Commands List_of_Commands[200];

void EEPROM_ini(void)
{
	SYSCTL->RCGCEEPROM = 0x01;//Enable Clock Gating to EEPROM
	while(EEPROM->EEDONE & (1 << 0));//Wait till the EEPROM is working...
	
	EEPROM->EEBLOCK = 0x01;//Selecting Block 1
	if(EEPROM->EEBLOCK != 0)
	{
		//MEans it is accecable...
		EEPROM->EERDWR = 0xFEDCBA98;//Write this value...
	}	
}
void EEPROM_Read(void)
{
	SYSCTL->RCGCEEPROM = 0x01;//Enable Clock Gating to EEPROM
	while(EEPROM->EEDONE & (1 << 0));//Wait till the EEPROM is working...
	
	EEPROM->EEBLOCK = 0x01;//Selecting Block 1
	
	int X = 0;
	X = EEPROM->EERDWR;
	X++;
}
void Write_To_EEPROM(int8_t Block_Number,uint32_t * Data)
{
	int Counter = 0;
	
	EEPROM->EEBLOCK = Block_Number;
	EEPROM->EEOFFSET = 0x00;//Set the First Offset...
	
	while(EEPROM->EEDONE & (1 << 0));//Wait till the EEPROM is free...
	
	while(Counter < 16)
	{
		EEPROM->EEOFFSET = Counter;
		EEPROM->EERDWR = Data[Counter];//Write The Entery of the Data then the Register will increment
		
		if(Data[Counter + 1] == 0x00)//End of Array Character
		{
			//Means this is the end of string...
			//So We will fill the other data with 0...
			Counter++;
			while(Counter < 16)
			{
				EEPROM->EEOFFSET = Counter;
				EEPROM->EERDWR = 0x00;
				Counter++;
			}
			break;
		}
		
		Counter++;
		while(EEPROM->EEDONE & (1 << 0));//Wait till the EEPROM is free...
	}
	
}
uint32_t * Read_EEPROM(int8_t Block_Number)
{
	int8_t Counter = 0;
	uint32_t static Data[16];
	
	EEPROM->EEBLOCK = Block_Number;
	EEPROM->EEOFFSET = 0x00;//Set the First Offset...
	
	while(Counter < 16)
	{
		EEPROM->EEOFFSET = Counter;
		Data[Counter] = EEPROM->EERDWR;
		
		if(Data[Counter] == 0x00)
		{
			//Then This is the End of the Array and we should Exit
			break;
		}
		Counter++;
	}
	Data[Counter] = 0x00;
	
	return Data;
}
void Write_Commands(int8_t Array_Lenth,int8_t Block_Number)
{
	int Counter = 0,Word_Counter = 0;
	uint32_t Data;
	
	EEPROM->EEBLOCK = Block_Number;
	while(EEPROM->EEDONE & (1 << 0));//Wait till the EEPROM is free...
	EEPROM->EEOFFSET = Word_Counter++;//Set The Word Offset
	EEPROM->EERDWR = Array_Lenth;//Write The Lenth of the Array
		
	while(Counter < Array_Lenth)
	{
		//Prepare the Data First
		Data = ((List_of_Commands[Counter].Command & 0xFF) << 24);//Shift The Commands...
		Data |= List_of_Commands[Counter].Time & 0xFFFFFF;
		
		//Now Write To EEPROM
		while(EEPROM->EEDONE & (1 << 0));//Wait till the EEPROM is free...
		EEPROM->EEOFFSET = Word_Counter;//Set The Word Offset
		EEPROM->EERDWR = Data;//Write The Entery of the Data then the Register will increment
		
		Counter++;
		
		if(Word_Counter == 15)
		{
			Word_Counter = 0;
			Block_Number++;
			EEPROM->EEBLOCK = Block_Number;
		}
		else
		{Word_Counter++;}
	}
}
void Write_Command(char Command,int32_t Time)
{
  List_of_Commands[Counter_].Time = 0xFFFFFF & Time;
  List_of_Commands[Counter_].Command = Command;
  Counter_++;
}
void Write_All_Commands(int8_t Block_Number)
{
  Write_Commands(Counter_,Block_Number);
}
void Reset(void)
{
  Counter_ = 0;
  for(Counter_ = 0;Counter_ < 400;Counter_++)
  {
    List_of_Commands[Counter_].Time = 0x00;
    List_of_Commands[Counter_].Command = 0x00;
  }
  Counter_ = 0;
}