#include "./delay/delay.h"


//static __IO u32 TimingDelay;
// 
///**
//  * @brief  ����ϵͳ�δ�ʱ�� SysTick
//  * @param  ��
//  * @retval ��
//  */
//void SysTick_Init(void)
//{
//	/* SystemFrequency / 1000    1ms�ж�һ��
//	 * SystemFrequency / 100000	 10us�ж�һ��
//	 * SystemFrequency / 1000000 1us�ж�һ��
//	 */
//	
////	if (SysTick_Config(SystemCoreClock / 1000000))
//	if (SysTick_Config(8))
//	{ 
//		/* Capture error */ 
//		while (1);
//	}
//}

///**
//  * @brief   us��ʱ����,10usΪһ����λ
//  * @param  
//  *		@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
//  * @retval  ��
//  */
//void delay_us(__IO u32 nTime)
//{ 
//	TimingDelay = nTime;	

//	while(TimingDelay != 0);
//}

///**
//  * @brief  ��ȡ���ĳ���
//  * @param  ��
//  * @retval ��
//  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
//  */
//void TimingDelay_Decrement(void)
//{
//	if (TimingDelay != 0x00)
//	{ 
//		TimingDelay--;
//	}
//}
/*********************************************END OF FILE**********************/

//ֻ���������ϵͳ
//void delay_us(u32 nus)
//{
// u32 temp;
// SysTick->LOAD = nus;
// SysTick->VAL=0X00;//��ռ�����
// SysTick->CTRL=0X01;//ʹ�ܣ����������޶����������ⲿʱ��Դ
// do
// {
//  temp=SysTick->CTRL;//��ȡ��ǰ������ֵ
// }while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
//     SysTick->CTRL=0x00; //�رռ�����
//    SysTick->VAL =0X00; //��ռ�����
//}

void delay_us(u32 nus)
{
	u32 i,j;
	for( i=0 ;i<nus;i++)
	{
		for( j=0 ; j<168 ; j++);//168MHz
	}
}

