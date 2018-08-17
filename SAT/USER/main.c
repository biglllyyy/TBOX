#include "sys.h"



int main(void)
{		

	System_init();
    
    while(1)
    {
    	IWDG_Feed();  //ι��
    #if 1 	
		N720Init();
    
		N720_TCPInit();

    Led_task();
		
		UartDeal_task();
    #endif
#if 1		
		UartSendData_task();		

		/*������յ���CAN����*/
		CANrecv_task();

		/*CAN��������*/
		CANsend_task();
#endif

    if(printFlag == 1)
        {
        printFlag = 0;
        
        printf("gN720TCPInitStep:0x%x\r\n",gN720TCPInitStep);
    }
    }
}


