#include "stm32f0xx.h"
#include "PWM_control.h"
#include "ADC_control.h"
#include "DMA_control.h"
#include "I_O_control.h"
//#define  PSEUDO_COMMUTATION
int Period;//PWM period, line 18, PWM.c - This value is stored i TIM1->ARR
int Frequency_ext=100;
int Frequency=0;
uint16_t ADC_Data[8]={0,0,0,0,0,0,0,0};
uint16_t Back_EMF_test=0,Back_EMF_test2=0,Back_EMF_test3=0;
uint32_t ADC_time_conversion_debug=0;
volatile int Encoder_count=0;
int Button=0;
uint8_t Toggle=0, P5V_driver=0;
uint8_t Toggle_COM=0;
uint8_t Step=0, Next_Step=0, Start_up=1, BEMF_at_startup=0;
int zero_cross_period=0;
int	main(void)

	{
  	Sample_indicator_config();//Config LED for indicating
		ADC_Common_config();
		DMA_ADC_config(0x40012440,ADC_Data);
		
		Timer1_PWM_Pin_Config();// Output_PWM_channel
		Timer1_PWM_config();
		I_O_function_config();
		TIM_SetCompare3(TIM1,Period/3);
		TIM_SetCompare2(TIM1,Period/3);
		TIM_SetCompare1(TIM1,Period/3);
		TIM_SetCompare4(TIM1,Period-1);// -100 is calibration param
		
		//#ifdef PSEUDO_COMMUTATION
		Timer3_sample_config();// Config timer for pseudo 6 step commutating   
		//#endif
		TIM_Cmd(TIM3,ENABLE);
		
		//------Motor Startup-------
		while(BEMF_at_startup<6){
		//TIM_Cmd(TIM3,DISABLE);
		Motor_Startup(&Step,&Next_Step,&Start_up);			
		TIM_Cmd(TIM3,ENABLE);
		TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
		if(Next_Step - Step==0){}//waiting for response from TIM3 interrupt.This diffirence means zero crossing has been detected
			else{
			GPIO_WriteBit(GPIOB,GPIO_Pin_9,1);
			BEMF_at_startup++;
			GPIO_WriteBit(GPIOB,GPIO_Pin_9,0);
	  }	
		
		TIM_Cmd(TIM3,ENABLE);
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	 }
		//-----------------------------------------
		TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);
		Start_up=0;
		Step=6;	
	  Next_Step=6;
	
	
		//---------------------------
		//TIM_SetCompare1(TIM3,500);// -100 is calibration param || testing timer counting
		
		//using edge of OC4REF to trigger ADC conversion and automatically transfer data to array ADC_Data[8].
		//still do not understand why I have to use -100 to calib. ADC conversion moment does not happen at center of PWM pulse.
		while(1){
		/*
		Back_EMF_test=ADC_Data[0]*ADC_max_volt/0xFFF;
		Back_EMF_test2=ADC_Data[V_Phase_A]*ADC_max_volt/0xFFF;
		Back_EMF_test3=ADC_Data[Virtual_ground]*ADC_max_volt/0xFFF;	
		*/
			
		while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET );    
    /* Clear DMA TC flag */
    DMA_ClearFlag(DMA1_FLAG_TC1);
		
		}
	}
	
		
		
		
		
		
void TIM3_IRQHandler(){
	//TIM_Cmd(TIM3,DISABLE);
	//TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);
	
	P5V_driver=1;//-P5V_driver;
	GPIO_WriteBit(GPIOB,GPIO_Pin_2,P5V_driver);
	
	//////for start up phase/////
	if (Start_up ==1){
   Step++;
		if (Step >6) Step =1;
		Next_Step=Step;
	}
	///////////////////////////
	
	
	
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1)){
	//last_period=TIM3_action_at_BEMF_zero_crossing();// this line is just for test capture interrupt of timer 3 channel 1
	//half of a step time expired.
	if (Start_up ==0){
	#ifndef PSEUDO_COMMUTATION
			if(Step != Next_Step) {
				Step=Next_Step;
				Toggle_PB8();
				Commutation_six_tep(Step);
		}
			#endif
	}
	
	
}
	#ifdef PSEUDO_COMMUTATION
if(TIM_GetITStatus(TIM3, TIM_IT_CC1)){
	Step++;
	if(Step>6) Step=1;
	Commutation_six_tep(Step);
}
	#endif		
	//Generate event COM after configure PWM channel in above. Configuration parameter affected after this event happened.
	//TIM_GenerateEvent(TIM1,TIM_EventSource_COM);//COM event is for communication between step.
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
	//TIM3->CNT=0;
	//TIM_Cmd(TIM3,ENABLE);
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//what, why TIM1 !??
}


void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM1,TIM_IT_COM)){
    TIM_ClearITPendingBit(TIM1,TIM_IT_COM);	
	}
	
TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

}
void TIM1_CC_IRQHandler(void){//line 7 PWM.c, connfig CCR interrupt only on channel 4.
TIM_ClearITPendingBit(TIM1,TIM_IT_CC4);
	
	//GPIO_WriteBit(GPIOB,GPIO_Pin_8,1);
	//Toggle = 1- Toggle;
	//GPIO_WriteBit(GPIOB,GPIO_Pin_8,0);
	//ADC_time_conversion_debug = TIM1->CNT;
	Back_Emf_detect(ADC_Data[V_Phase_A], ADC_Data[V_Phase_B], ADC_Data[V_Phase_C], ADC_Data[Virtual_ground],&Next_Step,Step,Start_up,&zero_cross_period);
	//last_period=TIM3_action_at_BEMF_zero_crossing();

	}

