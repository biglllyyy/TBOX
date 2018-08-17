#include "timer.h"



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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM6, ENABLE);	//ʹ��TIMx					 

}

static u16 printCount = 0;
u8 printFlag = 0;
void TIM6_IRQHandler(void)	 //TIM2�ж�
{

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)	//���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx�����жϱ�־

        printCount++;
		if(printCount == 5000)
        {
            printFlag = 1;
            printCount = 0;
        }      
		/*can count*/
        CANSendTimerHandler();
		/*LED count*/
        LedTimerHandler();

		/*N720*/
        N720InitTimerHandler();
        N720TCPInitTimerHandler();

        N720PowerTimerHandler();

	}
}

/**********************************
 *��������void TIM_INIT(void)
 *�������ܣ���ʱ����ʼ��
 *����:None
 *����ֵ:none
***********************************/
void TIM_INIT(void)
{
	TIM6_Init(999, 71);// 1kHz	
}
