#include "sys.h"
#include "usart.h"	  

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
	  
UARTData gUARTData[USART_REC_BUF] = {{0},0};	  
  
void uart_init(u32 bound)
{
  //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
     
    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

static u8 QueListFlag                   = 0;
static u8 CORERcvState                  = 0;
u8 CORERcvPnt                    = 0;

u8 RcvHeadCount  = 0;//���յ�������ͷ����
u8 RcvTailCount  = 0;//�յ���������β����

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
	    switch(CORERcvState)
        {
        	case 0:
        		if(Res == 0xE1)
                {
                    if(gUARTData[QueListFlag].USART_RX_Flag == 1)
                    {
                        QueListFlag++;
                        gUARTData[QueListFlag].USART_RX_Flag = 0;
                    }
        			CORERcvState = 1;
        			CORERcvPnt = 0;
                    gUARTData[QueListFlag].USART_RX_BUF[CORERcvPnt++] = Res;
        		}
        		break;
     		case 1:
                gUARTData[QueListFlag].USART_RX_BUF[CORERcvPnt++] = Res;
            	CORERcvState = 2;
            	break;
            case 2:
    			if(CORERcvPnt < USART_REC_LEN)
                {
                    gUARTData[QueListFlag].USART_RX_BUF[CORERcvPnt++] = Res;
                    CORERcvState = 3;
    			}
    			else
                {
                    CORERcvState = 0;
                    CORERcvPnt = 0;
    			}
    			break;
            case 3:
    			if(CORERcvPnt < USART_REC_LEN)
                {
                    gUARTData[QueListFlag].USART_RX_BUF[CORERcvPnt++] = Res;
    			}
    			else
                {
    				CORERcvState = 0;
                    CORERcvPnt = 0;
    			}
    			if(CORERcvPnt == (gUARTData[QueListFlag].USART_RX_BUF[2] + 5))
                {
    				CORERcvState = 4;
    			}
    			break;
            case 4:
                if(Res == 0xFE)
                {
                    gUARTData[QueListFlag].USART_RX_BUF[CORERcvPnt] = 0xfe;
                    gUARTData[QueListFlag].USART_RX_Flag = 1;

                    QueListFlag ++;
                    if(QueListFlag == USART_REC_BUF)
          	 		{
        				QueListFlag = 0;
        			}
                }
                CORERcvState = 0;
                CORERcvPnt = 0;
                break;
            default:
                break;
        }   		 
     } 
} 
#endif	

