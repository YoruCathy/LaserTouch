#include "adc.h"
#include "delay.h"		
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

/*----ת��ʱ��T = ����ʱ�䣨��С3�����ڣ�+12�����ڣ�12λ��------*/
/*APB2=84M  ADC_CLK = 84/4=21M  T = 3+12=15����  15/21M =0.71us   ����STM32΢���*/
/*--------ADC�����ʱ��36M----------*/
//��ʼ��ADC	


//ADC1��ͨ��5��PA5��   ADC1��ͨ��8��PB0��   ADC1��ͨ��9(PB1)
u16 ADC1_Data[ADC1_CHN];                      //AD�ɼ��˲����ֵ
u16 ADC1_ConvertedValue[ADC1_Count][ADC1_CHN];//DMA����extern volatile u8 adc_flag;
extern volatile u8 adc_flag;
u16 adc_buf[4];
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef    	  DMA_Initfjh;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_DMA2, ENABLE);//ʹ��GPIOA/B/Cʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��


	
    //PA1 ��Ϊģ��ͨ����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//ģ����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
 
 ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//ͬ��Ҳ�Ƕ��ADCͬʱ�������ͬһ��ͨ���Ż��õ��� ���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ�� ����Ƕ��ADCͬʱʹ�õ�ʱ�� �Ż��õ���
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode =ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//��������ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = ADC1_CHN;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
 // ���� ADC ͨ��ת��˳��Ͳ���ʱ������
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1,1,  ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2,2,  ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3,3,  ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5,4,  ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6,5,  ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7,6,  ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8,7,  ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9,8,  ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10,9, ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11,10, ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12,11, ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13,12, ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14,13, ADC_SampleTime_112Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15,14, ADC_SampleTime_112Cycles );

	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE); // ʹ��DMA����(Single-ADC mode)
	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);// ʹ��ADC DMA
	ADC_Cmd(ADC1, ENABLE);//����ADת����	
//	ADC_SoftwareStartConv(ADC1);//��ʼadcת�����������

 
 //DMA2�ĳ�ʼ���ṹ��
	DMA_Initfjh.DMA_Channel = DMA_Channel_0;
	DMA_Initfjh.DMA_PeripheralBaseAddr =  ADC1_ADDR; //�������ַ��ADC�����ݼĴ�����ַ
	DMA_Initfjh.DMA_Memory0BaseAddr = (u32)&ADC1_ConvertedValue;  //�洢����ַ�����������ڲ�SRAM�����һ�������ַ
	DMA_Initfjh.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_Initfjh.DMA_BufferSize = ADC1_CHN * ADC1_Count;  //һ�δ��������������������������
	DMA_Initfjh.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //����Ҫ����
	DMA_Initfjh.DMA_MemoryInc = DMA_MemoryInc_Enable; //����
	DMA_Initfjh.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;// �������ݴ�СΪ���֣��������ֽ� 
	DMA_Initfjh.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_Initfjh.DMA_Mode = DMA_Mode_Circular;	// ѭ������ģʽ
	DMA_Initfjh.DMA_Priority = DMA_Priority_High;//���ȼ���
	DMA_Initfjh.DMA_FIFOMode = DMA_FIFOMode_Disable; //��ʹ��FIFO 
	DMA_Initfjh.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//�±���������ʹ��FIFO����Ҫ�õ�
	DMA_Initfjh.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Initfjh.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA2_Stream0, &DMA_Initfjh);
  DMA_ClearFlag(DMA2_Stream0,DMA_IT_TC);                                         //���DMA���б�־ 
  DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);                              //����������ж� 
	
 
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // ���ȼ�����
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
  DMA_Cmd(DMA2_Stream0, ENABLE);	

	
}
void filter(void)
{
	register u16 sum=0;
	u8 count=0,i=0,j=0;
	for(;i<ADC1_CHN;i++)
	{
		while(j<ADC1_Count)
		{//865
			if(ADC1_ConvertedValue[j][i]<0){}
				else
			{
			sum+=ADC1_ConvertedValue[j][i];
					count++;
			}
		  j++;
		}
		ADC1_Data[i]=sum/count;
		sum=0;count=0;j=0;
	}
}


//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 


void DMA2_Stream0_IRQHandler(void) 
{
	if (DMA_GetFlagStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET)  
	{
		filter();
		DMA_ClearFlag(DMA2_Stream0, DMA_IT_TCIF0); 
		adc_flag=0;
	}
}


void IO_ctl(u8 numid,FunctionalState NewState)
{
    u8 idnum;
    if (numid>15)
    {
			
        idnum = numid-6;
        PEout(idnum)=NewState;
			
    }
    else
    {
			switch(numid)
			{
				case 9:
						 PEout(2)=NewState;
				break;
				case 10:
						 PEout(3)=NewState;
				break;
				case 14:
						 PEout(5)=NewState;
				break;
				case 15:
						 PEout(6)=NewState;
				break;
				default:
					 PFout(numid)=NewState;
				break;
			
			
			
			}

}						
}

