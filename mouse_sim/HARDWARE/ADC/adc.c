#include "adc.h"
#include "delay.h"		
#include "led.h"
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

/*----转化时间T = 采样时间（最小3个周期）+12个周期（12位）------*/
/*APB2=84M  ADC_CLK = 84/4=21M  T = 3+12=15周期  15/21M =0.71us   这是STM32微妙级别*/
/*--------ADC的最大时钟36M----------*/
//初始化ADC	


//ADC1的通道5（PA5）   ADC1的通道8（PB0）   ADC1的通道9(PB1)
u16 ADC1_Data[ADC1_CHN];                      //AD采集滤波后的值
u16 ADC1_ConvertedValue[ADC1_Count][ADC1_CHN];//DMA缓存extern volatile u8 adc_flag;
extern volatile u8 adc_flag;
u16 adc_buf[4];
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef    	  DMA_Initfjh;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_DMA2, ENABLE);//使能GPIOA/B/C时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟


	
    //PA1 作为模拟通道输入引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚
    GPIO_Init(GPIOC, &GPIO_InitStructure);
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
 
 ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//同样也是多个ADC同时交替采用同一个通道才会用到的 两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能 这个是多个ADC同时使用的时候 才会用到的
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode =ENABLE;//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//开启连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = ADC1_CHN;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
 // 配置 ADC 通道转换顺序和采样时间周期
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

	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE); // 使能DMA请求(Single-ADC mode)
	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);// 使能ADC DMA
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
//	ADC_SoftwareStartConv(ADC1);//开始adc转换，软件触发

 
 //DMA2的初始化结构体
	DMA_Initfjh.DMA_Channel = DMA_Channel_0;
	DMA_Initfjh.DMA_PeripheralBaseAddr =  ADC1_ADDR; //外设基地址，ADC的数据寄存器地址
	DMA_Initfjh.DMA_Memory0BaseAddr = (u32)&ADC1_ConvertedValue;  //存储器地址，既我们在内部SRAM定义的一个数组地址
	DMA_Initfjh.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_Initfjh.DMA_BufferSize = ADC1_CHN * ADC1_Count;  //一次传输的数据量，我们这里是三个
	DMA_Initfjh.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //不需要递增
	DMA_Initfjh.DMA_MemoryInc = DMA_MemoryInc_Enable; //递增
	DMA_Initfjh.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;// 外设数据大小为半字，即两个字节 
	DMA_Initfjh.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //存储器数据大小也为半字，跟外设数据大小相同
	DMA_Initfjh.DMA_Mode = DMA_Mode_Circular;	// 循环传输模式
	DMA_Initfjh.DMA_Priority = DMA_Priority_High;//优先级高
	DMA_Initfjh.DMA_FIFOMode = DMA_FIFOMode_Disable; //不使用FIFO 
	DMA_Initfjh.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//下边三个均是使用FIFO才需要用的
	DMA_Initfjh.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Initfjh.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA2_Stream0, &DMA_Initfjh);
  DMA_ClearFlag(DMA2_Stream0,DMA_IT_TC);                                         //清除DMA所有标志 
  DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);                              //开传输完成中断 
	
 
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // 优先级设置
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


//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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

