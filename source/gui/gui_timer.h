#ifndef __GUI_TIMER_H__
#define __GUI_TIMER_H__
#include "inc.h"      
	  
#define TIMER_MAX	5	
#define GUI_TIMER_INTERVAL   	5     // GUI定时器周期(ms)  1000/25=40hz

typedef enum {
	TIMER_ID1 = 0, 
	TIMER_ID2 = 1, 
	TIMER_ID3 = 2, 
	TIMER_ID4 = 3, 
	TIMER_ID5 = 4,
	TIMER_IDMAX = TIMER_MAX,
}TIMERID;

typedef enum{
	TIMER_ONESHOT 	= 0,
	TIMER_REPEAT 	= 1,
}TIMERMODE;

/**
 * 函数：void guiInitTimer(void);
 * 功能：初始化gui定时器
 * 参数：无
 * 返回：无
 */
void guiInitTimer(void);

/**
 * 函数：void guiSetTimer( TIMERID id, uint32_t proid, TIMERMODE reload);
 * 功能：创建一个gui定时器
 * 参数：
 * @id： 定时器ID
* @proid：定时周期(1-65536),uini:ms
 * @relod: 计时模式，TIMER_ONESHOT一次性，TIMER_REPEAT周期性
 * 返回：无
 */
#define GUI_TIMER_10MS		10
#define GUI_TIMER_100MS		100
#define GUI_TIMER_1000MS	1000
#define GUI_TIMER_MS_MAX	65535
void guiSetTimer( TIMERID id, uint32_t proid, TIMERMODE reload);

/**
 * 函数：void guiStartTimer( TIMERID id );
 * 功能：启动一个gui定时器
 * 参数：
 * @id： 定时器ID
 * 返回：无
 */
void guiStartTimer( TIMERID id );

/**
 * 函数：void guiStopTimer( TIMERID id );
 * 功能：停止一个gui定时器
 * 参数：
 * @id： 定时器ID
 * 返回：无
 */
void guiStopTimer( TIMERID id );

/**
 * 函数：void guiStopTimer( TIMERID id );
 * 功能：复位一个gui定时器
 * 参数：
 * @id： 定时器ID
 * 返回：无
 */
void guiResetTimer ( TIMERID id );

/**
 * 函数：void guiStopTimer( TIMERID id );
 * 功能：检查定时器是否超时,由硬件定时器中断调用.
 * 参数：
 * @ms： 已经逝去的时间:ms(单位毫秒)
 * 返回：无
 */
void guiCheckTimer( uint32_t ms );

/**
 * 函数：void guiTimerStart(void);
 * 功能：定时器系统启动
 * 参数：无
 * 返回：无
 */
void guiTimerStart(void);

/**
 * 函数：void guiTimerStop(void);
 * 功能：定时器系统停止
 * 参数：无
 * 返回：无
 */
void guiTimerStop(void);

#endif


