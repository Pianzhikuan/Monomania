#ifndef __TEST_CAN_H
#define	__TEST_CAN_H
#include "stm32f4xx.h"
#include <stdio.h>
#include "./delay/delay.h"

typedef enum {
	Gears_P=0x40,   //P��--index��0
	Gears_D=0x41, 	//D��--index��0
	Gears_N=0x42, 	//N��--index��0
	Gears_R=0x43	  //R��--index��0
}Car_Gear;

typedef struct{
	uint8_t Gears;						//��λ0
	uint8_t TurnL;						//ת�ǵ�λ1
	uint8_t TurnH;						//ת�Ǹ�λ2
	uint8_t Speed;						//�ٶ�3
	uint8_t BrakeOrNot;				//�Ƿ�ɲ��4
	uint8_t Reserved5;				//����5
	uint8_t Reserved6;				//����6
	uint8_t Reserved7;				//����7
}Car_Data;

extern Car_Data car_Data;
//���Ŷ���
/*******************************************************/
#define TEST_CAN                             CAN2
#define TEST_CAN_CLK                         RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2
#define TEST_CAN_DIV                         12  //����500kHzʱ�ķ�Ƶϵ��

#define TEST_CAN_RX_GPIO_PORT                GPIOB
#define TEST_CAN_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define TEST_CAN_RX_PIN                      GPIO_Pin_12
#define TEST_CAN_RX_AF                       GPIO_AF_CAN2
#define TEST_CAN_RX_SOURCE                   GPIO_PinSource12

#define TEST_CAN_TX_GPIO_PORT                GPIOB
#define TEST_CAN_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define TEST_CAN_TX_PIN                      GPIO_Pin_13
#define TEST_CAN_TX_AF                       GPIO_AF_CAN2
#define TEST_CAN_TX_SOURCE                   GPIO_PinSource13

#define TEST_CAN_IRQHandler                  CAN2_RX0_IRQHandler
#define TEST_CAN_IRQ                 				 CAN2_RX0_IRQn
/************************************************************/
//С��������غ궨��
/************************************************************/
#define SINGLE_ANGLE 44.4 //ÿ1��Ƕ�ת�������ֵ��1200/27 = 44.4��
#define MAX_ANGLE   1200  //���Ƕ�Ϊ27��
#define TURN_STEP  (SINGLE_ANGLE * 3)   //ת�䲽��--ÿ�β���3��
#define SPEED_STEP  2   //�ٶȲ���--ÿ�β���0.2km/h
#define MAX_SPEED   200   //�ٶ�����20km/h
#define EMPTY       0   //�ȴ����ա�Ϊ��
#define FULL        1

extern uint8_t mail_box;
extern uint8_t i;

//ÿ����������˼��0-��λ 1~2-ת�� 3-�ٶ� 4-ɲ�� ����ļ�λ����
//uint8_t car_Forward[8] = {0x41,0x00,0x00,0x28,0x00,0x00,0x00,0x00};
//uint8_t car_Left[8] = {0x40,0xA8, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00};
//uint8_t car_Right[8] = {0x40,0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
//extern uint8_t car_Data[8];//��ʼ��Ϊɲ��

//extern const uint8_t Gears_P;//P��--index��0
//extern const uint8_t Gears_D;//D��--index��0
//extern const uint8_t Gears_N;//N��--index��0
//extern const uint8_t Gears_R;//R��--index��0

extern int16_t Turn_Sum;//ת��Ƕ�--index��1~2
extern uint8_t Speed_Sum; //�ٶ�0~20km/h----0~200--index��3
//extern const uint8_t car_Brake_On;//ɲ��--index��4
//extern const uint8_t car_Brake_Off;//��ɲ��--index��4

extern uint8_t rec_flag;//can���ձ�־λ
//extern uint8_t car_Left_flag;//����1��ת��־λ
//extern uint8_t car_Right_flag;//����2��ת��־λ
//extern uint8_t car_Speed_Up;//����1���ٱ�־λ
//extern uint8_t car_Speed_Down;//����2���ٱ�־λ

extern CanTxMsg test_TxMsg;//can���ͼĴ���
extern CanRxMsg test_RxMsg;//can���ռĴ���
/*******************************************************************/

void CAN_Config(void);//CAN����
void CAN_Filter_Config(void);
void car_Init(void);//�����ʼ��
//���ܺ���
void sandMassage(Car_Data* const Date);
void add_Speed(void);//����
void sub_Speed(void);//����
void left_Turn(void);//��ת
void right_Turn(void);//��ת
void stop_Car(void);//ɲ��














#endif /* __TEST_CAN_H */
