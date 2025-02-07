/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.5.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#ifndef __HAL_LCD_H__
#define	__HAL_LCD_H__
#include "inc.h"
#include "lcd_config.h"


//获取指定操作的lcd的指针
extern DRVLCD *lcdGetLCD(uint16_t lcd);
///////////////////////////////显示屏初始化接口////////////////////////////////////
//主屏LCD初始化
extern  void MainlcdInit(void);

//副屏lcd初始化函数指针类型
typedef void (*drvExtLcdInit)(DRVLCD *pLcd);

enum LCD_type{
	EXT_LCDTYPE_ST7789_MCU_005, //宋,240x280,mcu8
};
extern void ExtlcdInit(uint16_t num);

#endif 

