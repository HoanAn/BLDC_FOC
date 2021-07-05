#ifndef __PWM_H
#define __PWM_H

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
//////////////////////////

////Define///
#define Sample_period 1000 //1s
#define Start_up_change_step 2000-1
#define First_PWM_width 350
#define Min_PWM_width 170
#define Max_PWM_width 1050	 
#define TIM3_clock_start_up 100000 //(kHz)
#define TIM3_clock 500000 //(kHz)
#define TIM14_clock_start_up 1000000 //(Hz)
#define TIM14_clock 1000 //(Hz)
#define Sample_time 100 //(means 100ms for timer 14)	 
#define Z_cross_startup 50	 
//#define  PSEUDO_COMMUTATION
/////////////

/////Variable//////
extern int Period;
extern int Frequency;	 
/////Function/////
void Timer1_PWM_config(void);
void Timer1_PWM_Pin_Config(void);
void Timer3_sample_config(void);
void Sample_indicator_config(void);
void Commutation_six_tep(int Step);
int Motor_Startup(uint8_t* Step, uint8_t* Next_Step, uint8_t* Start_up);
int TIM3_action_at_BEMF_zero_crossing( int Start_up );
void Timer14_sample_config(void);
//////////////////////	 
	 
	 
#ifdef __cplusplus
}
#endif

#endif /*__PWM_H */