/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2024.06.30
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2024 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#ifndef __SYSTEM_H__
#define	__SYSTEM_H__
#include "inc.h"

SYS_EXTERN void SysTick_Handler(void);

SYS_EXTERN void DelayMs(uint32_t ms);

//看门狗初始化
SYS_EXTERN void WatchDogInit(void);

//喂狗
SYS_EXTERN void WatchDogFeed(void);
enum {
	MAIN_SCR_METER = 0,
	MAIN_SCR_WAVE,
	MAIN_SCR_DATA,
};
enum {
	SLEEP_TIME_30S = 0,
	SLEEP_TIME_60S,
	SLEEP_TIME_300S,
	SLEEP_TIME_xxxS,
};
enum {
	LANGUAGE_CHINA = 0,
	LANGUAGE_ENGLISH,
	LANGUAGE_RUSSIA,
};
#define CONFIG_MAGIC_CODE  0x68623264 //hb2d
typedef struct setting{
	uint32_t magic;
	uint8_t chMainScrType; //主屏类型
	uint8_t chSleepTime;	//息屏时间
	uint8_t chBeepSet;		//声音开关
	uint8_t chLanguage;		//系统语言
} SYS_SET;
extern SYS_SET gSet;

typedef struct config{
	struct{
		uint32_t MainLcdEn		: 1; //主屏选择：DISABLE：disable，ENABLE：enable
		uint32_t ExternLcdEn	: 1; //外屏测试：DISABLE：disable，ENABLE：enable
		uint32_t CTpEn				: 1; //ctp电容屏测试：DISABLE：disable，ENABLE：enable
		uint32_t rsv					: 32-3;
	};
} SYS_CONFIG;
extern SYS_CONFIG gConfig;

extern void LoadSystemConfig(void);
extern void LoadSystemSetting(void);

#endif 

