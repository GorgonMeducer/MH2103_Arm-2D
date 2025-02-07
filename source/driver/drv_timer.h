/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2024.06.30
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2024 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#ifndef __DRV_TIMER_H__
#define	__DRV_TIMER_H__
#include "inc.h"

extern void Timer2Init(void);
extern void Timer3Init(void);

typedef void (* fpTimeout)(void);		//超时回调函数

extern void TimeoutTimerEnable(uint32_t ms, fpTimeout ftimeout);
extern void TimeOutTimerReset(uint32_t ms);

typedef enum {
	TIMEOUT_1,
	TIMEOUT_2,
	TIMEOUT_3,
	TIMEOUT_4,
	TIMEOUT_5,
	TIMEOUT_MAX, //最大16个
}TIMEOUT_LIST;
typedef	struct timeout {
	fpTimeout		pfCallback[TIMEOUT_MAX];
	uint16_t		uTime[TIMEOUT_MAX];
	uint16_t		uTimeRemaind[TIMEOUT_MAX];
	uint16_t		uTimerEn;
}TIMEOUT_TIMER;

//使能超时定时器
//nTimer：哪一个超时定时器
//ms： 超时时间，单位ms
//pfTimeout：超时回调函数
extern void TimeOutEnable(TIMEOUT_LIST nTimer, uint16_t ms,fpTimeout pfTimeout);

//超时定时器初始化
extern void TimeOutInit(void);

//复位超时定时器
extern void TimeOutReset(TIMEOUT_LIST nTimer);
	
//关闭超时定时器
extern void TimeOutDisable(TIMEOUT_LIST nTimer);
	
//超时定时器检查
extern void TimeOutCheck( uint32_t ms);
#endif 

