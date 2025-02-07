/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.5.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#ifndef __HAL_KEY_H__
#define	__HAL_KEY_H__
#include "inc.h"
#define VK_NULL			0x0000			//null key

#define VK_LEFT			0x0001			//left key
#define VK_RIGHT		0x0002			//right key
#define VK_UP			0x0003			//up key
#define VK_DOWN			0x0004			//down key
#define VK_ENTER		0x0005			//enter key
#define VK_POWER		0x0006			//power key
#define VK_ESC			0x0007			//esc key
#define VK_KEY_ATK		0x0008			//'*' key
#define VK_KEY_SHARP	0x0009			//# key
#define VK_KEY_MENU		0x000a			//menu key
#define VK_PAGEDOWN		0x000b			//pagedown key
#define VK_PAGEUP		0x000c			//pageup key

//下面0-9和.按键的定义顺序不能改变,并且必须连续
#define VK_KEY0			0x0080			//digital 0 key
#define VK_KEY1			0x0081			//digital 1 key
#define VK_KEY2			0x0082			//digital 2 key
#define VK_KEY3			0x0083			//digital 3 key
#define VK_KEY4			0x0084			//digital 4 key
#define VK_KEY5			0x0085			//digital 5 key
#define VK_KEY6			0x0086			//digital 6 key
#define VK_KEY7			0x0087			//digital 7 key
#define VK_KEY8			0x0088			//digital 8 key
#define VK_KEY9			0x0089			//digital 9 key
#define VK_DOT			0x008a			//. key
#define VK_CLEAR		0x008b			//clear key

//按键消息的lParam: 高16位表示按键按下的时间(10ms为单位),bit15表示前一次按键的状态
//前一次按键的状态,1:up,0:down
#define KS_KEYSTATUS				  0x00008000
//以下宏用户使用
#define PREKEY_ISUP(lParam)		(((lParam) & KS_KEYSTATUS) ? TRUE : FALSE) //判断前一次按键的状态TRUE 弹起
#define PREKEY_ISDOWN(lParam)	(((lParam) & KS_KEYSTATUS) ? FALSE : TRUE)//判断前一次按键的状态:TRUE 按下
#define GETKEY_TIME(lParam)		((((lParam) >> 16) & 0x0000FFFF)*10)	//获取按键时长，单位: ms
#define GETKEY_VALUE(lParam)	((lParam) & 0x00007FFF)			//获取键值

//以下宏系统使用	
#define SETKEY_UP(lParam)			((lParam) | KS_KEYSTATUS)
#define SETKEY_DOWN(lParam)			((lParam) & ~KS_KEYSTATUS)
#define SETKEY_TIME(lParam,time)	((lParam) | ((time) << 16))
#define SETKEY_VALUE(lParam,key)	((lParam) | (key))
#define SETKEY_ALL(lParam,key,flag)	((lParam) | (key) | flag)
#define KEYTIME_MAX					0xFFFF

//判断是否是弹起的确认按键
#define KeyEnterIsUp(lParam)  ((GETKEY_VALUE(lParam) == VK_ENTER) && PREKEY_ISDOWN(lParam))      

//key init
extern void KeyInit(void);

//key scan,10ms period
extern void KeyScan(void);
#endif 

