#ifndef __BSP_JDY31_H
#define __BSP_JDY31_H

#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdio.h>


// 蓝牙模块使用的UART句柄（外部定义）

extern UART_HandleTypeDef huart1;  // 连接电脑的串口


// 缓冲区大小定义
#define JDY31_RX_BUF_SIZE  128     // 蓝牙接收缓冲区大小
#define PC_TX_BUF_SIZE     128     // 电脑发送缓冲区大小

// 蓝牙模块初始化
void JDY31_Init(void);

// 发送数据到蓝牙模块
void JDY31_SendData(uint8_t *data, uint16_t len);

// 发送字符串到蓝牙模块
void JDY31_SendString(uint8_t *str);

// 处理接收到的蓝牙数据（转发到电脑）
void JDY31_ProcessReceivedData(void);

//接收字符在OLED上面显示
void JDY31_ShowOnOLED(void);

#endif /* __BSP_JDY31_H */
