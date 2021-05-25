#ifndef __DMA_CONTROL_H
#define __DMA_CONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif
////Include////
#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_adc.h"	 
//////////////////////////

////Define///
#define Phase_A ADC_Channel_0
#define Phase_B ADC_Channel_1
#define Phase_C ADC_Channel_2
/////////////

/////Variable//////
 
/////Function/////
void DMA_ADC_config(uint32_t Periph_Address, uint16_t* Mem_Address);
void DMA_UART1_config(uint8_t* TX_buff, uint8_t* RX_buff);
//////////////////////	 
	 
	 
#ifdef __cplusplus
}
#endif

#endif /*__DMA_CONTROL_H */