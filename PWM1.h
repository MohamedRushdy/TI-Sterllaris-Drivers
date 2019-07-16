#ifndef _PWM1_H
	#define PWM1_H
	
	#if defined(TM4C123GH6PM)
			#ifndef TM4C123GH6PM_H
				#include "TM4C123GH6PM.h"
			#endif
	#elif defined(LM4F120H5QR)
			#ifndef LM4F120H5QR_H
				#include "LM4F120H5QR.h"
			#endif
	#endif
	

void PWM1_3A_ini(float Duty)
{
  int volatile dump = 0;
	
  SYSCTL->RCGCPWM |= (1 << 1);			//Enable The Clock to PWM Module
  SYSCTL->RCC &= ~0x0010000;				//Disable PWM Divider
  for(dump = 0;dump <= 30;dump++);
  
  PWM1->_3_CTL = 0x00;							//Diable the generator before making changes
  PWM1->_3_CTL &= ~(1 << 1);				//Mode is down counter
  
  int Load = 0x1000;
  PWM1->_3_LOAD = Load;							//Freq = 1Mhz
  PWM1->_3_CMPA = Load - (int)((Duty/100) * (float)Load);//Half Duty Cycle
  PWM1->_3_GENA = 0x8C;							//High on Load and Zero on Compare match A down.
  
  
  //Now Prepare the GPIOs
  SYSCTL->RCGCGPIO |= (1 << 5);			//Enable Clock to port F
  for(dump = 0;dump <= 30;dump++);
  
  GPIOF->DEN |= (1 << 2);
  GPIOF->AFSEL |= (1 << 2);					//Enable Alternative function for pin 2
  GPIOF->PCTL &= ~(0xF << 8);
  GPIOF->PCTL |= (0x5 << 8);
  
  //Now Enable The Generator
  PWM1->_3_CTL |= (1 << 0);					//Enable the Genertator
  PWM1->ENABLE |= 0x40;							//Enable Output to channel 6
}
void PWM1_3B_ini(float Duty)
{
	int volatile dump = 0;
	
  SYSCTL->RCGCPWM |= (1 << 1);			//Enable The Clock to PWM Module
  SYSCTL->RCC &= ~0x0010000;				//Disable PWM Divider
  for(dump = 0;dump <= 30;dump++);
  
  PWM1->_3_CTL = 0x00;							//Diable the generator before making changes
  PWM1->_3_CTL &= ~(1 << 1);				//Mode is down counter
  
  int Load = 0x1000;
  PWM1->_3_LOAD = Load;							//Freq = 1Mhz
  PWM1->_3_CMPB = Load - (int)((Duty/100) * (float)Load);
  PWM1->_3_GENB = 0x80C;						//High on Load and Zero on Compare match A down.
  
  
  //Now Prepare the GPIOs
  SYSCTL->RCGCGPIO |= (1 << 5);			//Enable Clock to port F
  for(dump = 0;dump <= 30;dump++);
  
  GPIOF->DEN |= (1 << 3);
  GPIOF->AFSEL |= (1 << 3);					//Enable Alternative function for pin 2
  GPIOF->PCTL &= ~(0xF << 12);
  GPIOF->PCTL |= (0x5 << 12);
  
  //Now Enable The Generator
  PWM1->_3_CTL |= (1 << 0);					//Enable the Genertator
  PWM1->ENABLE |= 0x80;							//Enable Output to channel 6
}
void PWM1_2A_ini(float Duty)
{
  int volatile dump = 0;
	
  SYSCTL->RCGCPWM |= (1 << 1);			//Enable The Clock to PWM Module
  SYSCTL->RCC &= ~0x0010000;				//Disable PWM Divider
  for(dump = 0;dump <= 30;dump++);
  
  PWM1->_2_CTL = 0x00;							//Diable the generator before making changes
  PWM1->_2_CTL &= ~(1 << 1);				//Mode is down counter
  
  int Load = 0xFF;
  PWM1->_2_LOAD = Load;							//Freq = 1Mhz
  PWM1->_2_CMPA =Load - (int)((Duty/100) * (float)Load);
  PWM1->_2_GENB = 0x8C;							//High on Load and Zero on Compare match A down.
  
  
  //Now Prepare the GPIOs
  SYSCTL->RCGCGPIO |= (1 << 5);			//Enable Clock to port F
  for(dump = 0;dump <= 30;dump++);
  
  GPIOF->DEN |= (1 << 1);
  GPIOF->AFSEL |= (1 << 1);					//Enable Alternative function for pin 1
  GPIOF->PCTL &= ~(0xF << 4);
  GPIOF->PCTL |= (0x5 << 4);
  
  //Now Enable The Generator
  PWM1->_2_CTL |= (1 << 0);					//Enable the Genertator
  PWM1->ENABLE |= 0x20;							//Enable Output to channel 5
}

void PWM1_2A_SetValue(int Value)
{
	PWM1->_2_CMPA = 0xFF - Value;
}
void PWM1_3A_SetValue(int Value)
{
	PWM1->_3_CMPA = 0xFF - Value;
}
void PWM1_3B_SetValue(int Value)
{
	PWM1->_3_CMPB = 0xFF - Value;
}

#endif
