#ifndef __BSP_JDY31_H
#define __BSP_JDY31_H

#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdio.h>


// ����ģ��ʹ�õ�UART������ⲿ���壩

extern UART_HandleTypeDef huart1;  // ���ӵ��ԵĴ���


// ��������С����
#define JDY31_RX_BUF_SIZE  128     // �������ջ�������С
#define PC_TX_BUF_SIZE     128     // ���Է��ͻ�������С

// ����ģ���ʼ��
void JDY31_Init(void);

// �������ݵ�����ģ��
void JDY31_SendData(uint8_t *data, uint16_t len);

// �����ַ���������ģ��
void JDY31_SendString(uint8_t *str);

// ������յ����������ݣ�ת�������ԣ�
void JDY31_ProcessReceivedData(void);

//�����ַ���OLED������ʾ
void JDY31_ShowOnOLED(void);

#endif /* __BSP_JDY31_H */
