// STM32 Keil printf USART2 (Tx PA.2, Rx PA.3) STM32F103RB NUCLEO - sourcer32@gmail.com
 
#include "stm32f10x.h"
 
#include <stdio.h>
#include <stdlib.h>
 
//****************************************************************************
 
void USART2_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
 
  /* Enable GPIO and USART2 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
 
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  /* USARTx configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
  /* USART configuration */
  USART_Init(USART2, &USART_InitStructure);
 
  /* Enable USART */
  USART_Cmd(USART2, ENABLE);
}
 
//****************************************************************************
 
void OutString(char *s)
{
  while(*s)
  {
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty
 
    USART_SendData(USART2, *s++); // Send Char
  }
}
 
//****************************************************************************
 
int main(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
 
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
  */
 
  RCC_GetClocksFreq(&RCC_Clocks);
 
  USART2_Configuration();
 
  OutString("Welcome to Nucleo F103RB\r\n");
 
  printf("Hello World!\n");
 
  printf("Running at %d MHz\n", RCC_Clocks.SYSCLK_Frequency / 1000000);
 
  while(1); // Don't want to exit
}
 
//****************************************************************************
// Hosting of stdio functionality through USART2
//****************************************************************************
 
#include <rt_misc.h>
 
#pragma import(__use_no_semihosting_swi)
 
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
 
int fputc(int ch, FILE *f)
{
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
 
  USART_SendData(USART2, ch);
 
  return(ch);
}
 
int fgetc(FILE *f)
{
  char ch;
 
  while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
 
  ch = USART_ReceiveData(USART2);
 
  return((int)ch);
}
 
int ferror(FILE *f)
{
  /* Your implementation of ferror */
  return EOF;
}
 
void _ttywrch(int ch)
{
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
 
  USART_SendData(USART2, ch);
}
 
void _sys_exit(int return_code)
{
label:  goto label;  /* endless loop */
}
 
//****************************************************************************
 
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
 
