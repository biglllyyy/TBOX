#include "sys.h"



int main(void)
{		

	System_init();

    while(1)
    {
    	IWDG_Feed();  //ι��
    	
		Led_task();
		
		UartSendData_task();		

		/*������յ���CAN����*/
		CANrecv_task();

		/*CAN��������*/
		CANsend_task();
    }
}


