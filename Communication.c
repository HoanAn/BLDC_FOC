#include "Communication.h"

void UART1_Init(uint32_t Baudrate)
{
  GPIO_InitTypeDef 	GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;   
	
   
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Enable UART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
  /* Connect UART1 pins to AF0 */  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);//TX1 pin AF0
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);//RX1 pin AF0 // page 35 datasheet

  /* GPIO Configuration for UART1 Tx and Rx */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

 
  USART_InitStructure.USART_BaudRate = Baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
	
	/* Enable USART */
  USART_Cmd(USART1, ENABLE);
	
	
}

void IntToStr6(int16_t u, uint8_t *y)
{
	int16_t a;
	a=u;
	if (a<0)
	{
		a=-a;
		y[0]='-';
	}
	else y[0]= '+';
	y[5]=a%10+0x30;
	a= a/10;
	y[4]=a%10+0x30;
	a= a/10;
	y[3]=a%10+0x30;
	a= a/10;
	y[2]=a%10+0x30;
	a= a/10;
	y[1]=a%10+0x30;
}
void FloatToStr5(float u, uint8_t *y)
{
	int16_t a;
	a=u*10;
	if (a<0)
	{
		a=-a;
		y[0]='-';
	}
	else y[0]= ' ';
//	y[5]=a%10+0x30;
//	a= a/10;
	y[4]=a%10+0x30;
	a= a/10;
	y[3]=a%10+0x30;
	a= a/10;
	y[2]=a%10+0x30;
	a= a/10;
	y[1]=a%10+0x30;
}