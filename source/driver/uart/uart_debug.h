/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2024.06.30
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2024 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#ifndef __UART_DEBUG_H__
#define	__UART_DEBUG_H__
#include "datatype.h"
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
//////////////////////////////////////////////////////////////////////////
#define MDRX_MAX	256+32
#define MDTX_MAX	256+32
typedef	struct mbuart {
	uint8_t		TxBuf[MDTX_MAX];			//接收buffer
	uint8_t		RxBuf[MDRX_MAX];			//接收buffer
	uint16_t	TxCnt;						//已经发送的数据数量
	uint16_t	TxTotal;					//要发送数据的数量
	uint16_t	RxCnt;								//接收数据数量
	uint8_t		TxFlag;
}MODBUS_UART;

SYS_EXTERN void Uart_debug_int(void);
//阻塞方式发送数据
SYS_EXTERN void Uart_debug_send(const uint8_t* txBuf, uint32_t txLen);
//中断方式发送数据
SYS_EXTERN void UartDebugSend(uint8_t* txBuf, uint16_t txLen);
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
