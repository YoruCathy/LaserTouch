//#include "sys.h"

/*function scanner*/

#include "delay.h"
//#include "usart.h"
#include "led.h"
//#include "beep.h"
//#include "key.h"
//#include "test.h"
#include "adc.h"
//USB function include
#include  "usbd_hid_core.h"
#include  "usbd_usr.h"
#include  "usbd_desc.h"


#define APP_TEAT 1 //目前允许测试
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

#ifdef APP_TEAT   //  鼠标测试
uint8_t buf[4];


void datainit()
{
	buf[0]=0;
	buf[1]=1;
	buf[2]=1;
	buf[3]=0;
}
#endif
volatile u8 adc_flag=1;                                                                                               // int a;
int main(void)
{ 
	  u8 scanx,scany, scanx1 = 0;
	  u8 count,touch_sta,time_count;
    u8 time=5;
    u16 pos[1][2];
    u16 lastpos[1][2];
    u16 voltx[112];
		u16 voltx_init[112];
    u16 volty[196]; 
		u16 volty_init[112];
    u8 x,y;
    u16 maxx,maxy;
	
		delay_init(168);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	  LED_Init();					//初始化LED 
	  Adc_Init();         //初始化ADC
		uart_init(115200);
	
		// 初始化环境
		for(scanx=0; scanx<8; scanx++)      // x方向2*4组，依次遍历
        {
            IO_ctl(scanx,ENABLE);
            delay_us(50);
            ADC_SoftwareStartConv(ADC1);
						
            while(adc_flag)
            {
            }
						adc_flag=1;
            for(count=0; count<14; count++)   //每组
            {
                voltx_init[scanx*14+count]=ADC1_Data[count];
            }

            IO_ctl(scanx,DISABLE);
        }
        for(scany=8; scany<22; scany++)   // y方向2*7组，依次遍历
        {
            IO_ctl(scany,ENABLE);
            delay_us(50);
            ADC_SoftwareStartConv(ADC1);
            while(adc_flag)
            {

            }
						adc_flag=1;
            for(count=0; count<14; count++)
            {
                volty_init[(scany-8)*14+count]=ADC1_Data[count];
            }
            IO_ctl(scany,DISABLE);
       
					}
				
				
	
	while(1)
	{
//		if (time_count>10)
//				{
			touch_sta=0;
//					time_count = 0;
//				}
//				else
//				{
//					time_count++;
//				}
        for(scanx=0; scanx<8; scanx++)      // x方向2*4组，依次遍历
        {
            IO_ctl(scanx,ENABLE);
            delay_us(50);
            ADC_SoftwareStartConv(ADC1);
						
						scanx1 = scanx;
						if (scanx1 > 5) scanx1 *= 3;
						
            while(adc_flag)
            {
            }
						adc_flag=1;
            for(count=0; count<14; count++)   //每组
            {
                voltx[scanx*14+count]=ADC1_Data[count]*2;
            }

            IO_ctl(scanx,DISABLE);
        }
        for(scany=8; scany<22; scany++)   // y方向2*7组，依次遍历
        {
            IO_ctl(scany,ENABLE);
            delay_us(50);
            ADC_SoftwareStartConv(ADC1);
            while(adc_flag)
            {

            }
						adc_flag=1;
            for(count=0; count<14; count++)
            {
                volty[(scany-8)*14+count]=ADC1_Data[count]*2;
            }
            IO_ctl(scany,DISABLE);
        }
				
        maxx=voltx[0];
        x=0;
        maxy=volty[0];
        y=0;
        for(count=0; count<112; count++)
        {

            if(voltx[count]>maxx)
            {
                maxx=voltx[count];
                x=count;
            }

            if(volty[count]>maxy)
            {
                maxy=volty[count];
                y=count;
            }
        }
				for(count=112; count<196; count++)
        {
            if(volty[count]>maxy)
            {
                maxy=volty[count];
                y=count;
            }
        }
				
				
	 if( y <56 || y >140)
	 {
	    if(x>56)
			{
			  if (maxx>500)
        {
            lastpos[0][1]=pos[0][1];
//            pos[0][0]=16+x*16;
//					  pos[0][1]=620-5*x;
					pos[0][1]=x;
            touch_sta=touch_sta|0x01;
//					printf("%d %d\n ",pos[0][0]); //打印LCD ID   
        }
        if(maxy>500)
        {
            lastpos[0][0]=pos[0][0];
//            pos[0][1]=252+(64-y)*16;
//					  pos[0][0]=400-5*y;
					pos[0][0]=y;
            touch_sta=touch_sta|0x02;
//						printf("%d %d\n ",pos[0][1]); //打印LCD ID  
        }
			
			}
				if(28<=x<=56)
				{
				if (maxx>500)
        {
            lastpos[0][1]=pos[0][1];
//            pos[0][0]=16+x*16;
//					  pos[0][1]=620-5*x;
					pos[0][1]=x;
            touch_sta=touch_sta|0x01;
//					printf("%d %d\n ",pos[0][0]); //打印LCD ID   
        }
        if(maxy>500)
        {
            lastpos[0][0]=pos[0][0];
//            pos[0][1]=252+(64-y)*16;
//					  pos[0][0]=400-5*y;
					pos[0][0]=y;
            touch_sta=touch_sta|0x02;
//						printf("%d %d\n ",pos[0][1]); //打印LCD ID  
        }
				
				}
				if(x<28)
				{
					if (maxx>400)
        {
            lastpos[0][1]=pos[0][1];
//            pos[0][0]=16+x*16;
//					  pos[0][1]=620-5*x;
					pos[0][1]=x;
            touch_sta=touch_sta|0x01;
//					printf("%d %d\n ",pos[0][0]); //打印LCD ID   
        }
        if(maxy>400)
        {
            lastpos[0][0]=pos[0][0];
//            pos[0][1]=252+(64-y)*16;
//					  pos[0][0]=400-5*y;
					pos[0][0]=y;
            touch_sta=touch_sta|0x02;
//						printf("%d %d\n ",pos[0][1]); //打印LCD ID  
        }
				}
	  
	 
	 }		 
	 if( 56<=y<=140)
	 {
	 
		 if(x>56)
		 {
		  if (maxx>400)
        {
            lastpos[0][1]=pos[0][1];
//            pos[0][0]=16+x*16;
//					  pos[0][1]=620-5*x;
					pos[0][1]=x;
            touch_sta=touch_sta|0x01;
//					printf("%d %d\n ",pos[0][0]); //打印LCD ID   
        }
        if(maxy>400)
        {
            lastpos[0][0]=pos[0][0];
//            pos[0][1]=252+(64-y)*16;
//					  pos[0][0]=400-5*y;
					pos[0][0]=y;
            touch_sta=touch_sta|0x02;
//						printf("%d %d\n ",pos[0][1]); //打印LCD ID  
        }
		 
		 }
		 if(28<=x<=56)
		 {
		  if (maxx>400)
        {
            lastpos[0][1]=pos[0][1];
//            pos[0][0]=16+x*16;
//					  pos[0][1]=620-5*x;
					pos[0][1]=x;
            touch_sta=touch_sta|0x01;
//					printf("%d %d\n ",pos[0][0]); //打印LCD ID   
        }
        if(maxy>300)
        {
            lastpos[0][0]=pos[0][0];
//            pos[0][1]=252+(64-y)*16;
//					  pos[0][0]=400-5*y;
					pos[0][0]=y;
            touch_sta=touch_sta|0x02;
//						printf("%d %d\n ",pos[0][1]); //打印LCD ID  
        }
		 }
	 
		 if(x<28)
		 {
		 
        if (maxx>300)
        {
            lastpos[0][1]=pos[0][1];
//            pos[0][0]=16+x*16;
//					  pos[0][1]=620-5*x;
					pos[0][1]=x;
            touch_sta=touch_sta|0x01;
//					printf("%d %d\n ",pos[0][0]); //打印LCD ID   
        }
        if(maxy>300)
        {
            lastpos[0][0]=pos[0][0];
//            pos[0][1]=252+(64-y)*16;
//					  pos[0][0]=400-5*y;
					pos[0][0]=y;
            touch_sta=touch_sta|0x02;
//						printf("%d %d\n ",pos[0][1]); //打印LCD ID  
        }
		 }
		 
	 }
//        if (maxx>500)
//        {
//            lastpos[0][1]=pos[0][1];
////            pos[0][0]=16+x*16;
////					  pos[0][1]=620-5*x;
//					pos[0][1]=x;
//            touch_sta=touch_sta|0x01;
////					printf("%d %d\n ",pos[0][0]); //打印LCD ID   
//        }
//        if(maxy>500)
//        {
//            lastpos[0][0]=pos[0][0];
////            pos[0][1]=252+(64-y)*16;
////					  pos[0][0]=400-5*y;
//					pos[0][0]=y;
//            touch_sta=touch_sta|0x02;
////						printf("%d %d\n ",pos[0][1]); //打印LCD ID  
//        }

				
//				 if (maxx>1000*(x/112)*(x/112))
//        {
//            lastpos[0][1]=pos[0][1];
////            pos[0][0]=16+x*16;
////					  pos[0][1]=620-5*x;
//					pos[0][1]=x;
//            touch_sta=touch_sta|0x01;
////					printf("%d %d\n ",pos[0][0]); //打印LCD ID   
//        }
//				if(y<=98)
//        {if(maxy>1000*(98/(y+1))*(98/(y+1)))
//        {
//            lastpos[0][0]=pos[0][0];
////            pos[0][1]=252+(64-y)*16;
////					  pos[0][0]=400-5*y;
//					pos[0][0]=y;
//            touch_sta=touch_sta|0x02;
////						printf("%d %d\n ",pos[0][1]); //打印LCD ID  
//        }
//			}
//				if(y>98)
//					
//        {if(maxy>1000*(98/(y-98))*(98/(y-98)))
//        {
//            lastpos[0][0]=pos[0][0];
////            pos[0][1]=252+(64-y)*16;
////					  pos[0][0]=400-5*y;
//					pos[0][0]=y;
//            touch_sta=touch_sta|0x02;
////						printf("%d %d\n ",pos[0][1]); //打印LCD ID  
//        }
//			}
//				
				

        //////////////////////////////////报点///////////////////////////////////
//        gui_fill_circle(lastpos[0][0],lastpos[0][1],2,WHITE);
        if(touch_sta==3)
        {
//            gui_fill_circle(pos[0][0],pos[0][1],2,RED);
            touch_sta=0;
//					  printf("x=:%d , y=%d!\r\n",x,y);

	printf("X%d\r\n",pos[0][0]); //打印LCD ID   
	printf("Y%d\r\n",pos[0][1]); //打印LCD ID   
 
	
        }

      //delay_ms(100);			
	}

}



