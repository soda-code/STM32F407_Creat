#include "Can_App.h"
#include "usart.h"
#include "can.h"
#include "Light_App.h"


uint8_t can_rx_buff[8]={0};

//********************************************************************************************
// * @brief       CAN接收初始化函数
// * @note	      本函数用于接收CAN总线上的数据
// * @param       无
// * @retval      无
//********************************************************************************************
void CAN_RX_App(void)
{
	uint8_t res=0;

	res=can_receive_msg(0x125,can_rx_buff);
	if(res)
	{
		my_printf("CAN RX: %02X %02X %02X %02X %02X %02X %02X %02X\r\n",
				  can_rx_buff[0], can_rx_buff[1], can_rx_buff[2], can_rx_buff[3],
				  can_rx_buff[4], can_rx_buff[5], can_rx_buff[6], can_rx_buff[7]);

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
	uint16_t light_data=0;
    /* CAN初始化, 普通(0)/回环(1)模式, 波特率500Kbps */
	light_data=Get_Light_Data();

	canbuf[0] = light_data & 0xFF;         /* 低8位 */
	canbuf[1] = (light_data >> 8) & 0xFF;  /* 高8位 */
	can_send_msg(0x125, canbuf, 8);    /* ID = 0x12, 发送8个字节 */
}

