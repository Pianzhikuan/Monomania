#include "bsp_robot_gripper.h"

static uint8_t test_init[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x08, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFB};//��ʼ��
static uint8_t test0[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x10, 0x09, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFB};//��I/O ģʽ����д������
static uint8_t test1[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x10, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFB};//(���õ�һ���г� �պ�λ�� 0)
static uint8_t test2[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x10, 0x02, 0x01, 0x00, 0x5A, 0x00, 0x00, 0x00, 0xFB};//(���õ�һ���г� �ſ�λ�� 90)
static uint8_t test3[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x10, 0x03, 0x01, 0x00, 80, 0x00, 0x00, 0x00, 0xFB};//(���õ�һ���г� �г��� 60)
static uint8_t test4[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x10, 0x0A, 0x01, 0x00, 60, 0x00, 0x00, 0x00, 0xFB};//(���õ�һ���г� ����� 60)
/*****************************************************************
	* @brief  ��ʼ����еצ
  * @param  
  * @retval �޷���ֵ
  ****************************************************************/
void init_Robot_Gripper(void)
{
	LED3_ON;
	  //���ʹ���
	Usart_SendStr_length( Gripper_UART ,test_init , 14 );
	delay_ms(30);//����
	Usart_SendStr_length( Gripper_UART ,test0 , 14 );
	delay_ms(30);//����
	Usart_SendStr_length( Gripper_UART ,test1 , 14 );
	delay_ms(30);//����
	Usart_SendStr_length( Gripper_UART ,test2 , 14 );
	delay_ms(30);//����
	Usart_SendStr_length( Gripper_UART ,test3 , 14 );
	delay_ms(30);//����
	Usart_SendStr_length( Gripper_UART ,test4 , 14 );
	LED3_OFF;
	delay_ms(0x0FF);//����
	delay_ms(0x0FF);//����
	delay_ms(0x0FF);//����
	delay_ms(30);//����
	LED2_ON;//��ʼ�����
	delay_ms(0x0FF);//����
	delay_ms(0x0FF);//����
	delay_ms(0x0FF);//����
	LED2_OFF;
}

////ץס
//void gripperCatch(void);
////�ɿ�
//void gripperPut(void);
