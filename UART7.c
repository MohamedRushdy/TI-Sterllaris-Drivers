#include "UART7.h"

void UART7_ini(int Enable_ReceiveInterrupt)
{
  int volatile dump = 0;
  
  SYSCTL->RCGCUART |= (1u << 7);                //Enable UART Clock in main register
  for(dump = 0;dump <= 3;dump++);               //Wait loop because 3 system clock cycles are required before accessing the module
  
  UART7->CTL = 0;                               //Disable The UART before making settings...
  
  //Set the Baud Rate
  UART7->IBRD = 104;
  UART7->FBRD = 11;
  UART7->LCRH = (0x03 << 5);                    //Word Lenth 8-bit
  
  if(Enable_ReceiveInterrupt == 1)
  {
    UART7->IM |= (1u << 4);                     //Enable UART Receive Interrupt Mask
    NVIC->IP[63] = (3 << 5);                    //Set Priority to 3
    NVIC->ISER[1] |= (1u << 31);                 //Enable the interrupt in the NVIC
  }
  
  //Prepare the GPIO Pins
  SYSCTL->RCGCGPIO |= (1u << 4);                //Enable PORTE Clock in main register  
  for(dump = 0;dump <= 3;dump++);               //Wait because it is required that the system passes 3 clock cycles before it can access the module
  
  GPIOE->DEN = (0x03 << 0);                     //Enable Digital on pins A0 & A1
  GPIOE->AFSEL = (0x03 << 0);                   //Alternate Functions for pins A0&A1
  GPIOE->PCTL = (0x01 << 0x00) | (0x01 << 4);   //Enable UART for E0&E1
 
  UART7->CTL = (1 << 0) | (11 << 8);            //Enable UART , TX&RX  
}
void UART7_SendChar(char Char)
{
  while(UART7->FR & (1 << 5));  //Wait until the flag indicates that transmission is avauilable
  UART7->DR = Char;
  while(UART7->FR & (1 << 5));  //Wait until the flag indicates that transmission is done
}
void UART7_SendNumber(float Number,int Precision)
{
  //First Get The Count of digits of the number...
  int Counter = 0;
  int Test_Number = (int)Number;

  while(Test_Number > 0)
  {
    Test_Number /= 10;
    Counter++;
  }

  //Now Prepare for the Array...
  char Number_AsChar[38];
  int const Start = Counter + Precision;
  
  while(Counter > 0)
  {
    Test_Number = (int)(Number / pow(10,Counter - 1));
    Number_AsChar[Counter + Precision] = (char)(Test_Number + 48);
    Number = Number - (float)(Test_Number * pow(10,Counter - 1));
    Counter--;
  }
  Number_AsChar[Precision--] = '.';

  while(Precision >= 0)
  {
    Test_Number = (int)(Number * 10.0);
    Number_AsChar[Precision] = (char)(Test_Number + 48);
    Number = Number * 10.0 - ((float)(Test_Number));
    Precision--;
  }
  
  //Now Print the Output...
  Counter = Start;
  while(Counter >= 0)
  {
    UART7_SendChar(Number_AsChar[Counter--]);
  }
}
void UART7_SendString(char* String)
{
  int Counter = 0;
  int ArraySize = strlen(String);
  while(Counter < ArraySize)
  {
    while(UART7->FR & (1 << 5));  //Wait until the flag indicates that transmission is avauilable
    UART7->DR = String[Counter];
    Counter++;    
    
  }
}
char UART7_Receive(void)
{
  while(UART7->FR & (1 << 4));
  return UART7->DR;
}
