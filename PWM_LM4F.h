#ifndef PWM_LM4F_H
	
	#if defined(TM4C123GH6PM)
			#ifndef TM4C123GH6PM_H
				#include "TM4C123GH6PM.h"
			#endif
	#elif defined(LM4F120H5QR)
			#ifndef LM4F120H5QR_H
				#include "LM4F120H5QR.h"
			#endif
	#endif
	
	#define PWM_LM4F_H
	
	
	void PWM0B_ini(int Duty)
	{
		//Set the Limits of the Duty 0 - 1000
		if (Duty > 1000)
		{Duty = 1000;}
		else if (Duty < 0)
		{Duty = 0;}
	
		SYSCTL->RCGCTIMER |= (1 << 0);				//Enable Clock to Timer 1
		SYSCTL->RCGCGPIO |= (1 << 5);				//Enable Clock to GPIO F
		
		TIMER0->CTL &= ~0xFF00;						//Reset Timer 0B before making changes.
		
		TIMER0->CFG = 0x04;							//Set the Timer to be in 16-bit mode.
		TIMER0->TBMR = 0x0A;						//Set the mode to be periodic and select the alternative functions(PWM) of the timer.
		
		TIMER0->TBILR = 0x3E8;						//Freqency = 16kHz
		TIMER0->TBMATCHR = ((1.0 - (Duty / 1000.0)) * TIMER0->TBILR) - 1;
			
		TIMER0->TBPR = 0x00;						//No Prescaller
		TIMER0->TBPMR = 0x00;						//No Prescaller
		
		TIMER0->CTL |= 0x0100;						//Re-enable the Timer and set the PWM to be non-inverted.
		
		//GPIO Settings...
		GPIOF->DEN |= (1 << 1);
		GPIOF->AFSEL |= (1 << 1);					//Enable Alternative Function for pin 1
		GPIOF->PCTL &= ~(0xFF << 4);				//Reset the the Functions
		GPIOF->PCTL |= (0x07 << 4);					//Select the Timer 1 function in the MUX		
	}
	void PWM1A_ini(int Duty)
	{
		//Set the Limits of the Duty 0 - 1000
		if (Duty > 1000)
		{Duty = 1000;}
		else if (Duty < 0)
		{Duty = 0;}
	
		SYSCTL->RCGCTIMER |= (1 << 1);				//Enable Clock to Timer 1
		SYSCTL->RCGCGPIO |= (1 << 5);				//Enable Clock to GPIO F
		
		TIMER1->CTL &= ~0xFF;						//Reset Timer 1A before making changes.
		
		TIMER1->CFG = 0x04;							//Set the Timer to be in 16-bit mode.
		TIMER1->TAMR = 0x0A;						//Set the mode to be periodic and select the alternative functions(PWM) of the timer.
		
		TIMER1->TAILR = 0x3E8;						//Freqency = 16kHz
		TIMER1->TAMATCHR = ((1.0 - (Duty / 1000.0)) * TIMER1->TAILR) - 1;
			
		TIMER1->TAPR = 0x00;						//No Prescaller
		TIMER1->TAPMR = 0x00;						//No Prescaller
		
		TIMER1->CTL |= 0x01;						//Re-enable the Timer and set the PWM to be non-inverted.
		
		//GPIO Settings...
		GPIOF->DEN |= (1 << 2);
		GPIOF->AFSEL |= (1 << 2);					//Enable Alternative Function for pin 2
		GPIOF->PCTL &= ~(0xFF << 8);				//Reset the the Functions
		GPIOF->PCTL |= (0x07 << 8);					//Select the Timer 1 function in the MUX		
	}
	
	
	void PWM0B_ChangeDuty(int Duty)
	{
		TIMER0->TBMATCHR = ((1.0 - (Duty / 1000.0)) * TIMER0->TBILR) - 1;
	}
	void PWM1A_ChangeDuty(int Duty)
	{
		TIMER1->TAMATCHR = ((1.0 - (Duty / 1000.0)) * TIMER1->TAILR) - 1;
	}	
#endif
