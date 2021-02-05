#include "./robot_arm/bsp_robot_arm.h"


/*****************************************************************
	* @brief  ��е�ۿ��ƺ��������ڸ���е�۷���modbus������Ϣ�����ݴ���
  * @param  buf��������Ϣ������
  * @retval �޷���ֵ
  ****************************************************************/
void sand_Robot_Arm_Info(uint8_t* buf)
{
	volatile int16_t x=0;
	volatile int16_t y=0;
	volatile int16_t z=0;
	//X/*************************////////////////////////////////////////
	if(buf[2]>>6) //�����7λΪ1����Ϊ����(����python����ʹ�õ�chr()�������ƣ��������õ�8λ)
	{
		buf[2] = buf[2] & (~((uint8_t)1<<6));
		x = (buf[2]-OFFSET) * FACTOR + (buf[3]-OFFSET);
		x = x*(-1);
		printf("����x: %d\r\n",x);
	}
	else //Ϊ����
	{
		x = (buf[2]-OFFSET) * FACTOR + (buf[3]-OFFSET);
		printf("����x: %d\r\n",x);
	}
	
	//Y/***************************/////////////////////////////////////////
	if(buf[4]>>6) //�����7λΪ1����Ϊ����(����python����ʹ�õ�chr()�������ƣ��������õ�8λ)
	{
		buf[4] = buf[4] & (~((uint8_t)1<<6));
		y = (buf[4]-OFFSET) * FACTOR + (buf[5]-OFFSET);
		y = y*(-1);
		printf("����y: %d\r\n",y);
	}
	else //Ϊ����
	{
		y = (buf[4]-OFFSET) * FACTOR + (buf[5]-OFFSET);
		printf("����y: %d\r\n",y);
	}
	
	//Z/**************************/////////////////////////////////////////
	if(buf[6]>>6)//�����8λΪ1����Ϊ����(����python����ʹ�õ�chr()�������ƣ��������õ�8λ)
	{
		buf[6] = buf[6] & (~((uint8_t)1<<6));
		z = (buf[6]-OFFSET) * FACTOR + (buf[7]-OFFSET);
		z = z*(-1);
		printf("����z: %d\r\n",z);
	}
	else //Ϊ����
	{
		z = (buf[6]-OFFSET) * FACTOR + (buf[7]-OFFSET);
		printf("����z: %d\r\n",z);
	}
	
	LED1_TOGGLE;
	//��е��modbus����
	//����ȫ�ֱ���GV1--X
	Send_Msg_toRobot_HoldReg(1,x);//GV1--��ʾ�����л�Ա�GV212  300mm
	//����ȫ�ֱ���GV2--Y
	Send_Msg_toRobot_HoldReg(2,y);
	//����ȫ�ֱ���GV3--Z
	Send_Msg_toRobot_HoldReg(3,z);
	//����ȫ�ֱ���GV4--��־λ100
	Send_Msg_toRobot_HoldReg(4,100);
}
