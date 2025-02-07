/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2024.10.30
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2024 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#ifndef __DRV_UART_MCU_H__
#define	__DRV_UART_MCU_H__
#include "datatype.h"
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
//////////////////////////////////////////////////////////////////////////

#define MCU_UART_TX_MAX	64
#define MCU_UART_RX_MAX	64
typedef	struct mcuuart {
	uint8_t		TxBuf[MCU_UART_TX_MAX];			//接收buffer
	uint16_t	TxCnt;						//已经发送的数据数量
	uint16_t	TxTotal;					//要发送数据的数量
	uint8_t		RxBuf[MCU_UART_RX_MAX];			//接收buffer
	uint16_t	RxCnt;								//接收数据数量
}MCU_UART;

SYS_EXTERN void mcuUartInit(void);
//中断方式发送
SYS_EXTERN void mcuUartSend(uint8_t* txBuf, uint16_t txLen);
//阻塞方式发送
SYS_EXTERN void mcuUartsendLoop(uint8_t* txBuf, uint32_t txLen);
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
