#ifndef __USART_H
#define __USART_H
#include "sys.h" 


#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void USART_Send_String(u8 *buf);

#endif


