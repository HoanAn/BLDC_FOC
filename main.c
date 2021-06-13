#include "stm32f0xx.h"
#include "PWM_control.h"
#include "ADC_control.h"
#include "DMA_control.h"
#include "I_O_control.h"
#include "Communication.h"

//#define  PSEUDO_COMMUTATION

int Period;// currently configured as 1199 .PWM period, line 18, PWM.c - This value is stored i TIM1->ARR
int Frequency_ext=100;
int Frequency=0;
uint16_t ADC_Data[8]={0,0,0,0,0,0,0,0};
int Current_phase_A=0,Current_phase_B=0,Current_phase_C=0;// actual current per phase.
uint8_t  P5V_driver=0;
uint8_t Step=0, Next_Step=0, Start_up=1;
int16_t Ramp_up_one_second=0;
int16_t Duty_old=0;
int zero_cross_period=0;
uint8_t Rx_buff[8]={'0','0','0','0','0','0','0','0'};
uint8_t Tx_buff[8]={'S','0','0','0','0','0','0','E'};
int16_t Duty= First_PWM_width;
uint8_t Stop_time=0;
int	main(void)

	{
  	Sample_indicator_config();//Config LED for indicating
		ADC_Common_config();
		DMA_ADC_config(0x40012440,ADC_Data);
		
		Timer1_PWM_Pin_Config();// Output_PWM_channel
		Timer1_PWM_config();
		I_O_function_config();
		
		////Do not touch this line below, I temporarily forget why should be this value
		TIM_SetCompare4(TIM1,Period-1);// -100 is calibration param//do not modify channel 4, must be always this value unless a tuned event happen
		//using edge of OC4REF to trigger ADC conversion and automatically transfer data to array ADC_Data[8].
		//still do not understand why I have to use -100 to calib. ADC conversion moment does not happen at center of PWM pulse.
		Timer3_sample_config();// Config timer for changing step at start up   
		UART1_Init(115200);
		DMA_UART1_config(Tx_buff,Rx_buff);
		Timer14_sample_config();
		
		TIM_Cmd(TIM3,ENABLE);
		////////enable P5V_driver power for MOSFET trigger driver
		P5V_driver=1;//-P5V_driver;
		GPIO_WriteBit(GPIOB,GPIO_Pin_2,P5V_driver);
		//////////////////////////////////////////////
		Ramp_up_one_second=First_PWM_width;

		
		//-------Start up before entering main loop--------------------
		Motor_Startup(&Step,&Next_Step,&Start_up);
	
		while(1){
	
			
		//while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET );    
    // Clear DMA TC flag, TC = transfer complete
    //DMA_ClearFlag(DMA1_FLAG_TC1);		
		
		
		
		//-------------- Motor Start up---------------------------
		
		if( ADC_Data[Virtual_ground] < 30){
			Turn_on_P5V_driver();
			//TIM3->PSC=SystemCoreClock/TIM3_clock-1;
			TIM14->PSC=SystemCoreClock/TIM14_clock_start_up-1;
			TIM14->ARR=Start_up_change_step;
			Motor_Startup(&Step,&Next_Step,&Start_up);
			Ramp_up_one_second=First_PWM_width;
			//TIM_Cmd(TIM3,DISABLE);
			//TIM3->CNT=0;
			//TIM_ITConfig(TIM3,TIM_IT_CC1|TIM_IT_Update,ENABLE);
			TIM3->ARR=50000-1;//20 times turns to 1second
			//TIM3->PSC=SystemCoreClock/TIM3_clock-1;
			TIM14->PSC=SystemCoreClock/TIM14_clock-1;
			TIM14->ARR=Sample_time;
			//TIM_Cmd(TIM3,ENABLE);
			
			
		}
		////-----------End Motor Start up--------------	
			
			////Motor_ramp_up///
			
			///End Motor_ramp_up///
			
			
			TIM_SetCompare3(TIM1,Ramp_up_one_second);//Duty should be change to First_PWM_width. this code is for test. 
			TIM_SetCompare2(TIM1,Ramp_up_one_second);
			TIM_SetCompare1(TIM1,Ramp_up_one_second);
		  Current_per_phase_calculate(ADC_Data[Current_sense_A],ADC_Data[Current_sense_B],ADC_Data[Current_sense_C],&Current_phase_A,&Current_phase_B,&Current_phase_C,Step);
			 if( Current_phase_A>2000 || Current_phase_A<-2000) Turn_off_P5V_driver();// over current and start up again
		// need to consider turn off P5V_driver_condition
		}
	}
	
		
		
		
		
		
void TIM3_IRQHandler(){
	//TIM_Cmd(TIM3,DISABLE);
	
	//////for start up phase/////
	/*
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)){
	if (Start_up ==1){
		Turn_on_P5V_driver();
   Step++;
		if (Step >6) Step =1;
		Next_Step=Step;
		Current_per_phase_calculate(ADC_Data[Current_sense_A],ADC_Data[Current_sense_B],ADC_Data[Current_sense_C],&Current_phase_A,&Current_phase_B,&Current_phase_C,Step);
  //if( Current_phase_A>1700 || Current_phase_A<-1700) Turn_off_P5V_driver();
	}
	if (Start_up ==0){
   Stop_time++;
	if(Stop_time>20){//if cannot detect zero crossing in 1s, after Start_up, 5V driver will off.
		Turn_off_P5V_driver();
		Stop_time=0;
	}
	}
	
	}
	*/

	//--------changing step when running---------------------
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1)){
	//half of a step time (times during 2 zero crossing event) expired.
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
	//----------------------------------------------------------
	#ifdef PSEUDO_COMMUTATION
		if(TIM_GetITStatus(TIM3, TIM_IT_CC1)){
			Step++;
			if(Step>6) Step=1;
			Commutation_six_tep(Step);
		}
	#endif

	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
	//TIM_ITConfig(TIM1,TIM_IT_CC4|TIM_IT_COM|TIM_IT_Update,ENABLE);
//	TIM_Cmd(TIM1,ENABLE);
	

}

//----------------COM intterupt to adjust PWM out put in 3 channel--------------------------
void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM1,TIM_IT_COM)){
    TIM_ClearITPendingBit(TIM1,TIM_IT_COM);
		
	}
	
TIM_ClearITPendingBit(TIM1,TIM_IT_Update);

}
//-------------------------------------END-----------------------------------------------

//-----------Interrupt at center of PWM pulse to check zero crossing------------------------------

void TIM1_CC_IRQHandler(void){//line 7 PWM.c, connfig CCR interrupt only on channel 4.
TIM_ClearITPendingBit(TIM1,TIM_IT_CC4);
	
	//while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET );    
    // Clear DMA TC flag, TC = transfer complete
   // DMA_ClearFlag(DMA1_FLAG_TC1);	
	
	if( ADC_Data[Virtual_ground] > 30){
	Back_Emf_detect(ADC_Data[V_Phase_A], ADC_Data[V_Phase_B], ADC_Data[V_Phase_C], ADC_Data[Virtual_ground],&Next_Step,Step,Start_up,&zero_cross_period);
	}
	Toggle_PB10();
	
}
//-------------END------------------------------------------------------------------------------

//-----------DMA interrupt to receive data from UART------------------------------
void DMA1_Channel2_3_IRQHandler(void){
		uint8_t i =0;
//	DMA_ClearITPendingBit(DMA1_IT_TC2);
	DMA_ClearITPendingBit(DMA1_IT_TC3);
	DMA_ClearFlag(DMA1_FLAG_TC3);
	DMA_ClearFlag(DMA1_FLAG_TC2);
	DMA_ClearITPendingBit(DMA1_IT_TC2);
//	DMA_Cmd(DMA1_Channel2,DISABLE);
//	DMA1_Channel2->CNDTR=8;

//	for(i=0;i< 8;i++) Tx_buff[i]=Rx_buff[i];
//	DMA_Cmd(DMA1_Channel2,ENABLE);
	if (Rx_buff[0] == 'S' && Rx_buff[7] == 'E'){
		Duty_old=Duty;		
		Duty = (Rx_buff[2]-48)*1000+(Rx_buff[3]-48)*100+(Rx_buff[4]-48)*10+(Rx_buff[5]-48);
				
				DMA_ClearITPendingBit(DMA1_IT_TC2);
				DMA_Cmd(DMA1_Channel2,DISABLE);
				DMA1_Channel2->CNDTR=8;
				Tx_buff[0]='S';
				Tx_buff[1]=' ';
				Tx_buff[2]='D';
				Tx_buff[3]='O';
				Tx_buff[4]='N';
				Tx_buff[5]='E';
				Tx_buff[6]=' ';
				Tx_buff[7]='E';
			
				DMA_Cmd(DMA1_Channel2,ENABLE);
				
				}
}

void TIM14_IRQHandler(){
	//P5V_driver=1-P5V_driver;
	//	GPIO_WriteBit(GPIOB,GPIO_Pin_2,P5V_driver);

	//--------for start up phase---
	if(TIM_GetITStatus(TIM14, TIM_IT_Update)){
	if (Start_up >0 ){
		Turn_on_P5V_driver();
   Step++;
		if (Step >6) Step =1;
		Next_Step=Step;
		Current_per_phase_calculate(ADC_Data[Current_sense_A],ADC_Data[Current_sense_B],ADC_Data[Current_sense_C],&Current_phase_A,&Current_phase_B,&Current_phase_C,Step);
  //if( Current_phase_A>1700 || Current_phase_A<-1700) Turn_off_P5V_driver();
	}
	/*
	if (Start_up ==0){
   Stop_time++;
	if(Stop_time>20){//if cannot detect zero crossing in 1s, after Start_up, 5V driver will off.
		//Turn_off_P5V_driver();
		Stop_time=0;
	}
	}
	*/
	}
	////////////////////////////////
	
	if(Start_up==0){//------increase step time for ramp up

	Ramp_up_one_second = Ramp_up_one_second +  (Duty-Duty_old)/10;
		if(Ramp_up_one_second>Duty && Duty > Duty_old) Ramp_up_one_second=Duty;
		if(Ramp_up_one_second<Duty && Duty < Duty_old) Ramp_up_one_second=Duty;
		if(Ramp_up_one_second<Min_PWM_width ) Ramp_up_one_second=Min_PWM_width;
		if(Ramp_up_one_second>Max_PWM_width ) Ramp_up_one_second=Max_PWM_width;
	}
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);
}

void DMA1_Channel1_IRQHandler(void){
	    DMA_ClearFlag(DMA1_FLAG_TC1);
}