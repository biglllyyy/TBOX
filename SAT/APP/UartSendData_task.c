/*
*�������ݵ�˼·��������ͬһʱ����ܽ��յ����ID���͹�����CAN���ݣ����ǲ���ͬʱ����
                �����Ҫ�ڷ�����һ֡���ݺ��յ�4Gģ��ظ��ķ��ͳɹ������ٽ��ŷ�����һ֡
*
*/
#include "UartSendData_task.h"

#define USARTSENDBUF 10
#define USARTRECVCANDATA 10

UARTSENDDATA gUartSendData[USARTSENDBUF] = {0};


u8 DataSendBuf[SEND_DATA_LEN] = {0};
#if 1

/**********************************
 *��������static void SwitchCANData(u8* src ,u8* SendVal,u16 srclen)
 *�������ܣ���16������ת����ASCIIֵ
 *����:(1)src:ת�����ֵ��ŵĵ�ַ
        (2)SendVal:׼��ת����ֵ
        (3)srclen:��Ҫת�������ݳ���
 *����ֵ:none
***********************************/
static void SwitchCANData(u8* src ,u8* SendVal,u16 srclen)
{
	u16 i;
	for(i = 0; i < srclen; i++)
	{
		src[2*i] = SendVal[i] >> 4;
		src[2*i+1] = SendVal[i] & 0xf;
	}
	
	for(i = 0; i< srclen * 2; i++)
	{
		if (src[i] <= 0x09)
		{
			src[i] = src[i] + '0';
		}
		else if((src[i] >= 0x0a) && (src[i] <= 0x0f))
		{
			src[i] = src[i] + 0x37;
		}
		else
		{
			src[i] = 0xFF;
		}
	}   

}
#else
static void SwitchCANData(u16* src ,u8* SendVal,u16 srclen)
{
	u16 i;
	u8 Tempbuf[40] = {0};
	
	for(i = 0; i < srclen; i++)
	{
		Tempbuf[2*i] = SendVal[i] >> 4;
		Tempbuf[2*i+1] = SendVal[i] & 0xf;
	}
#if 0
	printf("tempbuf:");
	for(i = 0;i<40;i++)
	{
		printf("%02x ",Tempbuf[i]);
	}
	printf("\r\n");
	
	printf("src:");
#endif
	for(i = 0; i< srclen ; i++)
	{
		if (Tempbuf[2*i+1] <= 0x09)
		{
			src[i] = Tempbuf[2*i+1] + '0';
		}
		else if((Tempbuf[2*i+1] >= 0x0a) && (Tempbuf[2*i+1] <= 0x0f))
		{
			src[i] = Tempbuf[2*i+1] + 0x37;
		}
		else
		{
			src[i] = 0xFF;
		}

		
		if (Tempbuf[2*i] <= 0x09)
		{
			src[i] |= ((u16)(Tempbuf[2*i] + '0')) << 8;
		}
		else if((Tempbuf[2*i] >= 0x0a) && (Tempbuf[2*i] <= 0x0f))
		{
			src[i] |= ((u16)(Tempbuf[2*i] + 0x37)) << 8;
		}
		else
		{
			src[i] |= 0xFF00;
		}
		//printf("|%02x,%02x|",Tempbuf[i],Tempbuf[i+1]);
		//src[i] = (((u16)Tempbuf[i+1]) << 8 | Tempbuf[i]);
		//printf("%0x ",src[i]);
	}   
	//printf("\r\n");
}

#endif
/**********************************
 *��������void ATCommSendCAN(void)
 *�������ܣ�������յ���CANԭʼ���ݲ�����ת��
 *����:None
 *����ֵ:none
***********************************/
void ATCommSendCAN(void)
{
	static u8 CanRxCount = 0;

    if(gCanRxRawDataBuf[CanRxCount].NewDataFlag == 1)
    {

		SwitchCANData(DataSendBuf + CANDATA_SEND_LEN * CanRxCount,gCanRxRawDataBuf[CanRxCount].Buf, 
							CANDATA_SEND_LEN);
        
		gCanRxRawDataBuf[CanRxCount].NewDataFlag = 0;
	}
    
}

/**********************************
 *��������void UartSendData_task(void)
 *�������ܣ�����2�����������񣺴������ǰѽ��յ���CAN����ת����ASCIIֵ��Ȼ����ͨ��ATָ��͸�N720
 *����:None
 *����ֵ:none
***********************************/
void UartSendData_task(void)
{
//	u16 i = 0,j = 0;
//    static u8 UartSendDataCount = 0;
	//if(g_TIMFlag.bits.ATUartSendFlag == 1)
	{
		ATCommSendCAN();
		//g_TIMFlag.bits.ATUartSendFlag = 0;
	}
	
	if((DataSendFlag == 1) && (g_N720TCPInitFlag.bits.bN720SendACKFinishFlag == 1))
	{
	    DataSendFlag = 0;
        gN720TCPInitStep = N720SendTCPSEND;
    }
    
    if(g_N720TCPInitFlag.bits.bN720SendATStartSendCommandFlag == 1)
    {
        g_N720TCPInitFlag.bits.bN720SendATStartSendCommandFlag = 0;
        USART2_Send_CANData(DataSendBuf);
        gN720TCPInitStep = N720TCPInitFinish;
    }
    
}

