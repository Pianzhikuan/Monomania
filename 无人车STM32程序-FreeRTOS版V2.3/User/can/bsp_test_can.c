#include "./can/bsp_test_can.h"

uint8_t mail_box = 0;
uint8_t i = 0;
uint8_t countOf_Brake_On = 2;//ÿ�ΰ�ɲ�����ᷢ�͵Ĵ���
//ÿ����������˼��0-��λ 1~2-ת�� 3-�ٶ� 4-ɲ�� ����ļ�λ����
//uint8_t car_Forward[8] = {0x41,0x00,0x00,0x28,0x00,0x00,0x00,0x00};
//uint8_t car_Left[8] = {0x40,0xA8, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00};
//uint8_t car_Right[8] = {0x40,0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
//uint8_t car_Data[8] = {0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};//��ʼ��Ϊɲ��
Car_Data car_Data = {Gears_N, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};//Car_Data���ݳ�ʼ��

//const uint8_t Gears_P = 0x40;//P��--index��0
//const uint8_t Gears_D = 0x41;//D��--index��0
//const uint8_t Gears_N = 0x42;//N��--index��0
//const uint8_t Gears_R = 0x43;//R��--index��0

int16_t Turn_Sum = 0;//ת��Ƕ�--index��1~2
uint8_t Speed_Sum = 0; //�ٶ�0~20km/h----0~200--index��3
const uint8_t car_Brake_On = 0x02;//ɲ��--index��4
const uint8_t car_Brake_Off = 0x00;//��ɲ��--index��4

uint8_t rec_flag = EMPTY;//can���ձ�־λ

CanTxMsg test_TxMsg;//can���ͼĴ���
CanRxMsg test_RxMsg;//can���ռĴ���

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = TEST_CAN_IRQ;
  /* �������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ�Ϊ3 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  TEST_CAN GPIO ����,����ģʽ���ûػ�����ģʽ��1MHz��
  * @param  ��
  * @retval ��
  */
void CAN_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_InitTypeDef CAN_InitStructure;
	
	//ʹ��GPIO��ʱ��
  RCC_AHB1PeriphClockCmd(TEST_CAN_RX_GPIO_CLK|TEST_CAN_TX_GPIO_CLK,ENABLE);

  /* ʹ�� USART ʱ�� */
  RCC_APB1PeriphClockCmd(TEST_CAN_CLK, ENABLE);
  
	  
 /* ���� PXx �� CANTx_Tx*/
  GPIO_PinAFConfig(TEST_CAN_RX_GPIO_PORT,TEST_CAN_RX_SOURCE,TEST_CAN_RX_AF);

  /*  ���� PXx �� CANx__Rx*/
  GPIO_PinAFConfig(TEST_CAN_TX_GPIO_PORT,TEST_CAN_TX_SOURCE,TEST_CAN_TX_AF);
	
  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = TEST_CAN_TX_PIN  ;  
  GPIO_Init(TEST_CAN_RX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = TEST_CAN_RX_PIN;
  GPIO_Init(TEST_CAN_TX_GPIO_PORT, &GPIO_InitStructure);
  
  /* ���ûػ�ģʽ */
  /* ���������ã�TEST_CAN_DIV */
	CAN_InitStructure.CAN_ABOM = ENABLE;//�Զ������߹رչ���
	CAN_InitStructure.CAN_TTCM = DISABLE; //ʱ�䴥��ͨ��ģʽ
	CAN_InitStructure.CAN_AWUM = ENABLE;  //�Զ�����
	CAN_InitStructure.CAN_NART = ENABLE;  //�Զ��ش�  �����Զ��ش����ɹرգ�
	CAN_InitStructure.CAN_RFLM = DISABLE;  //������FIFO(�����Ļ��ᶪ���±��ģ����������±��ľ��ܸ��Ǿɱ���)
	CAN_InitStructure.CAN_TXFP = DISABLE;  //ʹ��ID�Ƚ���Ҫ�������ǰ���˳����
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack; //˫��ʱ��Ϊ����ģʽCAN_Mode_Normal  �ػ�CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = TEST_CAN_DIV;  //����Ϊ12��Ƶ  ---500KHz
 /* CAN Baudrate = 1 MBps (1MBps ��Ϊ stm32 �� CAN �������) (CAN ʱ��Ƶ��Ϊ APB 1 = 42 MHz) 
35 ////BTR-BRP �����ʷ�Ƶ�� ������ʱ�䵥Ԫ��ʱ�䳤�� 42/(1+4+2)/6=1 Mbps*/

  /* ���USART��ʼ������ */
  CAN_Init(TEST_CAN, &CAN_InitStructure); 
	
  /* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
  
	/* ʹ�ܴ��ڽ����ж� */   //ʹ��FIFIO0�ж�
	CAN_ITConfig(TEST_CAN, CAN_IT_FMP0, ENABLE);
	
  /* ʹ�ܴ��� */
  //USART_Cmd(DEBUG_USART, ENABLE);
}



//��ʼ��ɸѡ��
void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef CAN_Filter_Struct;
	
	CAN_Filter_Struct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0  ;//�洢��FIFO0����Ϊʹ����FIFO0�ж�
	CAN_Filter_Struct.CAN_FilterNumber = 14;  //ʹ��can1ʱ��Χ��0~13��ʹ��can2ʱ��Χ��14~27
	CAN_Filter_Struct.CAN_FilterMode = CAN_FilterMode_IdMask;//����ģʽ�������б�ģʽ��
	CAN_Filter_Struct.CAN_FilterScale = CAN_FilterScale_32bit;//��չģʽ��������32λ
	CAN_Filter_Struct.CAN_FilterActivation = ENABLE;//����
	
	//ɸѡ��������16λʱ���õ�ʱHigh��16λ����������32λ��ʱ�����ȫ��
	CAN_Filter_Struct.CAN_FilterIdHigh = (((uint32_t)0x120 <<3 |CAN_Id_Extended|CAN_RTR_Data)&0xFFFF0000)>>16;
	CAN_Filter_Struct.CAN_FilterIdLow = ((uint32_t)0x120 <<3 |CAN_Id_Extended|CAN_RTR_Data)&0x0000FFFF;
	CAN_Filter_Struct.CAN_FilterMaskIdHigh = 0x0;
	CAN_Filter_Struct.CAN_FilterMaskIdLow = 0x0;  //����0x1314��0x1315��ID���Խ���
	
	CAN_FilterInit(&CAN_Filter_Struct);
}

void car_Init(void)
{
	//can��ʼ��
	CAN_Config();
	CAN_Filter_Config();
	//��ʼ��---С����ʼ��ʱ����ɲ��
	car_Data.Gears = Gears_N;  //֮ǰ��0x40
	car_Data.TurnL = 0x00;//�Ƕ�L
	car_Data.TurnH = 0x00;//�Ƕ�H
	car_Data.Speed = 0x00;//�ٶ�
	car_Data.BrakeOrNot = car_Brake_Off;//һ��ʼ�Ȳ�ɲ��
	Speed_Sum = 0;
	Turn_Sum = 0;
	
}

/*****************************************CAN �������****************************************/
void sandMassage(Car_Data* const Data)
{
	//�������� ----�ɷ�װ�ɺ���
	test_TxMsg.StdId = 0x0120;
	test_TxMsg.ExtId = 0x00;
	test_TxMsg.IDE = CAN_Id_Standard;
	test_TxMsg.RTR = CAN_RTR_Data;  //��������������֡����ң��֡
	test_TxMsg.DLC = 8; //�����ֽ���
	
	{
		test_TxMsg.Data[0] = Data->Gears;
		printf("%#x  ",Data->Gears);
		test_TxMsg.Data[1] = Data->TurnL;
		test_TxMsg.Data[2] = Data->TurnH;
		test_TxMsg.Data[3] = Data->Speed;
		test_TxMsg.Data[4] = Data->BrakeOrNot;
		printf("%#x\r\n",Data->BrakeOrNot);
		test_TxMsg.Data[5] = Data->Reserved5;
		test_TxMsg.Data[6] = Data->Reserved6;
		test_TxMsg.Data[7] = Data->Reserved7;
	}
	
	//��������
	mail_box = CAN_Transmit(TEST_CAN,&test_TxMsg);//��������
	
	if(mail_box != CAN_TxStatus_NoMailBox)
	{
		//�ȴ�stm32�ɹ����͸�CAN�շ���
		while((CAN_TransmitStatus(TEST_CAN,mail_box)) != CAN_TxStatus_Ok);
		
		delay_us(1000);//��һ����ʱ����CAN�շ����ɹ����͵�����
		
//		printf("CANָ��ͳɹ�!\r\n");
	}
	else
	{
		printf("\t\nThere is no mail box!\r\n");
	}
}


void add_Speed(void)
{
	//���ڿյ�����ʼP����ǰ����ʱ��
	if(car_Data.Gears != Gears_R)
	{
		car_Data.Gears = Gears_D;//ǰ����
		car_Data.BrakeOrNot = car_Brake_Off;
		Speed_Sum += SPEED_STEP;
	}
	//���ڼ��ٵ�ʱ��
	else if(car_Data.Gears == Gears_R)
	{
		Speed_Sum -= SPEED_STEP;
	}
	
	if(Speed_Sum > MAX_SPEED)
	{
		Speed_Sum = MAX_SPEED;
	}
	else if(Speed_Sum < (SPEED_STEP-1))
	{
		Speed_Sum = 0x00;
		car_Data.Gears = Gears_N;//�ص��յ�
	}
	car_Data.Speed = Speed_Sum;
}


void sub_Speed(void)
{
	//���ڿյ�����ʼP�������ٵ�ʱ��
	if(car_Data.Gears != Gears_D)
	{
		car_Data.Gears = Gears_R;//ǰ����
		car_Data.BrakeOrNot = car_Brake_Off;
		Speed_Sum += SPEED_STEP;
	}
	//����ǰ����ʱ��
	else if(car_Data.Gears == Gears_D)
	{
		Speed_Sum -= SPEED_STEP;
	}
	
	if(Speed_Sum > MAX_SPEED)
	{
		Speed_Sum = MAX_SPEED;
	}
	else if(Speed_Sum < (SPEED_STEP-1))
	{
		Speed_Sum = 0x00;
		car_Data.Gears = Gears_N;//�ص��յ�
	}
	car_Data.Speed = Speed_Sum;
}

void left_Turn(void)
{
	Turn_Sum -= TURN_STEP;//���ղ���ֵ��
	if(Turn_Sum < -MAX_ANGLE)
	{
		Turn_Sum = -MAX_ANGLE;//���27��
	}
	else if((Turn_Sum > -(TURN_STEP-100)) && (Turn_Sum < SINGLE_ANGLE)) 
	{
		Turn_Sum = 0;
	}
	car_Data.TurnL = (Turn_Sum&0x00FF);
	car_Data.TurnH = (Turn_Sum&0xFF00)>>8;
}

void right_Turn(void)
{
	Turn_Sum += TURN_STEP;
	if(Turn_Sum > MAX_ANGLE)
	{
		Turn_Sum = MAX_ANGLE;
	}
	else if(Turn_Sum < (TURN_STEP-100)&& (Turn_Sum > (-SINGLE_ANGLE)))
	{
		Turn_Sum = 0;
	}
	car_Data.TurnL = (Turn_Sum&0x00FF);
	car_Data.TurnH = (Turn_Sum&0xFF00)>>8;
}

void stop_Car(void)
{
	car_Data.Gears = Gears_N; //�ص��յ�   ֮ǰ��P
	car_Data.Speed = 0x00;//�ٶ�
	
	if(car_Data.BrakeOrNot == car_Brake_Off)
		car_Data.BrakeOrNot = car_Brake_On;//ɲ��
	else
		car_Data.BrakeOrNot = car_Brake_Off;//��ڶ��ξͲ�ɲ����
		
	Speed_Sum = 0;//�ٶ�Ϊ0
}

//void stop_Car(void)
//{
//		car_Data.Gears = Gears_N; //�ص��յ�   ֮ǰ��P
//		car_Data.Speed = 0x00;//�ٶ�
//	car_Data[4] = car_Brake_On;//ɲ��
//	Speed_Sum = 0;//�ٶ�Ϊ0
//	countOf_Brake_On = 2;
//}

