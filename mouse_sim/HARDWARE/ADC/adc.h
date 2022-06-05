#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//ADC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#define ADC1_CHN         14                  //要使用的ADC通道数
#define ADC1_Count      1			        //AD滤波采样次数


extern u16 ADC1_Data[ADC1_CHN];                      //AD采集滤波后的值
extern u16 ADC1_ConvertedValue[ADC1_Count][ADC1_CHN];//DMA缓存

void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
void IO_ctl(u8 numid,FunctionalState NewState);

#define	ADC1_ADDR  ((u32)ADC1+0x4c)
#endif 















