#ifndef __TB6612_H
#define __TB6612_H

#include "stm32f1xx_hal.h"

// ���Ŷ��� - �ɸ���ʵ�ʽ����޸�
#define TB6612_IN1_GPIO_Port    GPIOB
#define TB6612_IN1_Pin          GPIO_PIN_12
#define TB6612_IN2_GPIO_Port    GPIOB
#define TB6612_IN2_Pin          GPIO_PIN_13
#define TB6612_STBY_GPIO_Port   GPIOB
#define TB6612_STBY_Pin         GPIO_PIN_14
#define TB6612_PWMA_GPIO_Port  TIM3_GPIO_Port  
#define TB6612_PWMA_Pin        GPIO_PIN_6

// �����Ŷ���
typedef enum {
    MOTOR_A,  // ����IN1��IN2��PWMA
    MOTOR_B   // ����IN3��IN4��PWMB
} Motor_TypeDef;

// ���������
typedef enum {
    FORWARD,  // ��ת
    BACKWARD, // ��ת
    STOP,     // ֹͣ
    BRAKE     // �ƶ�
} Motor_Dir_TypeDef;

// ��������
void TB6612_Init(TIM_HandleTypeDef *htim, uint32_t ChannelA, uint32_t ChannelB);
void TB6612_SetMotorDir(Motor_TypeDef motor, Motor_Dir_TypeDef dir);
void TB6612_SetMotorSpeed(Motor_TypeDef motor, uint8_t speed);
void TB6612_SetStandby(uint8_t state);
uint8_t TB6612_CheckFault(void);

#endif
