#ifndef __I_O_CONTROL_H
#define __I_O_CONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif
////Include////

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_it.h"

//////////////////////////

////Define///
#define OVC_OUT_N GPIO_Pin_2 //PB2
#define BRAKE			3 //PB3

// need to devide to 0xFFF at calculating time.

/////////////

/////Variable//////
 
/////Function/////
void I_O_function_config(void);
void Turn_on_P5V_driver(void);
void Turn_off_P5V_driver(void);
//////////////////////	 
	 
#ifdef __cplusplus
}
#endif

#endif /*__I_O_CONTROL_H */