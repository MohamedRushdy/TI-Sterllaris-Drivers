#include "SPI3.h"

void SPI3_Enable_Interrupt(void);

void SPI0_ini(int Master,unsigned long Baud_Rate)
{
  SYSCTL->RCGCSSI |= 0x04u;//Enable Clock to SSI3
  SYSCTL->RCGCGPIO |= (1 << 3)//Enable Clock to Port D
  
  //First Disable The SPI Protocol to make changes
  SSI3->CR1 &= ~(0x02u);
  
  SSI3->CR0 = (0x07); //8-Bit , Set to SPI - Low - On leading Edge
  
  //Set Master and Slave
  if( Master == 1)
  {
    //Edit this to be available for all baud rates...
    int CPSR,CR0=256,Result;
    Result = (int)(16000000UL / Baud_Rate);
    while(CR0 > 225)
    {
      CPSR += 2;
      CR0 = (int)floor(Result / (unsigned long)CPSR) - 1;
    }
  
    SSI3->CPSR = CPSR;
    SSI3->CC = 0x00;  //Use System Clock...
    SSI3->CR0 |= (CR0 << 8);
    SSI3->CR1 &= ~(0x04);
  }
  else
  {
    SSI3->CR1 |= (0x04);
    SSI3_Enable_Interrupt();
  }
  
  //Set GPIO to be SPI 3
  GPIOD->AFSEL |= (0x0F);//Enable Alternative Functions for PD0,PD1,PD2,PD3
  GPIOD->PCTL |= (0x1111);//Select the ulternate functions to be SSI 3
  GPIOD->DEN |= (0x0F);//Enable Digital Pins
  
  //Enable SPI
  SSI3->CR1 |= (unsigned char)(0x02);
}
unsigned char SPI0_Send(char Data)
{
  //First Set the Slave Select To be Low...
  //GPIOD->DATA &= ~(1 << 3);
  //while(!(SSI3->SR & (1 << 1)));//Until FIFO not Full
  SSI3->DR = (unsigned char) Data;
  while(!(SSI3->SR & 0x10));//Until FIFO is empty
  //GPIOA->DATA |= (1 << 3);
  return SSI3->DR;        
}
void SSI3_Enable_Interrupt()
{
  NVIC->IP[58] = (5 << 5);//Set NVIC Priotity
  NVIC->ISER[1] |= (1 << 27);//Enable the coresponding NVIC Bit fot the SSI3
  
  SSI3->IM = 0x0Bu;//Set the Interrupt mask of only the Receiving to be enabled
}