/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* ������Ӳ��bspͷ�ļ� */
#include "./led/bsp_led.h"
#include "./key/bsp_key.h" 
#include "./usart/bsp_debug_usart.h"
#include "./delay/delay.h"
#include "./can/bsp_test_can.h"
#include "./robot_arm/bsp_robot_arm.h"
#include "bsp_robot_gripper.h"

/*
LED 			PF6 		 PE7 			PE8
KEY				PA0 	   PC13
USART1		TX:PA9   RX:PA10
USART2    TX:PA2   RX:PA3
USART3    TX:PB10  RX:PB11
UART4     TX:PC10  RX:PC11
UART5     TX:PC12  RX:PD2      
CAN2      TX:PB13  RX:PB12
ͣ���������LED1
���յ������������ݣ����̵� LED2
��һ��KEY1��������ʼ����OpenMV�����ݣ������� LED3
*/
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
#define AppTaskCreat_PRIORITY      1    //�������� �������ȼ�
/* SCREEN��HC�������� */
static TaskHandle_t SCREEN_HC_Task_Handle;
#define SCREEN_HC_Task_PRIORITY    3    //��������HCģ������ȼ�
///* Gripperjixiez�������� */
//static TaskHandle_t Gripper_Task_Handle;
//#define Gripper_Task_PRIORITY    4    //Gripper��еצ�����ȼ�
/* CAMERA������ */
static TaskHandle_t CAMERA_Task_Handle = NULL;
#define CAMERA_Task_PRIORITY       2    //OpenMV�����ȼ� ��΢��һ�㣬��һһֱû�н��յ����ݣ���ѵ����ȼ����������
/* CAR_CAN������ --- �˺�������ֻ�Ƿ���canָ���С��*/
static TaskHandle_t CAR_CAN_Task_Handle = NULL;
#define CAR_CAN_Task_PRIORITY    5   	  //С��������ص����ȼ�


/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */

SemaphoreHandle_t BinarySem_Get_Camera_Handle = NULL; //��ֵ�ź������
/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void SCREEN_HC_Task(void* pvParameters);/* SCREEN_HC_Task����ʵ�� */
//static void Gripper_Task(void* pvParameters);/* Gripper_Task����ʵ�� */
static void CAMERA_Task(void* pvParameters);/* CAMERA_Task����ʵ�� */
static void CAR_CAN_Task(void* pvParameters);/* CAR_CAN_Task����ʵ�� */

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

  /* ������Ӳ����ʼ�� */
  BSP_Init();
	
	delay_us(0x100);
	//Usart_SendString(DEBUG_USART,(uint8_t*)"�½��Ƶ�USART2���ڳ���\t\n");
	LED1_ON;//ͣ�������
	
	printf("����һ��STM32F407������-FreeRTOS-���˳���Ŀ�����!\r\n\n");
	printf("\r\n");
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )128,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */ 
	
}



/*****************************************END OF FILE****************************************/

/***********************************************************************
  * @ ������  �� AppTaskCreate----������ѭ��
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���---����֮��Ͳ��ܱ�����ˣ�����
  
	/* ����BinarySem */
	BinarySem_Get_Camera_Handle = xSemaphoreCreateBinary();
	if(BinarySem_Get_Camera_Handle != NULL)
		printf("BinarySem_Get_Camera_Handle��ֵ�ź��������ɹ�\r\n");
  /* ����SCREEN_HC_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )SCREEN_HC_Task, /* ������ں��� */
                        (const char*    )"SCREEN_HC_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )SCREEN_HC_Task_PRIORITY,	    /* ��������ȼ�2 */
                        (TaskHandle_t*  )&SCREEN_HC_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����SCREEN_HC_Task����ɹ�!\r\n");
  
	/* ����CAMERA_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )CAMERA_Task, /* ������ں��� */
                        (const char*    )"CAMERA_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )CAMERA_Task_PRIORITY,	    /* ��������ȼ�3 */
                        (TaskHandle_t*  )&CAMERA_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����CAMERA_Task����ɹ�!\r\n");
  
	/* ����CAR_CAN_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )CAR_CAN_Task, /* ������ں��� */
                        (const char*    )"CAR_CAN_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )CAR_CAN_Task_PRIORITY,	    /* ��������ȼ�5 */
                        (TaskHandle_t*  )&CAR_CAN_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����CAR_CONTRL_Task����ɹ�!\r\n");
	printf("\r\n");
	
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  taskEXIT_CRITICAL();            //�˳��ٽ���
}

/**********************************************************************
  * @ ������  �� SCREEN_HC_Task
  * @ ����˵���� SCREEN_HC_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void SCREEN_HC_Task(void* parameter)
{	
    while (1)
    {
//			printf("����SCREEN_HC_Task����!\r\n");
			//���յ�����������---���ݴ���
			if(SCREEN_RX_Over_flag == 1)
			{
				printf("�յ�������ָ�\r\n");
//				vTaskSuspendAll();//����������������������������ڼ䲻������������
				//���ݴ���---�ٲô��������͹���������
				judge_Screen_Res(SCREEN_RX_BUF,SCREEN_RX_Num-2);
				printf("\r\n");
				//���ݴ�����ɣ���ʶ����
				SCREEN_RX_Num = 0;
				SCREEN_RX_Over_flag = 0;//�������
				vTaskDelay(10);//Ϊ����LEDС�ƶ���һ��
				LED2_OFF; //���մ���������
//				xTaskResumeAll();
			}
			
			//���յ�PC�˷��͵�����---���ݴ���
			if(HC_RX_Over_flag == 1)
			{
				printf("�յ�HCָ�\r\n");
//				vTaskSuspendAll();//����������������������������ڼ䲻������������		
				//���ݴ���---�ٲô��������͹���������
				judge_Screen_Res(HC_RX_BUF,HC_RX_Num-2);
				printf("\r\n");
				//���ݴ�����ɣ���ʶ����
				HC_RX_Num = 0;
				HC_RX_Over_flag = 0;//�������
				vTaskDelay(10);//Ϊ����LEDС�ƶ���һ��
				LED2_OFF; //����HC����
//				xTaskResumeAll();
			}
			//���ȼ��ϵͣ�ÿ50���룬���һ����û��ָ�����
			vTaskDelay(50);   /* ��ʱ50��tick */
    }
}




///**********************************************************************
//  * @ ������  �� Gripper_Task
//  * @ ����˵���� Gripper_Task��������
//  * @ ����    ��   
//  * @ ����ֵ  �� ��
//  ********************************************************************/
//static void Gripper_Task(void* parameter)
//{	
//    while (1)
//    {
////			printf("����Gripper_Task����!\r\n");
//			
////			//���յ�PC�˷��͵�����---���ݴ���
////			if(Gripper_RX_Over_flag == 1)
////			{
////				printf("�յ�HCָ�\r\n");

////				printf("\r\n");
////				//���ݴ�����ɣ���ʶ����
////				Gripper_RX_Num = 0;
////				Gripper_RX_Over_flag = 0;//�������
////				vTaskDelay(10);//Ϊ����LEDС�ƶ���һ��
////				LED2_OFF; //����HC����
//////				xTaskResumeAll();
////			}
//			//���ʹ���
//			Usart_SendStr_length( Gripper_UART ,test , 12 );
//			
//			//���ȼ��ϵͣ�ÿ50���룬���һ����û��ָ�����
//			vTaskDelay(50);   /* ��ʱ50��tick */
//    }
//}

/**********************************************************************
  * @ ������  �� CAMERA_Task//����KEY1��ʼִ��
  * @ ����˵���� CAMERA_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
#define DEAD_TIME  0xF0F
static void CAMERA_Task(void* parameter)
{	
	uint16_t dead_time = 0;
	uint8_t ch = 0;
	#if (USE_SEM_CAMERA==1) //����Ҫʹ�ö�ֵ�ź�������Ҫ��Ӻ궨��
		BaseType_t xReturn = pdPASS;//����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS
	#endif
    while (1)
    {
//			printf("����CAMERA_Task����!\r\n");
			#if (USE_SEM_CAMERA==1)
        //����KEY1��������ʼ����OpenMV������
			xReturn = xSemaphoreTake(BinarySem_Get_Camera_Handle,  //��ֵ�ź������
															 portMAX_DELAY); //һֱ�����ȴ�
			if(xReturn == pdTRUE)
			#else
			if(CAMERA_Start_RX_flag == 1)
			#endif
			{
//				CAMERA_Start_RX_flag��ʱ���ã�ʹ�ö�ֵ�ź���CPUЧ�ʸ���
				printf("��ʼ����OpenMV����Ϣ!\r\n");
			//	printf("��SCREEN_HC�������!\r\n");
				//vTaskSuspend(SCREEN_HC_Task_Handle);//����������������������������ڼ䲻������������
				CAMERA_RX_Num = 0;//�ٴγ�ʼ�����Է���һ
				
				do //�ȴ�֡ͷ��������֡ͷΪ0xA5
				{
					ch = get_USART3();
					dead_time++;
				}while(ch != 0xA5 & dead_time<DEAD_TIME); 
				
				CAMERA_RX_BUF[CAMERA_RX_Num++] = ch;//��֡ͷ������
				//����ѭ����ʼ��������
				while(1)
				{	
					if( dead_time==DEAD_TIME )
					{
						printf("OpenMV��������ʧ��\r\n");
						break;
					}
					else
					{
						dead_time = 0;
					}
					
					/////////////////////////////////
					ch = get_USART3();
					if(CAMERA_RX_Num <=  sizeof(CAMERA_RX_BUF))
					{
						CAMERA_RX_BUF[CAMERA_RX_Num++] = ch;
					}
					else
					{
						CAMERA_RX_Num = 0;
					}
					
					if (ch == 0x0A)	/* ���յ�֡β----0x0A�����ַ� */
					{	
						Usart_SendStr_length( DEBUG_USART, CAMERA_RX_BUF, CAMERA_RX_Num ); //--ͨ��USART1�����յ�������ͷ���ݷ��͸�����
						printf("OpenMV���ݸ�����%d",CAMERA_RX_Num);
						break;
						/*��⵽�лس��ַ��Ͱ����ݷ��ظ���λ��*/
					}
				}
				
					//�ָ���ʼ��
				CAMERA_RX_Num = 0;
				dead_time = 0;
				LED3_OFF;//��ɫ�ر�----�����������ͷ������
				
				#if (USE_SEM_CAMERA==0)  //��ʹ�ö�ֵ�ź���ʱ
				CAMERA_Start_RX_flag = 0;//��ʱ���ã����ö�ֵ�ź���
				vTaskDelay(50);   /* ��ʱ50��tick */
				#endif
				//vTaskResume(SCREEN_HC_Task_Handle);//�������رգ����Ա�����
			}
    }
}

/**********************************************************************
  * @ ������  �� CAR_CAN_Task
  * @ ����˵���� CAR_CAN_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void CAR_CAN_Task(void* parameter)
{  
	/* ���ڱ����ϴ�ʱ�䡣���ú�ϵͳ�Զ����� */
	static portTickType PreviousWakeTime;
	/* ������ʱʱ�䣬��ʱ��תΪ������ ÿ70���뷢��һ��ָ��*/
	const portTickType TimeIncrement = pdMS_TO_TICKS(pdMS_TO_TICKS(10));//Ҳ����ֱ��д50����Ϊ��ǰ����һ��tickΪ1ms
	/* ��ȡ��ǰϵͳʱ�� */
	PreviousWakeTime = xTaskGetTickCount();
	
    while (1)
    {
			/* ���þ�����ʱ����,����ʱ����Ϊ 50 �� tick */
			vTaskDelayUntil( &PreviousWakeTime,TimeIncrement );
			//vTaskDelay(50); 
		//printf("����CAR_CAN_Task��!\r\n");
		
			/************************can***************************/
			//״ָ̬ʾ��--�ٶ�Ϊ0ʱ�����
			if( car_Data.Speed==0 || Speed_Sum==0x00 )
			{
				LED1_ON;//ͣ�������
				
			}
			else
			{
				LED1_OFF;
			}
			/**************���Ϸ���can���ݿ���С��***************************************/
			sandMassage(&car_Data);
		}
}

/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
//  /*��ʼ������*/
  EXTI_Key_Config();
//	
//	//USART��س�ʼ��
	BSP_USART_Config();
//	delay_us(0x100);
	
	//С��can��ʼ��
	car_Init();
	
	
	//*****************************��ʼ����еצ����***************************************
	init_Robot_Gripper();

}

