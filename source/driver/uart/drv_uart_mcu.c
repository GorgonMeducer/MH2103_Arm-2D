/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2022.05.10
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2015 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#include "inc.h"

#define __this_uart		USART3
#define mcuUartIrq		USART3_IRQn

MCU_UART	mcuUart;

void mcuUartInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
//    Pb10 (USART3_TX),   Pb11 (USART3_RX)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	USART_DeInit(__this_uart);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(__this_uart, &USART_InitStructure);
	
/* Enable the UART Interrupt Channel */
	NVIC_InitStructure.NVIC_IRQChannel = mcuUartIrq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(__this_uart, ENABLE);
	mcuUart.RxCnt = 0;
	USART_ITConfig(__this_uart,USART_IT_TXE,DISABLE); //开始不能enalbe，否则TXE和TC中断会不停的来
	USART_ITConfig(__this_uart,USART_IT_RXNE,ENABLE);
  
}
static void UartTxRx(void);
void USART3_IRQHandler(void)
{
	UartTxRx();
}
void mcuUartsendLoop(uint8_t* txBuf, uint32_t txLen)
{
  while(txLen--)
  {
	while(!USART_GetFlagStatus(__this_uart,USART_FLAG_TC));
	USART_SendData(__this_uart, (uint8_t) *txBuf++);
	while((__this_uart->SR & USART_FLAG_TC) == RESET);
  }
}
void mcuUartSend(uint8_t* txBuf, uint16_t txLen)
{
	if(txLen > MCU_UART_TX_MAX)
		return;
	memcpy(mcuUart.TxBuf,txBuf,txLen);
	mcuUart.TxCnt = 0;
	mcuUart.TxTotal = txLen;
	//	mcuUart.RxCnt = 0; //reset receiver:设备有不确定时间的主动上报，有可能打断帧
	USART_ITConfig(__this_uart, USART_IT_TXE, ENABLE);
}
//接收完一帧数据
extern uint16_t mcuCopyData(uint8_t *pBuf,uint16_t len);
static void RxTimeout(void)
{
//	mcuCopyData(mcuUart.RxBuf,mcuUart.RxCnt);
	mcuUart.RxCnt = 0;	//1.接收正确，等下一帧。2.接收错误，超时等情况，恢复接收
} 
#define MCU_UART_TIMEOUT_MS	5
#define MCU_UART_USE 	TIMEOUT_2
static void UartTxRx(void)
{
 volatile uint8_t rbyte;
  uint8_t int_id;

	if(USART_GetITStatus(__this_uart,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(__this_uart,USART_IT_RXNE);
    	USART_ClearFlag(__this_uart, USART_FLAG_ORE);
		
		rbyte = __this_uart->DR & 0x00FF;
		if(mcuUart.RxCnt == 0)
		{
			if( 0xAA == rbyte) //有效的包
			{
				mcuUart.RxBuf[mcuUart.RxCnt++] = rbyte;
				TimeOutEnable(MCU_UART_USE,MCU_UART_TIMEOUT_MS,RxTimeout);	//帧超时，115200=0.868ms
			}
		}
		else if(mcuUart.RxCnt < MDRX_MAX)
		{
			mcuUart.RxBuf[mcuUart.RxCnt++] = rbyte;		//先放起来
			TimeOutReset(MCU_UART_USE); 		  //这里应该要重置计数器，帧超时时间要设置得更短，才能快速响应
		}
	}
	else if(USART_GetITStatus(__this_uart,USART_IT_TXE) != RESET)
	{
		USART_ClearITPendingBit(__this_uart,USART_IT_TXE);
		if(mcuUart.TxCnt < mcuUart.TxTotal)
			__this_uart->DR = mcuUart.TxBuf[mcuUart.TxCnt++];
		else
			USART_ITConfig(__this_uart,USART_IT_TXE,DISABLE);
	}	
	else if(USART_GetFlagStatus(__this_uart,USART_FLAG_ORE) != RESET)
	{
		volatile BYTE t = __this_uart->DR;
		USART_ClearFlag(__this_uart, USART_FLAG_ORE);
	}  
}
