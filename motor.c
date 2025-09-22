#include "motor.h"

// ȫ�ֱ���
static TIM_HandleTypeDef *tb6612_htim;
static uint32_t tb6612_ChannelA;
static uint32_t tb6612_ChannelB;

/**
 * @brief  ��ʼ��TB6612�������
 * @param  htim: ��ʱ�����
 * @param  ChannelA: ���A��PWMͨ��
 * @param  ChannelB: ���B��PWMͨ��
 * @retval ��
 */
void TB6612_Init(TIM_HandleTypeDef *htim, uint32_t ChannelA, uint32_t ChannelB) {
    // ���涨ʱ����ͨ����Ϣ
    tb6612_htim = htim;
    tb6612_ChannelA = ChannelA;
    tb6612_ChannelB = ChannelB;
    
    // ���ÿ�������Ϊ���ģʽ
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // IN1��IN2��IN3��IN4��STBY��������
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = TB6612_IN1_Pin;
    HAL_GPIO_Init(TB6612_IN1_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TB6612_IN2_Pin;
    HAL_GPIO_Init(TB6612_IN2_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TB6612_STBY_Pin;
    HAL_GPIO_Init(TB6612_STBY_GPIO_Port, &GPIO_InitStruct);
    
    // ��ʼ��״̬�����е��ֹͣ����������
    TB6612_SetMotorDir(MOTOR_A, STOP);
    TB6612_SetMotorDir(MOTOR_B, STOP);
    TB6612_SetStandby(1);  // 1: ��������, 0: ����
    
    // ����PWM
    HAL_TIM_PWM_Start(tb6612_htim, tb6612_ChannelA);
    HAL_TIM_PWM_Start(tb6612_htim, tb6612_ChannelB);
}

/**
 * @brief  ���õ������
 * @param  motor: ������(MOTOR_A��MOTOR_B)
 * @param  dir: ����(FORWARD, BACKWARD, STOP, BRAKE)
 * @retval ��
 */
void TB6612_SetMotorDir(Motor_TypeDef motor, Motor_Dir_TypeDef dir) {
    if (motor == MOTOR_A) {
        switch(dir) {
            case FORWARD:
                HAL_GPIO_WritePin(TB6612_IN1_GPIO_Port, TB6612_IN1_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(TB6612_IN2_GPIO_Port, TB6612_IN2_Pin, GPIO_PIN_RESET);
                break;
            case BACKWARD:
                HAL_GPIO_WritePin(TB6612_IN1_GPIO_Port, TB6612_IN1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(TB6612_IN2_GPIO_Port, TB6612_IN2_Pin, GPIO_PIN_SET);
                break;
            case STOP:
                HAL_GPIO_WritePin(TB6612_IN1_GPIO_Port, TB6612_IN1_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(TB6612_IN2_GPIO_Port, TB6612_IN2_Pin, GPIO_PIN_RESET);
                break;
            case BRAKE:
                HAL_GPIO_WritePin(TB6612_IN1_GPIO_Port, TB6612_IN1_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(TB6612_IN2_GPIO_Port, TB6612_IN2_Pin, GPIO_PIN_SET);
                break;
        }
    }
}

/**
 * @brief  ���õ���ٶ�
 * @param  motor: ������(MOTOR_A��MOTOR_B)
 * @param  speed: �ٶ�ֵ(0-100)����Ӧռ�ձ�0%-100%
 * @retval ��
 */
void TB6612_SetMotorSpeed(Motor_TypeDef motor, uint8_t speed) {
    // �����ٶȷ�Χ
    if (speed > 100) speed = 100;
    
    // ����PWM�Ƚ�ֵ�����趨ʱ��ARRΪ1000��
    uint32_t pwm_val = (speed * 1000) / 100;
    
    if (motor == MOTOR_A) {
        __HAL_TIM_SET_COMPARE(tb6612_htim, tb6612_ChannelA, pwm_val);
    } else {
        __HAL_TIM_SET_COMPARE(tb6612_htim, tb6612_ChannelB, pwm_val);
    }
}

/**
 * @brief  ���ô���ģʽ
 * @param  state: 0-����ģʽ��1-��������ģʽ
 * @retval ��
 */
void TB6612_SetStandby(uint8_t state) {
    if (state) {
        HAL_GPIO_WritePin(TB6612_STBY_GPIO_Port, TB6612_STBY_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(TB6612_STBY_GPIO_Port, TB6612_STBY_Pin, GPIO_PIN_RESET);
    }
}

/**
 * @brief  ������״̬
 * @retval 0-������1-���ϣ���������������
 */

