#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f1xx_hal.h"  // 补充核心头文件
#include "stm32f1xx_hal_gpio.h"
#include "tim.h"

// 统一宏定义名称（将TYPE改为PORT，与.c文件保持一致）
#define DHT11_GPIO_PORT    GPIOA
#define DHT11_GPIO_PIN     GPIO_PIN_1
#define DHT11_GPIO_CLK_EN  __HAL_RCC_GPIOA_CLK_ENABLE()  // 修正宏名称

// 引脚操作宏（保持与PORT匹配）
#define DHT11_OUT_H      HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET)
#define DHT11_OUT_L      HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET)
#define DHT11_IN         HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

// 函数声明
void dht11_gpio_input(void);
void dht11_gpio_output(void);
void dht11_reset(void);
uint16_t dht11_scan(void);
uint16_t dht11_read_bit(void);
uint16_t dht11_read_byte(void);
uint16_t dht11_read_data(uint8_t buffer[5]);
void delay_us(uint32_t us);

#endif  // 确保文件最后有一个空行（解决warning #1-D）

