#include "stm32f1xx.h"
#include "tim.h"
#include "dht11.h"
 
uint8_t Data[5]={0x00,0x00,0x00,0x00,0x00};//存储温湿度
 
//微妙延时
void Delay_us(uint16_t us)
{
	uint16_t dif=0xffff-us-5;
	__HAL_TIM_SET_COUNTER(&htim3,dif);//设置TIM1计数器起始值
	HAL_TIM_Base_Start(&htim3);//启动
	
	while(dif<0xffff-5)
	{
		dif=__HAL_TIM_GET_COUNTER(&htim3);//查询计数
	}
	HAL_TIM_Base_Stop(&htim3);
}
void DHT_GPIO_SET_OUTPUT(void)//输出
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void DHT_GPIO_SET_INPUT(void)//输入
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
uint8_t DHT_Read_Byte(void)//读取一位信号
{
	uint8_t ReadData=0;//存放8bit数据
	uint8_t temp;			//临时存放信号电平
	uint8_t retry=0;	//
	uint8_t i;
	for(i=0;i<8;i++)//一次读取8位
	{
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==0&&retry<100)//等待DHT11输出高电平
		{
			Delay_us(1);
			retry++;
		}
		retry=0;
		Delay_us(40);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==1)
		{
			temp=1;
		}
		else
			temp=0;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==1&&retry<100)
		{
			Delay_us(1);
			retry++;
		}
		retry=0;
		ReadData<<=1;//左移
		ReadData |=temp;
		
	}
	return ReadData;
	
}
//DHT11读取温湿度程序
uint8_t DHT_Read(void)
{
	uint8_t retry=0;
	uint8_t i;
	
	DHT_GPIO_SET_OUTPUT();//IO设置为输出模式
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);//拉低电平18ms
	HAL_Delay(18);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);//拉高电平20us
	Delay_us(20);
	
	DHT_GPIO_SET_INPUT();
	Delay_us(20);
	//如果检测到低电平，说明DHT11有响应
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==0)
	{
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==0&&retry<100)
		{
			Delay_us(1);
			retry++;
		}
		retry=0;
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==1&&retry<100)
		{
			Delay_us(1);
			retry++;
		}
		retry=0;
		//一次传输40位，一次返回8位，共读取5次
		for(i=0;i<5;i++)
		{
			Data[i]=DHT_Read_Byte();
		}
		Delay_us(50);
	}
 
	//校验
	uint32_t sum=Data[0]+Data[1]+Data[2]+Data[3];
	 if((sum)==Data[4])    
		 return 1;  
	 else   
	 return 0;
}





 
 