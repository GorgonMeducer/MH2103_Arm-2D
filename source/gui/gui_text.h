#ifndef __GUI_TEXT_H__
#define __GUI_TEXT_H__
#include "inc.h"      
	  
//设置字体
extern HFONT SetFont(HDC dc,HFONT font);

//获取当前字体
extern HFONT GetFont(HDC dc);

extern void SetLineExtra(HDC dc,uint32_t LineExtra);

extern uint32_t GetLineExtra(HDC dc);

extern void SetCharExtra(HDC dc,uint32_t CharExtra);

extern uint32_t GetCharExtra(HDC dc);

extern SINT32 GetCharHeight(HDC dc,UINT8 *ptext);
//计算字符串的显示宽度
extern SINT32 GetStringWidth(HDC dc,SINT8 *ptext);

//输出字符串,当遇到换行符或者超过ClientWindow宽度,将会换到一下行
//x:			横坐标
//y:			纵坐标
//ptext:		需要显示的字符串
extern void guiTextOut(HDC dc,int16_t x,int16_t y,int8_t *ptext);

extern void guiTextOutLine(HDC dc,int16_t x,int16_t y,int8_t *ptext);
#endif


