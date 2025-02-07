/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.5.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#ifndef __HAL_BUZZER_H__
#define	__HAL_BUZZER_H__
#include "inc.h"

/*
  <-----------repeat------->
	 set     stop
    _______         _______
   |	   |	   |	   |
___|       |_______|	   |_______
*/
typedef	struct tagbeep {
	uint16_t	Set;			//短脉冲的个数
	uint16_t	Stop;			//停止时间的脉冲个数
	uint16_t	Setshadow;		//短脉冲的个数shadow
	uint16_t	Stopshadow;		//停止时间的脉冲个数shadow
	uint16_t	Repeat;	//重复次数
	uint16_t	run;		//0:stop,1:running
//	uint32_t	mode;		//mode
}BEEPSET;
//bit0-7:mode
#define BEEP_MODE_NORMAL	0x00000001		//普通短促提示音
#define BEEP_MODE_ALARM		0x00000002		//急促报警提示音
#define BEEP_MODE_MASK		0x000000FF
//bit8-15: led
//bit16-23:motor

//buzzer init
extern void BuzzerInit(void);
//供异常提示使用，一直叫
extern void BuzzerAlwaysOn(void);

//timer call,check buzzer time
extern void BeepRunCheck(void);
//beep setting
//extern void Beep(uint32_t mode,uint32_t repeatnum);
extern void Beep(uint16_t set_10ms, uint16_t stop_10ms,uint16_t repeat);
#define BeepOnce()	Beep(20,20,1);
#define BeepAlarm()	Beep(10,10,5);


#endif 

