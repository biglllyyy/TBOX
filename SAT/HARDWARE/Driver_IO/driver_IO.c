#include "driver_IO.h"

void IO_init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;	
 
    //CANSELECT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;	    		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //���������IO���ٶ�Ϊ2MHz
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);		

    //MODEMCTRL    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;	    		 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //���������IO���ٶ�Ϊ2MHz
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);		
}


