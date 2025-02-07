#ifndef __GUI_MSG_H__
#define __GUI_MSG_H__
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
 
#ifdef __cplusplus
extern "C" {
#endif

#include "inc.h"
//WPARAM 参数保存消息类型
//LPARAM 参数保存消息值

#define		MSG_NULL		0x00000000

#define		MSG_KEYDOWN		0x00000001			//按键按下
#define		MSG_KEYUP		0x00000002			//按键弹起 
#define		MSG_SECOND		0x00000003			//秒中断
#define		MSG_TIMER		0x00000004			//定时器中断
#define		MSG_CREATE		0x00000005			//初始化窗口消息
#define		MSG_DESTROY			0x00000006			//销毁窗口消息
#define		MSG_PAINT			0x00000007			//重画窗口消息
#define		MSG_MOUSE_DOWN		0x00000008			//鼠标（手指）点击按下
#define		MSG_MOUSE_UP		0x00000009			//鼠标（手指）点击弹起 
#define		MSG_MOUSE_MOVE		0x0000000a			//鼠标（手指）移动
#define		MSG_GESTURE			0x0000000b			//手势

#define		MSG_COMM_MCU		0x00000200			//MCU解码完成
	#define MCU_COMM_GET_CMD(lParam)		((lParam) & 0x0000FFFF)			//获取命令类型
	#define MCU_COMM_SET_CMD(lParam,key)	((lParam) | (key))
#define		MSG_DRAWITEM	0x00000100			//列表条目重画

#define		MSG_DUMMY		0x0000FFFF			//虚假信息,占位


#define MAX_MSG			16	//最大能容纳消息数量
typedef	struct tagMsg {
	WPARAM	type;				//消息类型
	LPARAM	value;				//消息值
}MSGTYPE; 

typedef	struct tagmsgque {
	MSGTYPE		Msg_Queue[MAX_MSG];			//消息队列
	uint16_t	In;								//消息输入
	uint16_t	Out;							//消息输出
}MSGQUE;

/***************************************************************************
函数名称:        guiMessageQueueInit
功能描述：初始化消息队列
输入参数:	无
输出参数:	无
使用注意:
***************************************************************************/ 
SYS_EXTERN	void guiMessageQueueInit(void);

/***************************************************************************
函数名称:        guiguiSendMessage
功能描述：发送一条消息
输入参数:	msg: 要发送的消息
输出参数:	无
使用注意:
***************************************************************************/ 
SYS_EXTERN	void guiSendMessage(MSGTYPE *msg);
SYS_EXTERN	void guiSendMessageEx(WPARAM type,LPARAM value);
SYS_EXTERN	void guiSendMessageNoRepeat(uint32_t type,LPARAM value);

/***************************************************************************
函数名称:        guiGetMessage
功能描述：获取一条消息
输入参数:	msg: 返回消息
输出参数:	TRUE:有消息返回,FALSE:没有消息
使用注意:
***************************************************************************/ 
SYS_EXTERN uint32_t guiGetMessage(MSGTYPE *msg);


/***************************************************************************
函数名称:        guiMessageQueueClear
功能描述：清空消息队列
输入参数:	无
输出参数:	无
使用注意:
***************************************************************************/ 
SYS_EXTERN void guiMessageQueueClear(void);


/************************************************************************
						消息队列析构
************************************************************************/
SYS_EXTERN void guiMessageQueueDeinit(void);

//查询消息队列是否为空
SYS_EXTERN bool guiMessageQueueIsEmpty(void);
#ifdef __cplusplus
}
#endif

#endif /* __GUI_QUEUE_H__ */




