/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2022.07.20
  [email]		huangbinrd@163.com
************************************************************************/
#ifndef __GUI_CTRL_LIST_H__
#define __GUI_CTRL_LIST_H__
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include "inc.h"
 
#ifdef __cplusplus
extern "C" {
#endif
/************************************************************************/
//列表框控件

struct tagListBoxItemData
{
	int8_t *pStr;						//字符串指针
	uint32_t StrID;					//字符串在FLASH中的地址,0表示无字符串
	uint32_t IconAddr;				//图标地址,0表示无icon
	uint16_t StrWidth;				//字符串的宽度
	uint16_t IconWidth;				//图标的宽度
	uint16_t IconHeight;			//图标的高度
	uint16_t NeedUpdate;			//是否需要更新数据
	HCOLOR TextColor;				//条目文本显示颜色
};
typedef struct tagListBoxItemData LISTBOXITEM,*PLISTBOXITEM;


//列表控件

//x:当前条目显示的横坐标
//y:当前条目显示的纵坐标
//plbi:当前条目的数据结构指针
//返回值:暂无使用
typedef uint16_t	(* LISTPAINTCALLBACK)(HDC dc,uint16_t x,uint16_t y,uint32_t item);		//列表框显示回调函数函数指针
typedef uint16_t (*LISTLOADCALLBACK)(HDC dc,uint32_t item,PLISTBOXITEM plbi);			//条目加载回调

typedef struct _tagListBox {
	LISTPAINTCALLBACK DrawItem;		//绘制条目回调函数
	LISTLOADCALLBACK LoadItem;		//加载条目回调函数
	uint32_t Total;					//总的列表条目数
	uint16_t ItemHeight;				//条目的高度
	uint16_t x,y,w,h;					//位置参数	

	//下面的参数列表框统一初始化,用户在设置好上面的参数后,使用列表框的MSG_CREATE消息完成初始化
	//PLISTBOXITEM pItemHead;			//条目数据链表	
	uint16_t Page;					//一页的条目数量
	uint32_t Cur;						//当前选择项目(在整个列表中的序号)
	uint32_t Top;						//当前屏幕顶部项目在总的项目的编号
	LISTBOXITEM CurItemData;		//当前选择项目数据
	LISTBOXITEM ItemData;			//项目数据,用作加载数据BUFFER
	
	//左右滚动相关信息
	int16_t LastX;					//上一次显示的X坐标
	int16_t ScrStep;				//滚动步长，为负数时表示向左滚动，为正数时表示向右滚动，数字越大，滚动越快
	int16_t delay;					//文本滚到头时的停留时间，以MSG_TIMEOUT消息个数为单位

	//当前选中项字符串BUFFER
	int8_t buffer[72];
}LISTBOXUI;

#define LIST_NULL		0xFFFF		//列表框没有处理该消息
#define LIST_OK			0xFFFE		//列表框处理了该消息
#define LIST_SELECT		0xFFFD		//选择了条目并且按下确定键

#define LIST_TEXT_X_OFFSET		4		//列表显示文字的时候x的偏移

#define LIST_ARG_PAINT			0		//正常绘制列表
#define LIST_ARG_SECOND			1		//秒消息绘制列表

#define STR_INVALID_ID											0xffffffff
//列表控件处理
extern uint16_t ListBoxWndProc(HDC dc,LISTBOXUI *pLst,WPARAM wParam,LPARAM lParam);

					
/************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* __GUI_QUEUE_H__ */




