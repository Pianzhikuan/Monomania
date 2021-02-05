#include "./usart/bsp_debug_usart.h"

 /**
  * @brief  ���ڳ�ʼ��,����ģʽ���á�115200 8-N-1
  * @param  USARTx���ڼ�������   bound��������Ϊ����
  * @retval ��
  */
#if 1  //����ʱ������Ҫ�رմ��ڣ�������Ϊ0
 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */  //4bit for ��ռ���ȼ� 0bit for �����ȼ�  �� 16����0����
  
	/*************************USART1�ж�����****************************/
  /* ����USART1Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�Ϊ7 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  /* �����ȼ�Ϊ0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
	/*************************USART2�ж�����****************************/
	/* ����USART2Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = SCREEN_USART_IRQ;
	/* �������ȼ�Ϊ8 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	 /* �����ȼ�Ϊ0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
	
	/*************************USART3�ж�����****************************/
	#if (USE_USART3_Handler == 1)  //ʹ��USART3 �ж�
	/* ����USART2Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = CAMERA_USART_IRQ;
	/* �������ȼ�Ϊ9 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;
	 /* �����ȼ�Ϊ2 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
	#endif
	
	/*************************UART4�ж�����****************************/
	/* ����UART4Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = HC_UART_IRQ;
	/* �������ȼ�Ϊ10 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	 /* �����ȼ�Ϊ0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
	/*************************UART5�ж�����****************************/
	/* ����UART5Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = Gripper_UART_IRQ;
	/* �������ȼ�Ϊ11 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
	 /* �����ȼ�Ϊ0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void Debug_USART_Config(unsigned int bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);

  /* ʹ�� USART ʱ�� */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;  
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
  
  /* ���ô�DEBUG_USART ģʽ */
  /* ���������ã�DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
  /* �ֳ�(����λ+У��λ)��8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* ֹͣλ��1��ֹͣλ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* У��λѡ�񣺲�ʹ��У�� */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* Ӳ�������ƣ���ʹ��Ӳ���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ���USART��ʼ������ */
  USART_Init(DEBUG_USART, &USART_InitStructure); 
	
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
	
  /* ʹ�ܴ��� */
  USART_Cmd(DEBUG_USART, ENABLE);
}


void SCREEN_USART_Config(unsigned int bound, uint8_t Parity)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(SCREEN_USART_RX_GPIO_CLK|SCREEN_USART_TX_GPIO_CLK,ENABLE);

  /* ʹ�� USART2 ʱ�� */  //��APB1�ϣ���
  RCC_APB1PeriphClockCmd(SCREEN_USART_CLK, ENABLE);
  
  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = SCREEN_USART_TX_PIN  ;  
  GPIO_Init(SCREEN_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = SCREEN_USART_RX_PIN;
  GPIO_Init(SCREEN_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(SCREEN_USART_RX_GPIO_PORT,SCREEN_USART_RX_SOURCE,SCREEN_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(SCREEN_USART_TX_GPIO_PORT,SCREEN_USART_TX_SOURCE,SCREEN_USART_TX_AF);
  
  /* ���ô�DEBUG_USART ģʽ */
  /* ���������ã�DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
  /* ֹͣλ��1��ֹͣλ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	
  //��У��
	if(Parity == 1)
	{
		/* �ֳ�(����λ+У��λ)��9 */
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		/* У��λѡ�񣺲�ʹ��У�� */
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
	}
	else if(Parity == 2)
	{
		/* �ֳ�(����λ+У��λ)��9 */
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		/* У��λѡ�񣺲�ʹ��У�� */
		USART_InitStructure.USART_Parity = USART_Parity_Even;
	}
	//��У��
	else if (Parity == 0)
	{
		/* �ֳ�(����λ+У��λ)��8 */
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		/* У��λѡ�񣺲�ʹ��У�� */
		USART_InitStructure.USART_Parity = USART_Parity_No;
	}
	
	
  /* Ӳ�������ƣ���ʹ��Ӳ���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ���USART��ʼ������ */
  USART_Init(SCREEN_USART, &USART_InitStructure); 
  
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(SCREEN_USART, USART_IT_RXNE, ENABLE);
	
  /* ʹ�ܴ��� */
  USART_Cmd(SCREEN_USART, ENABLE);
}



void CAMERA_USART_Config(unsigned int bound,uint8_t Parity)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	
	//��GPIOʱ��
  RCC_AHB1PeriphClockCmd(CAMERA_USART_RX_GPIO_CLK|CAMERA_USART_TX_GPIO_CLK,ENABLE);

  /* ʹ�� USART3 ʱ�� */  //��APB1�ϣ���
  RCC_APB1PeriphClockCmd(CAMERA_USART_CLK, ENABLE);
  
  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = CAMERA_USART_TX_PIN  ;  
  GPIO_Init(CAMERA_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = CAMERA_USART_RX_PIN;
  GPIO_Init(CAMERA_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(CAMERA_USART_RX_GPIO_PORT,CAMERA_USART_RX_SOURCE,CAMERA_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(CAMERA_USART_TX_GPIO_PORT,CAMERA_USART_TX_SOURCE,CAMERA_USART_TX_AF);
  
  
  /* ���ô�DEBUG_USART ģʽ */
  /* ���������ã�DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
	/* ֹͣλ��1��ֹͣλ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	
	//��У��
	if(Parity == 1)
	{
		/* �ֳ�(����λ+У��λ)��9 */
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		/* У��λѡ�񣺲�ʹ��У�� */
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
	}
	else if(Parity == 2)
	{
		/* �ֳ�(����λ+У��λ)��9 */
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		/* У��λѡ�񣺲�ʹ��У�� */
		USART_InitStructure.USART_Parity = USART_Parity_Even;
	}
	//��У��
	else if (Parity == 0)
	{
		/* �ֳ�(����λ+У��λ)��8 */
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		/* У��λѡ�񣺲�ʹ��У�� */
		USART_InitStructure.USART_Parity = USART_Parity_No;
	}
	
  /* Ӳ�������ƣ���ʹ��Ӳ���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ���USART3��ʼ������ */
  USART_Init(CAMERA_USART, &USART_InitStructure); 
  
	#if USE_USART3_Handler 
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(CAMERA_USART, USART_IT_RXNE, ENABLE);
	#endif
	
  /* ʹ�ܴ��� */
  USART_Cmd(CAMERA_USART, ENABLE);
}


void HC_UART_Config(unsigned int bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(HC_UART_RX_GPIO_CLK|HC_UART_TX_GPIO_CLK,ENABLE);

  /* ʹ�� uart4 ʱ�� */  //��APB1�ϣ���
  RCC_APB1PeriphClockCmd(HC_UART_CLK, ENABLE);
  
  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = HC_UART_TX_PIN  ;  
  GPIO_Init(HC_UART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = HC_UART_RX_PIN;
  GPIO_Init(HC_UART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(HC_UART_RX_GPIO_PORT,HC_UART_RX_SOURCE,HC_UART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(HC_UART_TX_GPIO_PORT,HC_UART_TX_SOURCE,HC_UART_TX_AF);
  
  /* ���ô�DEBUG_USART ģʽ */
  /* ���������ã�DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
  /* �ֳ�(����λ+У��λ)��8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* ֹͣλ��1��ֹͣλ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* У��λѡ�񣺲�ʹ��У�� */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* Ӳ�������ƣ���ʹ��Ӳ���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ���USART��ʼ������ */
  USART_Init(HC_UART, &USART_InitStructure); 
  
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(HC_UART, USART_IT_RXNE, ENABLE);
	
  /* ʹ�ܴ��� */
  USART_Cmd(HC_UART, ENABLE);
}

void Gripper_UART_Config(unsigned int bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(Gripper_UART_RX_GPIO_CLK|Gripper_UART_TX_GPIO_CLK,ENABLE);

  /* ʹ�� uart4 ʱ�� */  //��APB1�ϣ���
  RCC_APB1PeriphClockCmd(Gripper_UART_CLK, ENABLE);
  
  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = Gripper_UART_TX_PIN  ;  
  GPIO_Init(Gripper_UART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = Gripper_UART_RX_PIN;
  GPIO_Init(Gripper_UART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(Gripper_UART_RX_GPIO_PORT,Gripper_UART_RX_SOURCE,Gripper_UART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(Gripper_UART_TX_GPIO_PORT,Gripper_UART_TX_SOURCE,Gripper_UART_TX_AF);
  
  /* ���ô�DEBUG_USART ģʽ */
  /* ���������ã�DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
  /* �ֳ�(����λ+У��λ)��8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* ֹͣλ��1��ֹͣλ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* У��λѡ�񣺲�ʹ��У�� */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* Ӳ�������ƣ���ʹ��Ӳ���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ���USART��ʼ������ */
  USART_Init(Gripper_UART, &USART_InitStructure); 
  
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
	
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(Gripper_UART, USART_IT_RXNE, ENABLE);
	
  /* ʹ�ܴ��� */
  USART_Cmd(Gripper_UART, ENABLE);
}


void BSP_USART_Config(void)
{
	Debug_USART_Config(115200);
//	SCREEN_USART_Config(115200,Parity_No);
	SCREEN_USART_Config(19200,Parity_Odd);//modbusͨ��
	CAMERA_USART_Config(19200,Parity_Odd);//modbusͨ��
	delay_us(0x100);
	
	HC_UART_Config(9600);
	Gripper_UART_Config(115200);//���ƻ�еצ  RS485
	
	//Gripper_UART_Config();
//	/* Ƕ�������жϿ�����NVIC���� */
//	NVIC_Configuration();
}


#endif

/*****************  ����һ���ַ� **********************/
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART2 */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  ָ�����ȣ��ַ��������ķ����ַ��� **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� �����ֽ� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}



///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����DEBUG_USART */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE******************************/
/******************************************************************************************/
/*************************������ش�����*************************************************/

/**********************************************************************
  * @ ������  �� judge_Screen_Res
  * @ ����˵���� �������ݴ���USART2 �� UART4 ---PC
  * @ ����    �� ������
  * @ ����ֵ  �� ��
  ********************************************************************/
void judge_Screen_Res(uint8_t* buf,uint8_t num)//�������ж�
{
	//������ֵ����������壺0~2-֡ͷ 2~7-�£�x��y��z  8-֡β
	//
	uint8_t ch;
	ch = buf[num];
	uint8_t ZhuaCatch[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x10, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFB};//�����Ƽ�צ�����һ��λ�ã�//�պ�
	uint8_t ZhuaPut[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x10, 0x04, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFB};//�����Ƽ�צ����ڶ���λ�ã�//��
//	printf("ch=%#x\r\n",ch);
//	printf("buf0=%#x\r\n",buf[0]);
//	printf("buf1=%#x\r\n",buf[1]);
//	printf("buf2=%#x\r\n",buf[2]);
//	printf("buf3=%#x\r\n",buf[3]);
//	printf("buf4=%#x\r\n",buf[4]);
//	printf("buf5=%#x\r\n",buf[5]);
//	printf("buf6=%#x\r\n",buf[6]);
//	printf("buf7=%#x\r\n",buf[7]);
//	printf("buf8=%#x\r\n",buf[8]);
	if(buf[0] == 0x5A)//˵���ǿ���С����
	{
		if(ch == 0x00) //ǰ��
		{
			add_Speed();
			printf("ǰ��\r\n");
		}
		else if(ch == 0x01)//��ת
		{
			left_Turn();
			printf("��ת\r\n");
		}
		else if(ch == 0x02)//����
		{
			sub_Speed();
			printf("����\r\n");
		}
		else if(ch == 0x03)//��ת
		{
			right_Turn();
			printf("��ת\r\n");
		}
		else if(ch == 0x04)//ɲ��
		{
			stop_Car();
			printf("ɲ��\r\n");
		}
		else if(ch == 0x50)//ץ��
		{
			LED3_TOGGLE;
			Usart_SendStr_length( Gripper_UART ,ZhuaCatch , 14 );
			printf("ץ��\r\n");
		}
		else if(ch == 0x0F)//�ɿ�
		{
			LED3_TOGGLE;
			Usart_SendStr_length( Gripper_UART ,ZhuaPut , 14 );
			printf("�ɿ�\r\n");
		}
		else
		{
			Usart_SendString(DEBUG_USART,(uint8_t*)"ѡ�����������ѡ��\t\n");
		}
		
		printf("��ǰ�ٶ�Ϊ��%.2f KM/s\r\n",(Speed_Sum/10.0));
		printf("��ǰת��Ϊ��%.1f��\r\n",(Turn_Sum/SINGLE_ANGLE));
	}
	else if(buf[0] == 0x3D)//˵���ǿ��ƻ�е�۵�
	{
		printf("modbus���ͻ�е�ۿ�����Ϣ\r\n");
		sand_Robot_Arm_Info(buf);
	}
}






/******************************************************************************************/
/*************************�жϽ�����س���*************************************************/
/**********************USART2_IRQHandler begin ********************************************/
uint8_t SCREEN_RX_BUF[SCREEN_REC_LEN];             //USART2���ջ���,���USART_REC_LEN���ֽ�
//����״̬               
u8 SCREEN_RX_Over_flag = 0;//USART2������ϱ��
u8 SCREEN_RX_Num = 0;

//USART2��������֡ͷ��֡β
u8 SCREEN_DATA_BEGIN1  = 0x5A;
u8 SCREEN_DATA_BEGIN2  = 0xA5;
u8 SCREEN_DATA_END = 0x0A;


//����2�жϷ�����򣬴��ڽ�����ȷ���ݴ�{��ͷ+ID+����+����λ} �磺#1+1@ 
//USART1_RX_STA��Ϊ״̬�Ĵ�����15λ��Ϊ֡β��־λ��14λ��Ϊ֡ͷ2��־λ��13λ��Ϊ֡ͷ1��־λ
void SCREEN_USART_IRQHandler(void)                	
{
	uint8_t ucTemp;
	delay_us(5);
	if(USART_GetITStatus(SCREEN_USART, USART_IT_RXNE) != RESET)      //�����ж�
	{
		
		ucTemp = USART_ReceiveData( SCREEN_USART );

			if (SCREEN_RX_Num < sizeof(SCREEN_RX_BUF) && (SCREEN_RX_Over_flag==0))
			{
				SCREEN_RX_BUF[SCREEN_RX_Num++] = ucTemp;
			}
			else
			{
				SCREEN_RX_Num = 0;
			}
			
			/* �򵥵�ͨ��Э�飬�����س����з���Ϊ1������֡�������м������ж�ʵ���Զ������� */
			/* ���������ַ�����Ϊ���յ�һ������ */
			if (ucTemp == SCREEN_DATA_END)	/* �����ַ� */
			{	
				LED2_ON;//���յ�������������
				SCREEN_RX_Over_flag =1; //�������
				/*��⵽�лس��ַ��Ͱ����ݷ��ظ���λ��*/
			}
	}
}  



/**********************USART3_IRQHandler begin ********************************************/
uint8_t CAMERA_RX_BUF[CAMERA_REC_LEN];             //USART3���ջ���,���USART_REC_LEN���ֽ�
//����״̬               
u8 CAMERA_RX_flag = 0;//USART2������ϱ��
u8 CAMERA_Start_RX_flag = 0;//USART3��ʼ���ձ��
u8 CAMERA_RX_Num = 0;

//USART3��������֡ͷ��֡β
u8 CAMERA_DATA_BEGIN1  = 0x5A;
u8 CAMERA_DATA_BEGIN2  = 0xA5;
u8 CAMERA_DATA_END = 0x0A;


#if USE_USART3_Handler //������USART3�Ĵ����ж�
////����3�жϷ�����򣬴��ڽ�����ȷ���ݴ�{��ͷ+ID+����+����λ} �磺#1+1@ 
////USART1_RX_STA��Ϊ״̬�Ĵ�����15λ��Ϊ֡β��־λ��14λ��Ϊ֡ͷ2��־λ��13λ��Ϊ֡ͷ1��־λ
//void CAMERA_USART_IRQHandler(void)                	
//{
//	uint8_t ucTemp;
//	LED3_ON;
//	delay_us(5);
//	if(USART_GetITStatus(CAMERA_USART, USART_IT_RXNE) != RESET)      //ȷ�����뵽��USART3�Ľ����ж�
//	{
//		ucTemp = USART_ReceiveData( CAMERA_USART ); //��������

//			if (CAMERA_RX_Num < sizeof(CAMERA_RX_BUF))
//			{
//				CAMERA_RX_BUF[CAMERA_RX_Num++] = ucTemp;
//			}
//			else
//			{
//				CAMERA_RX_Num = 0;
//			}
//			
//			/* �򵥵�ͨ��Э�飬�����س����з���Ϊ1������֡�������м������ж�ʵ���Զ������� */
//			/* ���������ַ�����Ϊ���յ�һ������ */
//			if (ucTemp == CAMERA_DATA_END)	/* �����ַ� */
//			{		
//				CAMERA_RX_flag =1; //�������
//				/*��⵽�лس��ַ��Ͱ����ݷ��ظ���λ��*/
//			}
//	}
//} 
/**********************USART3_IRQHandler begin ********************************************/
uint8_t Recive_Buff[8];//ԭ����8  �ܹ�8���ֽ�
//����״̬               
u8 MODBUS_RX_flag = 0;//USART3������ϱ��
u8 MODBUS_RX_Num = 0;

//USART1��������֡ͷ��֡β
u8 CAMERA_DATA_BEGIN1  = 0x01;

//����3�жϷ�����򣬴��ڽ�����ȷ���ݴ�{��ͷ+ID+����+����λ} �磺#1+1@ 
//USART1_RX_STA��Ϊ״̬�Ĵ�����15λ��Ϊ֡β��־λ��14λ��Ϊ֡ͷ2��־λ��13λ��Ϊ֡ͷ1��־λ
void CAMERA_USART_IRQHandler(void)                	
{
	uint8_t ucTemp;
	uint16_t crc = 0;
	uint8_t crcH = 0;
	uint8_t crcL = 0;
	LED3_ON;
	delay_us(5);
	if(USART_GetITStatus(CAMERA_USART, USART_IT_RXNE) != RESET)      //�����ж�
	{
		ucTemp = USART_ReceiveData( CAMERA_USART );
		USART_SendData(SCREEN_USART,ucTemp); 
		USART_SendData(DEBUG_USART,ucTemp);   //���յ����ݾ���������
		if (MODBUS_RX_Num < sizeof(Recive_Buff))
		{
			Recive_Buff[MODBUS_RX_Num++] = ucTemp;
			if(MODBUS_RX_Num == 8)
			{
				crc = CalCrcFast(Recive_Buff,6);
				crcH = ((crc>>8)&0xff);
				crcL = (crc&0xff);
				if(Recive_Buff[6]==crcH && Recive_Buff[7]==crcL)
				{
					MODBUS_RX_flag = 1;
					MODBUS_RX_Num = 0;
				}
			}
		}
		else
		{
			MODBUS_RX_Num = 0;
		}
	}
}  

#endif

/**********************USART3_���жϽ��պ���********************************************/
#if !USE_USART3_Handler  //���û�д򿪴����ж�
uint8_t get_USART3(void)
{
	uint16_t dead_time = 0;
	/* һֱ��ͣ�ĵȴ������������� */
		while (USART_GetFlagStatus(CAMERA_USART, USART_FLAG_RXNE) == RESET & dead_time < 0xFFF)
		{
			dead_time++;
		}

		return (uint8_t)USART_ReceiveData(CAMERA_USART);
}
#endif


/**********************UART4_IRQHandler begin ********************************************/
uint8_t HC_RX_BUF[HC_REC_LEN];             //USART2���ջ���,���USART_REC_LEN���ֽ�
//����״̬               
u8 HC_RX_Over_flag = 0;//USART2������ϱ��
u8 HC_RX_Num = 0;

//USART2��������֡ͷ��֡β
u8 HC_DATA_BEGIN1  = 0x5A;
u8 HC_DATA_BEGIN2  = 0xA5;//��ʱδ��
u8 HC_DATA_END = 0x0A;


//�����жϷ�����򣬴��ڽ�����ȷ���ݴ�{��ͷ+ID+����+����λ} �磺#1+1@ 
//USART1_RX_STA��Ϊ״̬�Ĵ�����15λ��Ϊ֡β��־λ��14λ��Ϊ֡ͷ2��־λ��13λ��Ϊ֡ͷ1��־λ
void HC_UART_IRQHandler(void)                	
{
	uint8_t ucTemp;
	
	delay_us(5);
	if(USART_GetITStatus(HC_UART, USART_IT_RXNE) != RESET)      //�����ж�
	{
		ucTemp = USART_ReceiveData( HC_UART );

			if (HC_RX_Num < sizeof(HC_RX_BUF) && (HC_RX_Over_flag==0))
			{
				HC_RX_BUF[HC_RX_Num++] = ucTemp;
			}
			else
			{
				HC_RX_Num = 0;
			}
			
			/* �򵥵�ͨ��Э�飬�����س����з���Ϊ1������֡�������м������ж�ʵ���Զ������� */
			/* ���������ַ�����Ϊ���յ�һ������ */
			if (ucTemp == HC_DATA_END)	/* �����ַ� */
			{	
				LED2_ON;//���յ�������������
				HC_RX_Over_flag =1; //�������
				/*��⵽�лس��ַ��Ͱ����ݷ��ظ���λ��*/
			}
	}
}  


/**********************UART5_IRQHandler begin ********************************************/
uint8_t Gripper_RX_BUF[Gripper_REC_LEN];             //USART2���ջ���,���USART_REC_LEN���ֽ�
//����״̬               
u8 Gripper_RX_Over_flag = 0;//USART2������ϱ��
u8 Gripper_RX_Num = 0;

//USART2��������֡ͷ��֡β
u8 Gripper_DATA_BEGIN1  = 0xFF;
u8 Gripper_DATA_BEGIN2  = 0xFF;//��ʱδ��
u8 Gripper_DATA_END = 0x0A;


//����2�жϷ�����򣬴��ڽ�����ȷ���ݴ�{��ͷ+ID+����+����λ} �磺#1+1@ 
//USART1_RX_STA��Ϊ״̬�Ĵ�����15λ��Ϊ֡β��־λ��14λ��Ϊ֡ͷ2��־λ��13λ��Ϊ֡ͷ1��־λ
void Gripper_UART_IRQHandler(void)                	
{
	uint8_t ucTemp;
	
	delay_us(5);
	if(USART_GetITStatus(Gripper_UART, USART_IT_RXNE) != RESET)      //�����ж�
	{
		ucTemp = USART_ReceiveData( Gripper_UART );
		Usart_SendByte(DEBUG_USART,ucTemp);
		LED2_TOGGLE;
//			if (Gripper_RX_Num < sizeof(Gripper_RX_BUF) && (Gripper_RX_Over_flag==0))
//			{
//				Gripper_RX_BUF[Gripper_RX_Num++] = ucTemp;
//			}
//			else
//			{
//				Gripper_RX_Num = 0;
//			}
//			
//			
//			/* �򵥵�ͨ��Э�飬�����س����з���Ϊ1������֡�������м������ж�ʵ���Զ������� */
//			/* ���������ַ�����Ϊ���յ�һ������ */
//			if (ucTemp == Gripper_DATA_END)	/* �����ַ� */
//			{	
//				LED2_ON;//���յ�������������
//				Gripper_RX_Over_flag =1; //�������
//				/*��⵽�лس��ַ��Ͱ����ݷ��ظ���λ��*/
//			}
	}
}  
