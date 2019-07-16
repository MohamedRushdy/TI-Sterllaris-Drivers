#ifndef _I2C1_H

	#if defined(TM4C123GH6PM)
			#ifndef TM4C123GH6PM_H
				#include "TM4C123GH6PM.h"
			#endif
	#elif defined(LM4F120H5QR)
			#ifndef LM4F120H5QR_H
				#include "LM4F120H5QR.h"
			#endif
	#endif
	
	#define I2C1_H

void I2C1_ini(int Mode,int Address)
{
	int volatile dump = 0;
	SYSCTL->RCGCI2C |= (1 << 1);
	for(dump = 0;dump <= 10;dump++);
	
	I2C1->MTPR = 0x07;//Setting it to standard 100kbps and High speed disabled
	
	if(Mode == 0)
	{		
		I2C1->MCR = 0x10;//Setting master only
	}
	else
	{
		//Means Slave Mode Active
		if(Mode == 1)
		{I2C1->MCR = 0x20;}//Set Slave functions only
		else if(Mode == 2)
		{I2C1->MCR = 0x30;}//Master and Slave Together
		
		
		I2C1->SOAR = Address;//Set the address of the device on the bus system
		I2C1->SACKCTL = 0x00;//Disable ACK Control
		I2C1->SIMR = 0x01;//Enable Data interrupt Mask
		I2C1->SICR = 0x01;//Clear Interrupt Flag
		
		NVIC->ISER[1] |= (1 << 5);//Enable I2C1 mask in NVIC(no. 35)
		NVIC->IP[37] = (3 << 5);//Set Priority
		
		I2C1->SCSR = 0x01;//Enable the Device as Slave
		dump = I2C1->SCSR;//force writing by reading the register...
	}
	
	
	//Preparing the GPIO Pins...
	SYSCTL->RCGCGPIO |= 0x01;//Enable Clock to Port A
	for(dump = 0;dump <= 10;dump++);
	
	GPIOA->DEN |= 0xC0;//Pins 6 and 7
	GPIOA->AFSEL |= 0xC0;//Select alternative functions for pins 6,7
	GPIOA->PCTL &= ~0xFF000000;//Clear the Previous data
	GPIOA->PCTL |= 0x33000000;//Set the pins 6,7 as I2C pins
	GPIOA->ODR |= 0x80;//Pin 7 as open drain ???
}
char I2C1_Master(unsigned int Receiver,int Address,int Data)
{	
	I2C1->MSA = Receiver;//Set the first bit of according to the state provide by user (Transmit/Receive)
	I2C1->MSA |= (Address << 1);//Setting the Address of the Slave
	
	if(Receiver == 0)
	{
		//Transmitter Mode:
		I2C1->MDR = Data;//Put the data before Sending Process
		
		I2C1->MCS = 0x0B;//Run and Start signal
		int volatile dump = I2C1->MCS;//Force read to make sure the bit is wrtten and data is starting...
		
		while(I2C1->MCS & (1 << 0));//Wait till busy is done...
		
		if(I2C1->MCS & (1 << 2))
		{
			//Error That the Slave address wasn't Found...
			while(1);
		}
		else if(I2C1->MCS & (1 << 3))
		{
			//Data Was not Acknoleaged.
			while(1);
		}
		
		//Means everything is fine...
		I2C1->MCS = 0x04;//Stop Signal
		dump = I2C1->MCS;//Force read to make sure the bit is wrtten and data is starting...
	}
	else
	{
		unsigned char Received_Char = 0x00;
	
		I2C1->MSA = (Address << 1);//Setting the address and transmit mode.
		I2C1->MSA |= (1 << 0);//Making the slave transmit data
		
		I2C1->MCS = 0x0B;//Run and Start signal
		int volatile dump = I2C1->MCS;//Force read to make sure the bit is wrtten and data is starting...
		
		while(I2C1->MCS & (1 << 0));//Wait till busy is done...	
		Received_Char = I2C1->MDR;//Put the received data in a local variable
			
		I2C1->MCS = 0x04;//Stop Signal
		dump = I2C1->MCS;//Force read to make sure the bit is wrtten and data is starting...
		
		return Received_Char;
	}
	
	return 0;
}
char* I2C1_Master_MultipleData(unsigned int Receiver,int Address,char* Data)
{
	int Counter = 0;
	int volatile dump = 0;
	
	I2C1->MSA = Receiver;//Set the first bit of according to the state provide by user (Transmit/Receive)
	I2C1->MSA |= (Address << 1);//Setting the Address of the Slave
	
	if(Receiver == 0)
	{
		//Transmitter Mode:
		I2C1->MDR = Data[0];//Put the data before Sending Process
		
		I2C1->MCS = 0x0B;//Run and Start signal
		dump = I2C1->MCS;//Force read to make sure the bit is wrtten and data is starting...
		
		while(1)
		{
			while(I2C1->MCS & (1 << 0));//Wait till busy is done...
			
			if(I2C1->MCS & (1 << 2))
			{
				//Error That the Slave address wasn't Found...
				while(1);
			}
			else if(I2C1->MCS & (1 << 3))
			{
				//Data Was not Acknoleaged.
				while(1);
			}
			else
			{
				//Means everything is fine...
				Counter++;
				if(Data[Counter] == '\0')
				{
					//Means End of Array(i.e. end of Data)
					break;
				}
				else
				{
					I2C1->MDR = Data[Counter];//Put the data before Sending Process
		
					I2C1->MCS = 0x09;//Run and Start signal
					dump = I2C1->MCS;//Force read to make sure the bit is wrtten and data is starting...
				}
			}
		}
		
		I2C1->MCS = 0x04;//Stop Signal
		dump = I2C1->MCS;//Force read to make sure the bit is wrtten and data is starting...
	}
	else
	{
		char Received_Data[8];
		unsigned char Received_Char = 0x00;
		int Counter = 0;
				
		I2C1->MCS = 0x0B;//Run and Start signal
		int volatile dump = I2C1->MCS;//Force read to make sure the bit is wrtten and data is starting...
			
		while(1)
		{
			while(I2C1->MCS & (1 << 0));//Wait till busy is done...	
			Received_Char = I2C1->MDR;//Put the received data in a local variable
			
			if(Received_Char != 0xFF)
			{
				Received_Data[Counter++] = Received_Char;
				I2C1->MCS = 0x09;//Run again with another ACK for slave to transmit next byte
			}
			else
			{break;}
		}
		
		I2C1->MCS = 0x04;//Stop Signal
		dump = I2C1->MCS;//Force read to make sure the bit is wrtten and data is starting...
		return Received_Data;
	}
	
	return 0;
}
char I2C1_Slave_Receive(void)
{
	if (I2C1->SCSR & (1 << 0))
	{
		return (I2C1->SDR);//Return the Data Received...
	}
}
void I2C1_Slave_Send(char* Data_TobeSent)
{
	if(I2C1->SCSR & (1 << 1))
		{
			//Slave Transmitter Mode Active
			int Counter = 0;
			while(Data_TobeSent[Counter + 1] != '\0')
			{
				I2C1->SDR = Data_TobeSent[Counter++];
				while(!(I2C1->SCSR & (1 << 1)));
			}
		}
}
#endif