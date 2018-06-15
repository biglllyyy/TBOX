#include "timer.h"

#define DELAY10S 10000
#define DELAY1S 1000
/*global value*/
STRUCT_TIMFLAG g_TIMFlag = {0};
STRUCT_N720InitTIMFlag g_N720InitTIMFlag = {0};

/**
  * @brief  ��ʼ��TIM6��ʱ����
  * @param  u16 arr���Զ���װ��ֵ
  * @param  U16 psc:Ԥ��Ƶϵ��
  *
  * @retval None
  */

static void TIM6_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��2�ĳ�ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�жϣ���������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM6, ENABLE);	//ʹ��TIMx					 

}



static u16 LEDCount = 0;
static u8 CANSendCount = 0;
static u16 N720InitCount = 0;
u8 gN720InitStep = 0;

void TIM6_IRQHandler(void)	 //TIM2�ж�
{

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)	//���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx�����жϱ�־

		
		/*can count*/
		if(g_TIMFlag.bits.CANFlag == 0)
		{
			CANSendCount++;
			if(CANSendCount == 99)
			{
				g_TIMFlag.bits.CANFlag = 1;
				g_TIMFlag.bits.ATUartSendFlag = 1;
				CANSendCount = 0;
			}
		}
		
		/*LED count*/
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

		/*N720 INIT*/
		if((g_N720InitTIMFlag.bits.bN720StartInitFlag == 0) && (gN720InitStep == N720StartSend))
		{
			N720InitCount++;
			if(N720InitCount == DELAY10S)
			{
				g_N720InitTIMFlag.bits.bN720StartInitFlag = 1;
				gN720InitStep = N720SendAT;
				N720InitCount = 0;
			}
		}
		
		if((g_N720InitTIMFlag.bits.bN720SendATCommandFlag == 0) && (gN720InitStep == N720SendAT))
		{
			N720InitCount++;
			if(N720InitCount == DELAY1S)
			{
				g_N720InitTIMFlag.bits.bN720SendATCommandFlag = 1;
				N720InitCount = 0;
			}
		}
		
		if((g_N720InitTIMFlag.bits.bN720SendATICommandFlag == 0) && (gN720InitStep == N720SendATI))
		{
			N720InitCount++;
			if(N720InitCount == DELAY1S)
			{
				g_N720InitTIMFlag.bits.bN720SendATICommandFlag = 1;
				N720InitCount = 0;
			}
		}

		if((g_N720InitTIMFlag.bits.bN720SendATCCIDCommandFlag == 0) && (gN720InitStep == N720SendATCCID))
		{
			N720InitCount++;
			if(N720InitCount == DELAY1S)
			{
				g_N720InitTIMFlag.bits.bN720SendATCCIDCommandFlag = 1;
				N720InitCount = 0;
			}
		}

		if((g_N720InitTIMFlag.bits.bN720SendATCPINCommandFlag == 0) && (gN720InitStep == N720SendATCPIN))
		{
			N720InitCount++;
			if(N720InitCount == DELAY1S)
			{
				g_N720InitTIMFlag.bits.bN720SendATCPINCommandFlag = 1;
				N720InitCount = 0;
			}
		}
        
		if((g_N720InitTIMFlag.bits.bN720SendATCSQCommandFlag == 0) && (gN720InitStep == N720SendATCSQ))
		{
			N720InitCount++;
			if(N720InitCount == DELAY1S)
			{
				g_N720InitTIMFlag.bits.bN720SendATCSQCommandFlag = 1;
				N720InitCount = 0;
			}
		}
        
        if((g_N720InitTIMFlag.bits.bN720SendATCREGCommandFlag == 0) && (gN720InitStep == N720SendATCREG))
		{
			N720InitCount++;
			if(N720InitCount == DELAY1S)
			{
				g_N720InitTIMFlag.bits.bN720SendATCREGCommandFlag = 1;
				N720InitCount = 0;
			}
		}

        
        if((g_N720InitTIMFlag.bits.bN720SendATCGATTCommandFlag == 0) && (gN720InitStep == N720SendATCGATT))
		{
			N720InitCount++;
			if(N720InitCount == DELAY1S)
			{
				g_N720InitTIMFlag.bits.bN720SendATCGATTCommandFlag = 1;
				N720InitCount = 0;
			}
		}

        if((g_N720InitTIMFlag.bits.bN720SendATMYSYSINFOCommandFlag == 0) && (gN720InitStep == N720SendATMYSYSINFO))
		{
			N720InitCount++;
			if(N720InitCount == DELAY1S)
			{
				g_N720InitTIMFlag.bits.bN720SendATMYSYSINFOCommandFlag = 1;
				N720InitCount = 0;
			}
		}
        
	}
}


void TIM_INIT(void)
{

	TIM6_Init(999, 71);// 1kHz	
}
