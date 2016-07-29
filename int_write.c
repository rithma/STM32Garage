int _write( int file, char *ptr, int len )

{

int txCount;

  (void)file;

  for ( txCount = 0; txCount < len; txCount++)

  {

  USART_SendData(USART1, ptr[txCount]);   //Don't forget to include "stm32f10x_usart.h"

  /* Loop until the end of transmission */

   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)

   {

   }

  }

  return len;

}

void abort(void)

{

  /* Abort called */

  while(1);

}
