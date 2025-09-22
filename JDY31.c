#include "JDY31.h"
#include "OLED.h"

// ȫ�ֱ���
uint8_t jdy31_rx_buf[JDY31_RX_BUF_SIZE];  // �������ջ�����
uint8_t jdy31_rx_len = 0;                 // �������ݳ���
uint8_t jdy31_rx_complete = 0;            // ������ɱ�־

uint8_t pc_tx_buf[PC_TX_BUF_SIZE];        // ���Է��ͻ�����

// ȫ�ֱ������������ջ�����
uint8_t bt_rx_buffer[128];  // ������128���ַ�
uint16_t bt_rx_len = 0;     // ��ǰ���ճ���


/**
 * @brief  ��ʼ��JDY31����ģ��
 * @param  ��
 * @retval ��
 */
void JDY31_Init(void) {
    // �����������ڽ����ж�
    HAL_UART_Receive_IT(&huart1, &jdy31_rx_buf[jdy31_rx_len], 1);
    
}

/**
 * @brief  �������ݵ�����ģ��
 * @param  data: Ҫ���͵�����
 * @param  len: ���ݳ���
 * @retval ��
 */
void JDY31_SendData(uint8_t *data, uint16_t len) {
    HAL_UART_Transmit(&huart1, data, len, 100);
}

/**
 * @brief  �����ַ���������ģ��
 * @param  str: Ҫ���͵��ַ���
 * @retval ��
 */
void JDY31_SendString(uint8_t *str) {
    JDY31_SendData(str, strlen((char*)str));
}

/**
 * @brief  ������յ����������ݣ�ת�������ԣ�
 * @param  ��
 * @retval ��
 */
void JDY31_ProcessReceivedData(void) {
    if (jdy31_rx_complete) {
        
        // ��ջ������ͱ�־
        memset(jdy31_rx_buf, 0, JDY31_RX_BUF_SIZE);
        jdy31_rx_len = 0;
        jdy31_rx_complete = 0;
        
        // ���¿��������ж�
        HAL_UART_Receive_IT(&huart1, &jdy31_rx_buf[jdy31_rx_len], 1);
    }
}

/**
 * @brief  �������ڽ����жϻص�����
 * @param  huart: UART���
 * @retval ��
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {  // ȷ��������ģ��ʹ�õĴ���
        jdy31_rx_len++;
        
        // �������������յ����з��򻺳�����
        if (jdy31_rx_buf[jdy31_rx_len-1] == '\n' || jdy31_rx_len >= JDY31_RX_BUF_SIZE-1) {
            jdy31_rx_complete = 1;  // ��ǽ������
        } else {
            // ����������һ���ֽ�
            HAL_UART_Receive_IT(&huart1, &jdy31_rx_buf[jdy31_rx_len], 1);
        }
    }
}

/**
 * @brief  ���������������ݲ���OLED����ʾ
 * @param  ��
 * @retval ��
 * @note   �����յ�������������ʱ����OLED����ʾ���ݣ�ÿ����ʾǰ�����Ļ
 */
void JDY31_ShowOnOLED(void) {
    if (jdy31_rx_complete) {
        // 1. �����Ļ��ȷ��������Ч��
        OLED_FullyClear();
        HAL_Delay(10);  // �ȴ��������

        // 2. Ϊ���յ��ַ�����ӽ��������������룩
        jdy31_rx_buf[jdy31_rx_len] = '\0';

        // 3. ��OLED��ʾ��ʹ��8*16���壬��������
        OLED_ShowStr(0, 0, jdy31_rx_buf, 2);  // �����С2��8*16��

        // 4. ���û�������׼����һ�ν���
        memset(jdy31_rx_buf, 0, JDY31_RX_BUF_SIZE);
        jdy31_rx_len = 0;
        jdy31_rx_complete = 0;

        // 5. ���¿����жϽ���
        HAL_UART_Receive_IT(&huart1, &jdy31_rx_buf[jdy31_rx_len], 1);
    }
}

