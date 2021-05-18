#include "ADC_control.h"
#include "PWM_control.h"
void ADC_Single_channel_start_conversion (ADC_TypeDef* ADCx, uint32_t ADC_Channel, uint16_t ADC_SampleTime){
	
	  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));

  /* Configure the ADC Channel */
  ADCx->CHSELR = (uint32_t)ADC_Channel;

  /* Clear the Sampling time Selection bits */
  tmpreg &= ~ADC_SMPR1_SMPR;

  /* Set the ADC Sampling Time register */
  tmpreg |= (uint32_t)ADC_SampleTime;

  /* Configure the ADC Sample time register */
  ADCx->SMPR = tmpreg ;
	//start_conversion!!
	ADCx->CR |= (uint32_t)ADC_CR_ADSTART;
	
}
void ADC_Common_config(){
	ADC_InitTypeDef     ADC_InitStructure;
  GPIO_InitTypeDef    GPIO_InitStructure;
  
  /* GPIOA Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  
// re config ADC pin PA0-PA7.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // convert all channels once.
  //--------------------------------automatically convert
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_RisingFalling;
	ADC_InitStructure.ADC_ExternalTrigConv= ADC_ExternalTrigConv_T1_TRGO;//                     ----CURRENTLY, TIM1_TRG0 IS FROM OCREF4.----
  //-------------------------------------
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  /* Convert the ADC1 Channel 11 with 239.5 Cycles as sampling time */ 
//#ifdef USE_STM320518_EVAL
  ADC_ChannelConfig(ADC1, ADC_Channel_0|ADC_Channel_1|ADC_Channel_2|ADC_Channel_3|ADC_Channel_4|ADC_Channel_5|ADC_Channel_6|ADC_Channel_7, ADC_SampleTime_1_5Cycles);
//#else
//  ADC_ChannelConfig(ADC1, ADC_Channel_10 , ADC_SampleTime_239_5Cycles);
//#endif /* USE_STM320518_EVAL */

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
	
	/* ADC DMA request in circular mode */
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  
  /* Enable ADC_DMA */
  ADC_DMACmd(ADC1, ENABLE);  
  
  /* Enable the ADC peripheral */
  //ADC_Cmd(ADC1, ENABLE);     
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
 
	/* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
}

int Back_Emf_detect(int Phase_A_Volt, int Phase_B_Volt, int Phase_C_Volt,int Virtual_Ground_Volt, int DC_Bus, int Step,uint8_t Start_up){
	// compare zero crossing between phases and virtual ground point.
	//should be read with pwm frequency.
	//return data to indicate when commutation should be happend, with steps should be communicated.
	uint8_t Next_step=Step;
	switch(Step){
		case 0: break;
		case 1: //AH_BL
		{
			if(Phase_C_Volt<Virtual_Ground_Volt) {
				Next_step=2;
				TIM3_action_at_BEMF_zero_crossing(Start_up);
			}
			break;
		}
		case 2: //AH_CL
		{
			if(Phase_B_Volt>Virtual_Ground_Volt){
				Next_step=3;
				TIM3_action_at_BEMF_zero_crossing(Start_up);
			}
		  break;
		}
	 case 3: //BH_CL
		{
			if(Phase_A_Volt<Virtual_Ground_Volt) {
				Next_step=4;
				TIM3_action_at_BEMF_zero_crossing(Start_up);
			}
			break;
		}
		case 4: //AL_BH
		{
			if(Phase_C_Volt>Virtual_Ground_Volt){
				Next_step=5;
				TIM3_action_at_BEMF_zero_crossing(Start_up);
			}
			break;
		}
		case 5://AL_CH 
		{
			if(Phase_B_Volt<Virtual_Ground_Volt){
				Next_step=6;
				TIM3_action_at_BEMF_zero_crossing(Start_up);
			}
			break;
		}
		case 6://BL_CH 
		{
			if(Phase_A_Volt>Virtual_Ground_Volt){
				Next_step=1;
				TIM3_action_at_BEMF_zero_crossing(Start_up);
			}
			break;
		}
	
	}
	return Next_step;
	//add a new line
}


