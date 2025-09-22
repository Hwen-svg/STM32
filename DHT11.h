#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f1xx_hal.h"  // �������ͷ�ļ�
#include "stm32f1xx_hal_gpio.h"
#include "tim.h"

// ͳһ�궨�����ƣ���TYPE��ΪPORT����.c�ļ�����һ�£�
#define DHT11_GPIO_PORT    GPIOA
#define DHT11_GPIO_PIN     GPIO_PIN_1
#define DHT11_GPIO_CLK_EN  __HAL_RCC_GPIOA_CLK_ENABLE()  // ����������

// ���Ų����꣨������PORTƥ�䣩
#define DHT11_OUT_H      HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET)
#define DHT11_OUT_L      HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET)
#define DHT11_IN         HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

// ��������
void dht11_gpio_input(void);
void dht11_gpio_output(void);
void dht11_reset(void);
uint16_t dht11_scan(void);
uint16_t dht11_read_bit(void);
uint16_t dht11_read_byte(void);
uint16_t dht11_read_data(uint8_t buffer[5]);
void delay_us(uint32_t us);

#endif  // ȷ���ļ������һ�����У����warning #1-D��

