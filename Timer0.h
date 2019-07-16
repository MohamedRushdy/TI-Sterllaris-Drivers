#ifndef __TIMER0_H__
	#if defined(TM4C123GH6PM)
			#ifndef TM4C123GH6PM_H
				#include "TM4C123GH6PM.h"
			#endif
	#elif defined(LM4F120H5QR)
			#ifndef LM4F120H5QR_H
				#include "LM4F120H5QR.h"
			#endif
	#endif
	
	#include <math.h>
    #define TIMER0_H
	

void Timer0A_ini(unsigned int Period_InMillis)
{
  SYSCTL->RCGCTIMER |= (1 << 0);                                        //Enable Clock To Timer 0...
  int dump = SYSCTL->RCGCTIMER;                                         //The read is to force the writing of the previous statement
  
  TIMER0->CTL &= ~0x01;                                                 //Disable the Timer Before changing the settings
  TIMER0->CFG = 0x04;                                                   //Making the timer 16-bit use
  
  TIMER0->IMR |= 0x01;                                                  //Enable The Maskt of the Time out interrupt
  TIMER0->TAMR = 0x02;                                                  //Periodic,Interrupt Match Disabled...
  
  unsigned int Prescaller = (unsigned int)floor((Period_InMillis * 16000) /  65536) + 1;
  TIMER0->TAPR = Prescaller - 1;
  
  if(Prescaller != 1)
  {TIMER0->TAILR = ((Period_InMillis * 16000) / Prescaller);}           //Load intp the timer
  else
  {TIMER0->TAILR = (Period_InMillis * 16000);}
  
  //Now Enable the interrupt in the NVIC
  NVIC->IP[19] = (3 << 5);                                              //Set Priority to 3
  NVIC->ISER[0] |= (1 << 19);                                           //Set the NVIC bit to 1
  
  TIMER0->CTL |= 0x01;                                                  //Enable The Timer Again
}
void Timer0B_ini(unsigned int Period_InMillis)
{
  SYSCTL->RCGCTIMER |= (1 << 0);                                        //Enable Clock To Timer 0...
  int dump = SYSCTL->RCGCTIMER;                                         //The read is to force the writing of the previous statement
  
  TIMER0->CTL &= ~0x100;                                                //Disable the Timer Before changing the settings
  TIMER0->CFG = 0x04;                                                   //Making the timer 16-bit use
  
  TIMER0->IMR |= 0x100;                                                 //Enable The Maskt of the Time out interrupt
  TIMER0->TBMR = 0x02;                                                  //Periodic,
  
  unsigned int Prescaller = (unsigned int)floor((Period_InMillis * 16000) /  65536) + 1;
  TIMER0->TBPR = Prescaller - 1;
  
  if(Prescaller != 1)
  {TIMER0->TBILR = ((Period_InMillis * 16000) / Prescaller);}           //Load intp the timer
  else
  {TIMER0->TBILR = (Period_InMillis * 16000);}
    
  //Now Enable the interrupt in the NVIC
  NVIC->IP[20] = (3 << 5);                                              //Set Priority to 3
  NVIC->ISER[0] |= (1 << 20);                                           //Set the NVIC bit to 1
  
  TIMER0->CTL |= 0x100;                                                 //Enable The Timer Again  
}
void Timer0_Delay(unsigned long millis)
{
  unsigned long Max_16 = (65536 * 256) / (16000);       //Max of 16-bit Timer in ms
  unsigned long Max_32 = (4294967296 * 256) / (16000);  //Max of 32-bit Timer in milliseconds 
  unsigned int Prescaller = 0;
  
  SYSCTL->RCGCTIMER |= (1 << 0);                        //Enable Clock To Timer 0...
  int dump = SYSCTL->RCGCTIMER;                        //The read is to force the writing of the previous statement
  
    
  //Check the max to see if this needs 16-bit or 32-bit...
  if(millis <= Max_16)
  {
    TIMER0->CTL &= ~0x001;                              //Disable only the A timer Before changing the settings
    TIMER0->CFG = 0x04;                                 //Timer 16-bit mode
    Prescaller = (int)floor((millis * 16000) / 65536) + 1;
  }
  else if (millis <= Max_32)
  {
    TIMER0->CTL &= ~0x101;                              //Disable both timers Before changing the settings
    TIMER0->CFG = 0x00;                                 //32-Bit Timer Mode
    Prescaller = (int)floor((millis * 16000) / 4294967296) + 1;
  }
  
  //Set The Prescaller
  if (Prescaller != 0)
  {
    TIMER0->TAPR = Prescaller - 1;
    TIMER0->TAILR = ((millis * 16000) / Prescaller);
  }
  else
  {
    TIMER0->TAPR = 0;
    TIMER0->TAILR = ((millis * 16000));
  }
  
  TIMER0->TAMR = 0x01;                          //One Shot Timer,Edge-Count,Down Counter
  TIMER0->ICR |= (1 << 0);                      //Clear The Interrupt Flag
  //TIMER0->IMR |= 0x01;                        //Enable The Maskt of the Time out interrupt
  
  
  TIMER0->CTL |= 0x001;                         //Enable Both Timers
  
  
  //Finally Wait till the period is over
  while(!(TIMER0->RIS & (1 << 0)));
  TIMER0->ICR |= (1 << 0);
}
#endif
