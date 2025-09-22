#include "DHT11.h"


// ????dht11.h?ж??????o?
// #define DHT11_GPIO_PORT    GPIOA
// #define DHT11_GPIO_PIN     GPIO_PIN_0
// #define DHT11_GPIO_CLK_EN  __HAL_RCC_GPIOA_CLK_ENABLE()

void dht11_gpio_input(void)
{
    GPIO_InitTypeDef g;
    
    DHT11_GPIO_CLK_EN;  // 现在宏名称与.h文件一致
    
    g.Pin = DHT11_GPIO_PIN;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_NOPULL;
    
    HAL_GPIO_Init(DHT11_GPIO_PORT, &g);  // 端口宏已统一
}

void dht11_gpio_output(void)
{
    GPIO_InitTypeDef g;
    
    DHT11_GPIO_CLK_EN;  // ???GPIO???
    
    g.Pin = DHT11_GPIO_PIN;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    g.Mode = GPIO_MODE_OUTPUT_PP;   // ???????
    g.Pull = GPIO_NOPULL;           // ????????

    HAL_GPIO_Init(DHT11_GPIO_PORT, &g);
}

// ??????????????
#define DHT11_OUT_L  HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET)
#define DHT11_OUT_H  HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET)
#define DHT11_IN     HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

void dht11_reset(void)
{
    // ????DHT11?????
    dht11_gpio_output();
    DHT11_OUT_L;
    HAL_Delay(20);      // ????????18ms
    DHT11_OUT_H;
    delay_us(30);       // ????30us
    dht11_gpio_input();
}

uint16_t dht11_scan(void)
{
    return DHT11_IN;
}

uint16_t dht11_read_bit(void)
{
    while (DHT11_IN == GPIO_PIN_RESET);
    delay_us(40);
    if (DHT11_IN == GPIO_PIN_SET)
    {
        while (DHT11_IN == GPIO_PIN_SET);
        return 1;
    }
    else
    {
        return 0;
    }
}

uint16_t dht11_read_byte(void)
{
    uint16_t i;
    uint16_t data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= dht11_read_bit();
    }
    return data;
}

uint16_t dht11_read_data(uint8_t buffer[5])
{
    uint16_t i = 0;
    
    dht11_reset();
    if (dht11_scan() == GPIO_PIN_RESET)
    {
        // ???DHT11???
        while (dht11_scan() == GPIO_PIN_RESET);
        while (dht11_scan() == GPIO_PIN_SET);
        
        // ???5???????
        for (i = 0; i < 5; i++)
        {
            buffer[i] = dht11_read_byte();
        }
        
        // ?????????
        while (dht11_scan() == GPIO_PIN_RESET);
        dht11_gpio_output();
        DHT11_OUT_H;
        
        // У??????
        uint8_t checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
        if (checksum != buffer[4])
        {
            return 1;  // У?????
        }
    }
    else
    {
        return 2;      // ?????
    }
    
    return 0;          // ??????
}

// ?????????????
void delay_us(uint32_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 1000000) / 8;
    while (ticks--);
}

