#include "DMA_control.h"
void DMA_ADC_config(uint32_t Periph_Address, uint16_t* Mem_Address){
	DMA_InitTypeDef   DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  
  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)Periph_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Mem_Address;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 8;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  /* DMA1 Channel1 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;//highest priority
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Transfer complete interrupt mask */
  //DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	 DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
}

void DMA_UART1_config(uint8_t* TX_buff, uint8_t* RX_buff){
  DMA_InitTypeDef  	DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;	
	/* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  USART_DMACmd(USART1, USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE);
	
	
	/* DMA Channel 2 for USART1 Tx configuration */		
	/* DMA Channel 3 for USART1 Rx configuration */		
  //DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->TDR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TX_buff;
  DMA_InitStructure.DMA_DIR =DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize =8;// BUFF_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// bugging, configured as non increase 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//adding this line is a must or despite of configuring disable increment peripheral, peripheral increment is still enable
  //DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  //DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  //DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  //DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);
  
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->RDR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RX_buff;
  DMA_InitStructure.DMA_DIR =DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize =8;// BUFF_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  //DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  //DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  //DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  //DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
  	/* Enable UART4 DMA */
  
	//USART_DMACmd(USART1, , ENABLE);
	DMA_Cmd(DMA1_Channel2, ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	/* Enable DMA Interrupt to the highest priority */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;//highest priority
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Transfer complete interrupt mask */
  //DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	 DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
}