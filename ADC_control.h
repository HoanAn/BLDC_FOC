#ifndef __ADC_CONTROL_H
#define __ADC_CONTROL_H

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
#define V_Phase_A 3
#define V_Phase_B 4
#define V_Phase_C 5
#define I_Phase_A 0
#define I_Phase_B 1
#define I_Phase_C 2
#define Virtual_ground 7
#define V_Bus 6	 
	 
	 
#define ADC_max_volt 3300 
// need to devide to 0xFFF at calculating time.
#define I_scale ADC_scale*(200/3)
#define V_scale ADC_scale*16
/////////////

/////Variable//////
 
/////Function/////
void ADC_Common_config();
void ADC_Single_channel_start_conversion (ADC_TypeDef* ADCx, uint32_t ADC_Channel, uint16_t ADC_SampleTime);
void Back_Emf_detect(int Phase_A_Volt, int Phase_B_Volt, int Phase_C_Volt, int Virtual_Ground_Volt, uint8_t* Next_Step, uint8_t Step, uint8_t Start_up,int* zero_cross_period);// rreturn commutation moment.
//////////////////////	 
	 
	 
#ifdef __cplusplus
}
#endif

#endif /*__ADC_CONTROL_H */