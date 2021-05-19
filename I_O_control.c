#include "I_O_control.h"
 void I_O_function_config(void){
		GPIO_InitTypeDef GPIO_OutInitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
		
		GPIO_OutInitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9|GPIO_Pin_8;//pin 9 is for testing only BEMF zero crossing, designed fucntion is I2c_SDA
		GPIO_OutInitStructure.GPIO_Mode= GPIO_Mode_OUT;
		GPIO_OutInitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_OutInitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_OutInitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB, &GPIO_OutInitStructure);
 }
 void Turn_on_P5V_driver(void){
	 GPIO_WriteBit(GPIOB,OVC_OUT_N,1);
 }
void Turn_off_P5V_driver(void){
	 GPIO_WriteBit(GPIOB,OVC_OUT_N,0);
 }
void Toggle_PB9(void){
	GPIO_WriteBit(GPIOB,GPIO_Pin_9,1);
	GPIO_WriteBit(GPIOB,GPIO_Pin_9,0);
}

void Toggle_PB8(void){
	GPIO_WriteBit(GPIOB,GPIO_Pin_8,1);
	GPIO_WriteBit(GPIOB,GPIO_Pin_8,0);
}