#ifndef __MODBUS_H
#define	__MODBUS_H

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"
#include "./delay/delay.h"

/* ������			���ݷ���
 * 02--0x02   ����ɢ������
 * 01--0x01   ����Ȧ
 * 03--0x03   ��������ּĴ���
 * 04--0x04   ������Ĵ���
 * 05--0x05   д������Ȧ
 * 15--0x0F   д�����Ȧ
 * 06--0x06   д�����Ĵ���
 * 16--0x10   д����Ĵ��� 
 --------�Զ���ͨ�üĴ���M---------
 0x41����0x01��
 0x42����0x02д
 */

//д����Ȧʱʹ��
#define MODBUS_ON  0xFF00
#define MODBUS_OFF 0x0000
#define MODBUS_ID  0x01   //����е�۵�����Ҫ��һһ��Ӧ
//����״̬   
extern uint8_t Recive_Buff[8];//ԭ����8  �ܹ�8���ֽ�
extern u8 MODBUS_RX_flag;//USART2������ϱ��
extern u8 MODBUS_RX_Num;


//���ܺ���#################################################################
/* ��  ����
 * puchMsg  �D�D const BYTE*��ָ���У�����ݴ洢�����׵�ַ
 * usDataLen �D�D ��У�����ݵ��ֽڸ���
 * ����ֵ��WORD�ͣ��������CRCֵ��   */
uint16_t CalCrcFast(const uint8_t* puchMsg , uint16_t usDataLen);

/****************************************************************************/
/**
* @brief  �������ݸ���е�۳�ȥ32λ��ȫ�ֱ���������Ȧ�������������
	* @param  id-վ��  fun-������ address-��ַ  data-����
  * @retval ��
	* ��ʱֻ֧��0x05д����Ȧ��0x06д���ּĴ���
  */
void Send_Msg_toRobot(uint8_t id,uint8_t fun,uint16_t address,uint16_t data);
/**
	* @brief  �������ݸ���е����Ȧ--��������0x05
	* @param   address-��ַ  data-����
  * @retval ��������ָ��
	* ��ʱֻ֧��0x05д����Ȧ��0x06д���ּĴ���
  */
void Send_Msg_toRobot_Coil(uint16_t address,uint16_t data);
/**
* @brief  �������ݸ���е��ȫ�ֱ��������ּĴ�����--��������0x06
	* @param  id-վ��   address-��ַ  data-����
  * @retval ��
	* ��ʱֻ֧��0x05д����Ȧ��0x06д���ּĴ���
  */
void Send_Msg_toRobot_HoldReg(uint16_t address,int16_t data);

/****************************************************************************/
//���ܺ궨��##################################################

//1-���г��� 0x40-���ؽ����������˶� x041-�������������˶�
#define RUN_Program(x)     Send_Msg_toRobot(0x01,0x06,440,x)
//ֹͣ����
#define STOP_Program       Send_Msg_toRobot(0x01,0x06,441,1)
//��ͣ����
#define PAUSE_Program      Send_Msg_toRobot(0x01,0x06,442,1)
//��������
#define CONTINUE_Program   Send_Msg_toRobot(0x01,0x06,443,1)
//ȫ����������λ
#define RUN_TO_ZERO        Send_Msg_toRobot(0x01,0x06,445,11)


#endif /* __MODBUS_H */

