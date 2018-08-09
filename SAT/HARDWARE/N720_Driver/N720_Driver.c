#include "N720_Driver.h"


/*��ʼ��������Ҫ���͵�ATָ��*/
static u8  *ATCommandBuf[] =
			{
				"AT\r",
				"ATI\r",
				"AT+CCID\r",
				"AT+CPIN?\r",
				"AT+CSQ\r",
				"AT+CREG?\r",
				"AT+CGATT?\r",
                "AT$MYSYSINFO\r",
			};
//u8 testarc[]="at+qcnversion\r";


STRUCT_N720InitTIMFlag g_N720InitTIMFlag = {0};

/**********************************
*��������void N720PowerON(void)
*�������ܣ�N720ģ�鿪��
*����:None
*����ֵ:none
***********************************/
void N720PowerON(void)
{
    N720POWERKEY = N720_ON;
    g_N720InitTIMFlag.bits.bN720PowerONFlag = 1;
}

/**********************************
*��������void N720PowerOFF(void)
*�������ܣ�N720ģ��ػ�
*����:None
*����ֵ:none
***********************************/
void N720PowerOFF(void)
{
    N720POWERKEY = N720_ON;
    g_N720InitTIMFlag.bits.bN720PowerOFFFlag = 1;
}

/**********************************
*��������void N720PowerkeyReset(void)
*�������ܣ�N720ģ������������powerkey����
*����:None
*����ֵ:none
***********************************/
void N720PowerkeyReset(void)
{
    N720PowerOFF();
    g_N720InitTIMFlag.bits.bN720PowerkeyResetFlag = 1;    
}
/**********************************
*��������void N720IOInit(void)
*�������ܣ�N720ģ��IO��ʼ��
*����:None
*����ֵ:none
***********************************/
void N720IOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
 
    //modern powerkey io
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;	    		 // ???��????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	  				 //���������IO���ٶ�Ϊ2MHz
}

/**********************************
*��������void N720InitTimerHandler(void)
*�������ܣ�N720ģ���ʼ��ʱ����ʱ����
*����:None
*����ֵ:none
***********************************/
u8 gN720InitStep = 0xFF;
u8 CMDFailedCount = 0;//û���յ���ȷ��Ӧʱ����AT����Ĵ���
u8 ReconnectCount = 0;//���³�ʼ���Ĵ���
void N720InitTimerHandler(void)
{
    static u16 N720InitCount = 0;
    
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
            CMDFailedCount++;
            if(CMDFailedCount == ATFAILEDCOUNT)
            {
                CMDFailedCount = 0;
                N720PowerkeyReset();
                gN720InitStep = N720PrepareReset;
            }
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
            
            CMDFailedCount++;
            if(CMDFailedCount == 40)
            {
                CMDFailedCount = 0;
                N720PowerkeyReset();
                gN720InitStep = N720PrepareReset;
            }
        }
    }
    
    if((g_N720InitTIMFlag.bits.bN720SendATCREGCommandFlag == 0) && (gN720InitStep == N720SendATCREG))
    {
        N720InitCount++;
        if(N720InitCount == DELAY1S)
        {
            g_N720InitTIMFlag.bits.bN720SendATCREGCommandFlag = 1;
            N720InitCount = 0;

            CMDFailedCount++;
            if(CMDFailedCount == 40)
            {
                CMDFailedCount = 0;
                N720PowerkeyReset();
                gN720InitStep = N720PrepareReset;
            }
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




/**********************************
*��������void N720PowerTimerHandler(void)
*�������ܣ�N720ģ����ϵ�ʱ����ʱ����
*����:None
*����ֵ:none
***********************************/
void N720PowerTimerHandler(void)
{
    static u16 PowerONOFFCount = 0;
    if(g_N720InitTIMFlag.bits.bN720PowerONFlag == 1)
    {
        PowerONOFFCount++;
        if(PowerONOFFCount == DELAY_POWERON)
        {
            PowerONOFFCount = 0;
            g_N720InitTIMFlag.bits.bN720PowerONFlag = 0;
            N720POWERKEY = N720_OFF;
            gN720InitStep = N720StartSend;
        }
    }

    if(g_N720InitTIMFlag.bits.bN720PowerOFFFlag == 1)
    {
        PowerONOFFCount++;
        if(PowerONOFFCount == DELAY_POWEROFF)
        {
            PowerONOFFCount = 0;
            g_N720InitTIMFlag.bits.bN720PowerOFFFlag = 0;
            N720POWERKEY = N720_OFF;                
        }
    }

    if((g_N720InitTIMFlag.bits.bN720PowerkeyResetFlag == 1) 
        &&(g_N720InitTIMFlag.bits.bN720PowerOFFFlag == 0))
    {
        PowerONOFFCount++;
        if(PowerONOFFCount == DELAY_POWERRESET)
        {
            PowerONOFFCount = 0;
            g_N720InitTIMFlag.bits.bN720PowerkeyResetFlag = 0;
            N720PowerON();
        }
    }

}



/**********************************
 *��������void N720Init(void)
 *�������ܣ�N720ģ���ϵ��ʼ������
 *����:None
 *����ֵ:none
***********************************/
void N720Init(void)
{
	#if 1
	if(g_N720InitTIMFlag.bits.bN720StartInitFlag == 1)
	{
		//printf("%s len:%d,%d",ATCommandBuf[0],sizeof(ATCommandBuf[0]),sizeof(ATCommandBuf[0][0]));
		//USART2_Send_String(ATCommandBuf[COMMAND_AT]);
		g_N720InitTIMFlag.bits.bN720SendATCommandFlag = 1;
		g_N720InitTIMFlag.bits.bN720StartInitFlag = 0;
	}

	if(g_N720InitTIMFlag.bits.bN720SendATCommandFlag == 1)
	{
        printf("send AT\r\n");
		USART2_Send_String(ATCommandBuf[COMMAND_AT]);
		g_N720InitTIMFlag.bits.bN720SendATCommandFlag = 0;
	}

	if(g_N720InitTIMFlag.bits.bN720SendATICommandFlag == 1)
	{
        printf("send ATI\r\n");
		USART2_Send_String(ATCommandBuf[COMMAND_ATI]);
		g_N720InitTIMFlag.bits.bN720SendATICommandFlag = 0;
	}

	if(g_N720InitTIMFlag.bits.bN720SendATCCIDCommandFlag == 1)
	{
        printf("send ATCCID\r\n");
		USART2_Send_String(ATCommandBuf[COMMAND_ATCCID]);
		g_N720InitTIMFlag.bits.bN720SendATCCIDCommandFlag = 0;
	}

	if(g_N720InitTIMFlag.bits.bN720SendATCPINCommandFlag == 1)
	{
        printf("send ATCPIN\r\n");
		USART2_Send_String(ATCommandBuf[COMMAND_ATCPIN]);
		g_N720InitTIMFlag.bits.bN720SendATCPINCommandFlag = 0;
	}
	
	if(g_N720InitTIMFlag.bits.bN720SendATCSQCommandFlag == 1)
	{
        printf("send ATCSQ\r\n");
		USART2_Send_String(ATCommandBuf[COMMAND_ATCSQ]);
        //delay_ms(10);
        
		//USART2_Send_String(testarc);
		g_N720InitTIMFlag.bits.bN720SendATCSQCommandFlag = 0;
	}
	
	if(g_N720InitTIMFlag.bits.bN720SendATCREGCommandFlag == 1)
	{
        printf("send ATCREG\r\n");
		USART2_Send_String(ATCommandBuf[COMMAND_ATCREG]);
		g_N720InitTIMFlag.bits.bN720SendATCREGCommandFlag = 0;
	}

	if(g_N720InitTIMFlag.bits.bN720SendATCGATTCommandFlag == 1)
	{
        printf("send ATCGATT\r\n");
		USART2_Send_String(ATCommandBuf[COMMAND_ATCGATT]);
		g_N720InitTIMFlag.bits.bN720SendATCGATTCommandFlag = 0;
	}
	
	if(g_N720InitTIMFlag.bits.bN720SendATMYSYSINFOCommandFlag == 1)
	{
        printf("send ATSYSINFO\r\n");
		USART2_Send_String(ATCommandBuf[COMMAND_ATMYSYSINFO]);
		g_N720InitTIMFlag.bits.bN720SendATMYSYSINFOCommandFlag = 0;
	}
	#endif
}

