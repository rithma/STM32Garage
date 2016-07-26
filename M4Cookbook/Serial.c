/********
/*
*Low level serial routines
*adapted from the M4 cookbook
*Molded to try & get USART 1 working on my nucleo 
*after a million hours of trying fucked up examples
*I will hit the registers at the bottom
*and look at the datasheet
*/

#include "stm32f4xx.h"
#include "Serial.h"

#ifdef __DBG_ITM
volatile int32_t ITM_RxBuffer;
#endif

//Inits

void SER_Init (void) {
#ifdef __DBG_ITM
	ITM_RxBuffer = ITM_RXBUFFER_EMPTY;
	
#else
	RCC->APB1ENR |= (1UL << 19);        //enables USART4, this is in the book
	RCC->APB2ENR |= (1UL << 4);         //this SHOULD enable USART1... this is me digging in the datasheet
	RCC->APB2ENR |= (1UL << 0);         //enable AFIO clock
	RCC->AHB1ENR |= (1UL << 2);         //enable GPIOC clock
	
	GPIOC->MODER &= 0xFF0FFFFF;
	GPIOC->MODER |= 0x00A00000;
	GPIOC->AFR[1] |= 0x00008800;
	
	UART4->BRR = (22 << 4) | 12;
	UART4->CR2 = 0x0000;
	UART4->CR1 = 0x200C;
	
#endif
}

/* SER_PutChar**************************
//**************************************/

int32_t SER_PutChar (int32_t ch) {
	#ifdef __DBG_ITM
	int i;
	ITM_SendChar (ch & 0xFF);
	for (i = 10000; i; i--)
		;
	#else
	while (!(UART1->SR & 0x0080));
	UART1->DR = (ch & 0xFF);
	#endif

	return (ch);
}


}
