#include "led.h"

   
//LED IO��ʼ��
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOF,GPIO_Pin_6);						 //PB.5 �����

}

/**********************************
 *��������void Led_task(void)
 *�������ܣ�����������
 *����:None
 *����ֵ:none
***********************************/
void Led_task(void)
{
	if(g_TIMFlag.bits.LedFlag == 1)
	{
		LED0 = LED_ON;
	}
	else
	{
		LED0 = LED_OFF;
	}
}

void LedTimerHandler(void)
{
    static u16 LEDCount = 0;
    
    LEDCount++;
    if(LEDCount == 499)
    {
        g_TIMFlag.bits.LedFlag = 1;
    }
    else if(LEDCount == 999)
    {
        g_TIMFlag.bits.LedFlag = 0;
        LEDCount = 0;
    }
}
