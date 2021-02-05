/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* ������Ӳ��bspͷ�ļ� */
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"
#include "./delay/delay.h"
#include "./key/bsp_key.h" 
#include "./can/bsp_test_can.h"


/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

///**
//  * @brief  This function handles SVCall exception.
//  * @param  None
//  * @retval None
//  */
//void SVC_Handler(void)
//{}

///**
//  * @brief  This function handles PendSV_Handler exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
//systick�жϷ�����
void SysTick_Handler(void)
{	
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
        xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/


//USART1���ڵ���	
//uint8_t USART1_RX_BUF[REC_LEN];             //USART1���ջ���,���USART_REC_LEN���ֽ�
////����״̬
//u8 USART1_RX_flag = 0;//USART1����״̬���
//u8 USART1_RX_Num = 0;
	
uint8_t ucTemp1;
	
void DEBUG_USART_IRQHandler(void)
{
	if(USART_GetITStatus(DEBUG_USART,USART_IT_RXNE)!=RESET)
	{		
		ucTemp1 = USART_ReceiveData( DEBUG_USART );
    USART_SendData(DEBUG_USART,ucTemp1);    
	}	 
}	


	
	
/**
  * @}
  */ 
//extern uint8_t car_Left_flag;//����1��ת��־λ
//extern uint8_t car_Right_flag;//����2��ת��־λ
//extern uint8_t car_Speed_Up;//����1���ٱ�־λ
//extern uint8_t car_Speed_Down;//����2���ٱ�־λ
extern SemaphoreHandle_t BinarySem_Get_Camera_Handle;
void KEY1_IRQHandler(void)
{
	#if USE_SEM_CAMERA //ʹ�ö�ֵ�ź���
	BaseType_t pxHigherPriorityTaskWoken;
	uint32_t ulReturn;
	//�����ٽ��,�ٽ�ο���Ƕ��
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		//�ͷŶ�ֵ�ź��������ͽ��յ������ݱ�־����ǰ̨�����ѯ
		xSemaphoreGiveFromISR(BinarySem_Get_Camera_Handle,
											   &pxHigherPriorityTaskWoken);
		
		//�����Ҫ�Ļ�����һ�������л���ϵͳ���ж��Ƿ���Ҫ�����л�
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		LED3_ON;//��ɫ //��ʼ��������ͷ������
    //����жϱ�־λ
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
	#else
	 //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		CAMERA_Start_RX_flag = 1;//��ʼ����OpenMV
		LED3_ON;//��ɫ //��ʼ��������ͷ������
    //����жϱ�־λ
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	} 
	#endif
}


void KEY2_IRQHandler(void)
{
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		// LED2 ȡ��		
		LED1_TOGGLE;
    //����жϱ�־λ
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}
/**
  * @}
  */ 


/**********************can����************************************/
extern uint8_t rec_flag;
extern CanRxMsg test_RxMsg;  //���ջ�����
void TEST_CAN_IRQHandler(void)
{
	if(CAN_GetITStatus(TEST_CAN,CAN_IT_FMP0) == SET)
	{
		/*�ѽ��սṹ������*/
		test_RxMsg.StdId = 0x00; //��׼ID
		test_RxMsg.ExtId = 0x00;  //��չID
		test_RxMsg.IDE = CAN_ID_STD;//��ʼ��ΪĬ�ϱ�׼֡
		test_RxMsg.DLC = 0;   //����λ��
		test_RxMsg.FMI = 0;  //ɸѡ��0  ֮��Ӧ�û��
		
		CAN_Receive(TEST_CAN,CAN_FIFO0,&test_RxMsg); //��ȡ���Զ����
//		/* �Ƚ� ID �Ƿ�Ϊ 0x1314 */ ���Էŵ���������
//		if ((test_RxMsg.ExtId==0x1314) && (test_RxMsg.IDE==CAN_ID_EXT) && (test_RxMsg.DLC==8) )
//		{
//			rec_flag = 1; //���ճɹ�
//		}
//		else
//		{
//			rec_flag = 0; //����ʧ��
//		}
		rec_flag = FULL;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
