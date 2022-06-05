#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
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

#define ADC1_CHN         14                  //Ҫʹ�õ�ADCͨ����
#define ADC1_Count      1			        //AD�˲���������


extern u16 ADC1_Data[ADC1_CHN];                      //AD�ɼ��˲����ֵ
extern u16 ADC1_ConvertedValue[ADC1_Count][ADC1_CHN];//DMA����

void Adc_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc(u8 ch); 				//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
void IO_ctl(u8 numid,FunctionalState NewState);

#define	ADC1_ADDR  ((u32)ADC1+0x4c)
#endif 















