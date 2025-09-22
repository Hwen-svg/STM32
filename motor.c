#include "motor.h"

// 全局变量
static TIM_HandleTypeDef *tb6612_htim;
static uint32_t tb6612_ChannelA;
static uint32_t tb6612_ChannelB;

/**
 * @brief  初始化TB6612电机驱动
 * @param  htim: 定时器句柄
 * @param  ChannelA: 电机A的PWM通道
 * @param  ChannelB: 电机B的PWM通道
 * @retval 无
 */
void TB6612_Init(TIM_HandleTypeDef *htim, uint32_t ChannelA, uint32_t ChannelB) {
    // 保存定时器和通道信息
    tb6612_htim = htim;
    tb6612_ChannelA = ChannelA;
    tb6612_ChannelB = ChannelB;
    
    // 配置控制引脚为输出模式
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // IN1、IN2、IN3、IN4、STBY引脚配置
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    GPIO_InitStruct.Pin = TB6612_IN1_Pin;
    HAL_GPIO_Init(TB6612_IN1_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TB6612_IN2_Pin;
    HAL_GPIO_Init(TB6612_IN2_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TB6612_STBY_Pin;
    HAL_GPIO_Init(TB6612_STBY_GPIO_Port, &GPIO_InitStruct);
    
    // 初始化状态：所有电机停止，启用驱动
    TB6612_SetMotorDir(MOTOR_A, STOP);
    TB6612_SetMotorDir(MOTOR_B, STOP);
    TB6612_SetStandby(1);  // 1: 正常工作, 0: 待机
    
    // 启动PWM
    HAL_TIM_PWM_Start(tb6612_htim, tb6612_ChannelA);
    HAL_TIM_PWM_Start(tb6612_htim, tb6612_ChannelB);
}

/**
 * @brief  设置电机方向
 * @param  motor: 电机编号(MOTOR_A或MOTOR_B)
 * @param  dir: 方向(FORWARD, BACKWARD, STOP, BRAKE)
 * @retval 无
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
 * @brief  设置电机速度
 * @param  motor: 电机编号(MOTOR_A或MOTOR_B)
 * @param  speed: 速度值(0-100)，对应占空比0%-100%
 * @retval 无
 */
void TB6612_SetMotorSpeed(Motor_TypeDef motor, uint8_t speed) {
    // 限制速度范围
    if (speed > 100) speed = 100;
    
    // 计算PWM比较值（假设定时器ARR为1000）
    uint32_t pwm_val = (speed * 1000) / 100;
    
    if (motor == MOTOR_A) {
        __HAL_TIM_SET_COMPARE(tb6612_htim, tb6612_ChannelA, pwm_val);
    } else {
        __HAL_TIM_SET_COMPARE(tb6612_htim, tb6612_ChannelB, pwm_val);
    }
}

/**
 * @brief  设置待机模式
 * @param  state: 0-待机模式，1-正常工作模式
 * @retval 无
 */
void TB6612_SetStandby(uint8_t state) {
    if (state) {
        HAL_GPIO_WritePin(TB6612_STBY_GPIO_Port, TB6612_STBY_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(TB6612_STBY_GPIO_Port, TB6612_STBY_Pin, GPIO_PIN_RESET);
    }
}

/**
 * @brief  检查故障状态
 * @retval 0-正常，1-故障（过流保护触发）
 */

