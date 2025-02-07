/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2022.05.10
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2015 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#include "inc.h"

#define __this_uart		USART1
#define DebugUartIrq	USART1_IRQn
//调试串口和下载串口复用（调试的模式不支持下载，最后发布软件的时候关闭调试串口，支持下载）
MODBUS_UART	mbUart;

void Uart_debug_int(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//    PA11 (USART1_TX),   PA10 (USART1_RX)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	USART_DeInit(__this_uart);

	USART_InitStructure.USART_BaudRate = 460800;//230400;//115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(__this_uart, &USART_InitStructure);
	
/* Enable the UART Interrupt Channel */
	NVIC_InitStructure.NVIC_IRQChannel = DebugUartIrq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(__this_uart, ENABLE);
	mbUart.RxCnt = 0;
	mbUart.TxFlag = 0;
	USART_ITConfig(__this_uart,USART_IT_TXE,DISABLE); //开始不能enalbe，否则TXE和TC中断会不停的来
	USART_ITConfig(__this_uart,USART_IT_RXNE,ENABLE);
  
}
static void UartTxRx(void);
void USART1_IRQHandler(void)
{
	UartTxRx();
}
void Uart_debug_send(const uint8_t* txBuf, uint32_t txLen)
{
//	if(mbUart.TxFlag == 1) //已经在发送
//		return;
  while(txLen--)
  {
	  while(!USART_GetFlagStatus(__this_uart,USART_FLAG_TC));
	USART_SendData(__this_uart, (uint8_t) *txBuf++);
	  
//    while((__this_uart->SR & USART_FLAG_TC) == RESET);
//	__this_uart->DR = *txBuf++;
  }
  /* Loop until the end of transmission */
  while((__this_uart->SR & USART_FLAG_TC) == RESET);
}
void UartDebugSend(uint8_t* txBuf, uint16_t txLen)
{
	if(txLen > MDTX_MAX)
		return;
	memcpy(mbUart.TxBuf,txBuf,txLen);
	mbUart.TxCnt = 0;
	mbUart.TxTotal = txLen;
//	mbUart.TxFlag = 1;
	USART_ITConfig(__this_uart, USART_IT_TXE, ENABLE);
}
//接收完一帧数据
extern void CopyReceiveData(uint8_t *buf,uint16_t len);
static void RxTimeout(void)
{
	mbCopyDataToRtu(mbUart.RxBuf,mbUart.RxCnt);
	mbUart.RxCnt = 0;	//1.接收正确，等下一帧。2.接收错误，超时等情况，恢复接收
//	DebugPrintf("===========Modbus timeout========%d\r\n",mbUart.RxCnt);
} 
#define DEBUG_UART_TIMEOUT_MS	5
#define DEBUG_UART_USE 	TIMEOUT_1
static void UartTxRx(void)
{
 volatile uint8_t rbyte;
  uint8_t int_id;

  if(USART_GetITStatus(__this_uart,USART_IT_RXNE) != RESET)
  {
	  USART_ClearITPendingBit(__this_uart,USART_IT_RXNE);
		
		rbyte = __this_uart->DR & 0x00FF;
		if(mbUart.RxCnt == 0)
		{
			DebugPrintf("****Modbus timeout start*****\r\n");
			TimeOutEnable(DEBUG_UART_USE,DEBUG_UART_TIMEOUT_MS,RxTimeout);	//帧超时，115200=0.868ms
		}
		if(mbUart.RxCnt < MDRX_MAX)
		{
			mbUart.RxBuf[mbUart.RxCnt++] = rbyte;		//先放起来
			TimeOutReset(DEBUG_UART_USE); 		//这里要重置计数器，帧超时时间要设置得更短，才能快速响应
		}
  }
  else if(USART_GetITStatus(__this_uart,USART_IT_TXE) != RESET)
  {
		USART_ClearITPendingBit(__this_uart,USART_IT_TXE);
		if(mbUart.TxCnt < mbUart.TxTotal)
			__this_uart->DR = mbUart.TxBuf[mbUart.TxCnt++];
		else
		{
			mbUart.TxFlag = 0;
			USART_ITConfig(__this_uart,USART_IT_TXE,DISABLE);
		}
  }	
	else if(USART_GetFlagStatus(__this_uart,USART_FLAG_ORE) != RESET)
	{
		volatile BYTE t = __this_uart->DR;
		USART_ClearFlag(__this_uart, USART_FLAG_ORE);
	}    
}
