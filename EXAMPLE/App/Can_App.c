#include "Can_App.h"
#include "usart.h"
#include "can.h"


uint8_t *can_rx_buff;
uint8_t res=0;

//********************************************************************************************
// * @brief       CAN接收初始化函数
// * @note	      本函数用于接收CAN总线上的数据
// * @param       无
// * @retval      无
//********************************************************************************************
void CAN_RX_App(void)
{
	res=can_receive_msg(0x125,can_rx_buff);
	if(!res)
	{
		my_printf("no can_rx \r\n");
	}
	else
	{
		my_printf("can_rx \r\n");
		for(uint8_t i=0;i<8;i++)
		{
			my_printf("%3x",can_rx_buff[i]);
		}
	}
}

//********************************************************************************************
// * @brief       CAN发送初始化函数
// * @note	      本函数用于发送CAN总线上的数据
// * @param       无
// * @retval      无
//********************************************************************************************
uint8_t canbuf[8]={0};


void CAN_TX_App(void)
{
    /* CAN初始化, 普通(0)/回环(1)模式, 波特率500Kbps */

	can_send_msg(0x125, canbuf, 8);    /* ID = 0x12, 发送8个字节 */
	canbuf[0]++;
}

