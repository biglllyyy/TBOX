#ifndef __USART_H
#define __USART_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define USART_REC_BUF			64	  	
typedef struct 
{
	u8 USART_RX_BUF[USART_REC_LEN];
	u8 USART_RX_Flag;
}UARTData;

typedef union
{
	u16 Byte;
	struct 
	{
		u8 bN720RecvATInfoFlag : 1;
		u8 bN720RecvATIInfoFlag : 1;
		u8 bN720RecvATCCIDInfoFlag : 1;
		u8 bN720RecvATCPINInfoFlag : 1;
		u8 bN720RecvATCSQInfoFlag : 1;
		u8 bN720RecvATCREGInfoFlag : 1;
		u8 bN720RecvATCGATTInfoFlag : 1;
		u8 bN720RecvATMYSYSINFOInfoFlag : 1;
		u8 bN720InitFinish : 1;
		u8 bnull : 7;
	}bits;
}STRUCT_InitRECVFlag;
extern STRUCT_InitRECVFlag g_N720InitRecvFlag;

typedef union
{
	u16 Byte;
	struct 
	{
		u8 bN720RecvATInfoFlag : 1;
		u8 bN720RecvATIInfoFlag : 1;
		u8 bN720RecvATCCIDInfoFlag : 1;
		u8 bN720RecvATCPINInfoFlag : 1;
		u8 bN720RecvATCSQInfoFlag : 1;
		u8 bN720RecvATCREGInfoFlag : 1;
		u8 bN720RecvATCGATTInfoFlag : 1;
		u8 bN720RecvATMYSYSINFOInfoFlag : 1;
		u8 bN720InitFinish : 1;
		u8 bnull : 7;
	}bits;
}STRUCT_TCPInitFlag;
extern STRUCT_InitRECVFlag g_N720TCPInitFlag;

typedef struct
{
	u8 SoftwareVerBuf[4];	//����汾
	u8 CCIDBuf[20];
	u8 CPINBuf[10];
	u8 CSQBuf[6];
	u8 CREGBuf[4];
	u8 CGATTBuf[1];
	u8 MYSYSINFOBuf[4];
}STRUCT_N720_INFO;
extern STRUCT_N720_INFO gN720Info;


extern UARTData gUARTData[USART_REC_BUF]; 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void USART_Send_String(u8 *buf);

#endif


