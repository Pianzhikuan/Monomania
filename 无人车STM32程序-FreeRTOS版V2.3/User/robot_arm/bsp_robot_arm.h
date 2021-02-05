#ifndef __ROBOT_ARM_H
#define	__ROBOT_ARM_H

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"
#include "./delay/delay.h"
#include "./can/bsp_test_can.h"
#include "./modbus/bsp_modbus.h"

#define FACTOR 				100  //ϵ�� ����python����ʱ���ǲ𿪷��͵ģ�������Ҫ��100
//���ڴ���10��ӦASCII�еĻس����������⣬Ϊ������11��ƫ��
#define OFFSET 				11  

void sand_Robot_Arm_Info(uint8_t* buf);



#endif /* __ROBOT_ARM_H */
