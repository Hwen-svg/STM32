#include "JDY31.h"
#include "OLED.h"

// 全局变量
uint8_t jdy31_rx_buf[JDY31_RX_BUF_SIZE];  // 蓝牙接收缓冲区
uint8_t jdy31_rx_len = 0;                 // 接收数据长度
uint8_t jdy31_rx_complete = 0;            // 接收完成标志

uint8_t pc_tx_buf[PC_TX_BUF_SIZE];        // 电脑发送缓冲区

// 全局变量：蓝牙接收缓冲区
uint8_t bt_rx_buffer[128];  // 最大接收128个字符
uint16_t bt_rx_len = 0;     // 当前接收长度


/**
 * @brief  初始化JDY31蓝牙模块
 * @param  无
 * @retval 无
 */
void JDY31_Init(void) {
    // 启动蓝牙串口接收中断
    HAL_UART_Receive_IT(&huart1, &jdy31_rx_buf[jdy31_rx_len], 1);
    
}

/**
 * @brief  发送数据到蓝牙模块
 * @param  data: 要发送的数据
 * @param  len: 数据长度
 * @retval 无
 */
void JDY31_SendData(uint8_t *data, uint16_t len) {
    HAL_UART_Transmit(&huart1, data, len, 100);
}

/**
 * @brief  发送字符串到蓝牙模块
 * @param  str: 要发送的字符串
 * @retval 无
 */
void JDY31_SendString(uint8_t *str) {
    JDY31_SendData(str, strlen((char*)str));
}

/**
 * @brief  处理接收到的蓝牙数据（转发到电脑）
 * @param  无
 * @retval 无
 */
void JDY31_ProcessReceivedData(void) {
    if (jdy31_rx_complete) {
        
        // 清空缓冲区和标志
        memset(jdy31_rx_buf, 0, JDY31_RX_BUF_SIZE);
        jdy31_rx_len = 0;
        jdy31_rx_complete = 0;
        
        // 重新开启接收中断
        HAL_UART_Receive_IT(&huart1, &jdy31_rx_buf[jdy31_rx_len], 1);
    }
}

/**
 * @brief  蓝牙串口接收中断回调函数
 * @param  huart: UART句柄
 * @retval 无
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {  // 确认是蓝牙模块使用的串口
        jdy31_rx_len++;
        
        // 检测结束条件：收到换行符或缓冲区满
        if (jdy31_rx_buf[jdy31_rx_len-1] == '\n' || jdy31_rx_len >= JDY31_RX_BUF_SIZE-1) {
            jdy31_rx_complete = 1;  // 标记接收完成
        } else {
            // 继续接收下一个字节
            HAL_UART_Receive_IT(&huart1, &jdy31_rx_buf[jdy31_rx_len], 1);
        }
    }
}

/**
 * @brief  处理蓝牙接收数据并在OLED上显示
 * @param  无
 * @retval 无
 * @note   当接收到完整蓝牙数据时，在OLED上显示内容，每次显示前清空屏幕
 */
void JDY31_ShowOnOLED(void) {
    if (jdy31_rx_complete) {
        // 1. 清空屏幕（确保清屏生效）
        OLED_FullyClear();
        HAL_Delay(10);  // 等待清屏完成

        // 2. 为接收的字符串添加结束符（避免乱码）
        jdy31_rx_buf[jdy31_rx_len] = '\0';

        // 3. 在OLED显示（使用8*16字体，更清晰）
        OLED_ShowStr(0, 0, jdy31_rx_buf, 2);  // 字体大小2（8*16）

        // 4. 重置缓冲区，准备下一次接收
        memset(jdy31_rx_buf, 0, JDY31_RX_BUF_SIZE);
        jdy31_rx_len = 0;
        jdy31_rx_complete = 0;

        // 5. 重新开启中断接收
        HAL_UART_Receive_IT(&huart1, &jdy31_rx_buf[jdy31_rx_len], 1);
    }
}

