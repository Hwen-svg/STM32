#include "stm32f1xx.h"
#include "tim.h"
#include "dht11.h"
 
uint8_t Data[5]={0x00,0x00,0x00,0x00,0x00};//�洢��ʪ��
 
//΢����ʱ
void Delay_us(uint16_t us)
{
	uint16_t dif=0xffff-us-5;
	__HAL_TIM_SET_COUNTER(&htim3,dif);//����TIM1��������ʼֵ
	HAL_TIM_Base_Start(&htim3);//����
	
	while(dif<0xffff-5)
	{
		dif=__HAL_TIM_GET_COUNTER(&htim3);//��ѯ����
	}
	HAL_TIM_Base_Stop(&htim3);
}
void DHT_GPIO_SET_OUTPUT(void)//���
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void DHT_GPIO_SET_INPUT(void)//����
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
uint8_t DHT_Read_Byte(void)//��ȡһλ�ź�
{
	uint8_t ReadData=0;//���8bit����
	uint8_t temp;			//��ʱ����źŵ�ƽ
	uint8_t retry=0;	//
	uint8_t i;
	for(i=0;i<8;i++)//һ�ζ�ȡ8λ
	{
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==0&&retry<100)//�ȴ�DHT11����ߵ�ƽ
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
		ReadData<<=1;//����
		ReadData |=temp;
		
	}
	return ReadData;
	
}
//DHT11��ȡ��ʪ�ȳ���
uint8_t DHT_Read(void)
{
	uint8_t retry=0;
	uint8_t i;
	
	DHT_GPIO_SET_OUTPUT();//IO����Ϊ���ģʽ
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);//���͵�ƽ18ms
	HAL_Delay(18);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);//���ߵ�ƽ20us
	Delay_us(20);
	
	DHT_GPIO_SET_INPUT();
	Delay_us(20);
	//�����⵽�͵�ƽ��˵��DHT11����Ӧ
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
		//һ�δ���40λ��һ�η���8λ������ȡ5��
		for(i=0;i<5;i++)
		{
			Data[i]=DHT_Read_Byte();
		}
		Delay_us(50);
	}
 
	//У��
	uint32_t sum=Data[0]+Data[1]+Data[2]+Data[3];
	 if((sum)==Data[4])    
		 return 1;  
	 else   
	 return 0;
}





 
 