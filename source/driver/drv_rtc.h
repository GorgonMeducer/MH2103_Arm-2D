/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.5.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#ifndef __HAL_RTC_H__
#define	__HAL_RTC_H__
#include "inc.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
////////////////////////////////////////////////////////////
typedef struct tagDateTime{
	uint16_t	year;		// current year(2000-2100)
	uint8_t		month;		// month (1-12)
	uint8_t		day;		// day of the month(1-31)
	uint8_t	 	hour;		// hours(0-23)
	uint8_t	 	minute;	// minutes(0-59)
	uint8_t	 	second;	// seconds(0-59)
	uint8_t	 	week;		// week(1-7,7=sunday)
}DATETIME; 
//设置实时时钟
SYS_EXTERN void GetRtc(DATETIME *dt);
//获取实时时钟
SYS_EXTERN void SetRtc(DATETIME *dt);
//初始化实时时钟
SYS_EXTERN void RtcInit(void);

//将时间转换距2000.1.1 0:00:00的秒数
//返回转换结果的数值
SYS_EXTERN UINT32 DataTimeToSecond(DATETIME *pDt);
//将秒数转换为时间
SYS_EXTERN void SecondToDataTime(DATETIME *pDt,UINT32 second);

//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif 

