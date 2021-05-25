#ifndef __Communication_H
#define __Communication_H

#ifdef __cplusplus
 extern "C" {
#endif
////Include////

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_usart.h"
//////////////////////////

////Define///


/////////////

/////Variable//////
 
/////Function/////
void UART1_Init(uint32_t Baudrate);
void IntToStr6(int16_t u, uint8_t *y);
void FloatToStr5(float u, uint8_t *y);

//////////////////////	 
	 
#ifdef __cplusplus
}
#endif

#endif /*__Communication_H */