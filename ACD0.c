#include "ADC0.h"

void ADC0_S3_ini(void)
{
  int volatile dump = 0;
  
  SYSCTL->RCGCADC |= (1 << 0);  //Enable Clock to ADC 2 Module
  SYSCTL->RCGCGPIO |= (1 << 4); //Enable Clock to port E
  
	SYSCTL->RCC = 0x078E3AD1;												//Reset the Run-Mode Clock to the normal values
  SYSCTL->RCC2 = 0x07C06810;											//Reset the Run-Mode Clock 2 to the normal values
  
  ADC0->ACTSS &= ~(1 << 3);     //Disable The SS3 before making any changes
  ADC0->EMUX &= ~(0xF << 12);   //Select the Processor (Default) Trigger through Software
  ADC0->SSMUX3 = 0x02;          //Selecting the pin AIN2 (PE1) to be the input for this SS
  ADC0->ISC |= (1 << 3);        //Clear the Flag
  
  ADC0->SSCTL3 = 0x06;          //Single Mode,End-Of-Sequence,Interrupt Enable,No Temperature Reading
  
  //Now Prepare the GPIO Ports 
  GPIOE->DEN &= ~(1 << 1);      //Disable the digital enable of the pin
  GPIOE->AFSEL |= (1 << 1);     //Select Alternative Function for PE1
  GPIOE->AMSEL |= (1 << 1);    //Select Analog Mode to disable the isolation
 
  
  ADC0->ACTSS |= (1 << 3);      //Enable the SS3...
}
int Read_ADC0_SS3(void)
{
  ADC0->PSSI |= (1 << 3);
  while(! (ADC0->RIS & (1 << 3)));
  ADC0->ISC |= (1 << 3);        //Clear the Flag
  return ADC0->SSFIFO3;
}
