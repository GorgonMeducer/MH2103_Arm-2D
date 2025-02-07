#ifndef __LCD_CONFIG_H__
#define __LCD_CONFIG_H__
#include "stdint.h"
#include "config.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
////////////////////////////define//////////////////////////////////////
#define LCD1_BACKLIGHT_TYPE_3V3	1		//单个LED并联
#define LCD1_BACKLIGHT_TYPE_18V	2		//多个LED串联

#define LCD1_BACKLIGHT_TYPE		LCD1_BACKLIGHT_TYPE_3V3


#define HCOLOR	uint16_t		//颜色宏定义
//转换颜色
//extern	HCOLOR guiMakeColor(HDC hdc,uint8_t r,uint8_t g,uint8_t b);
#define	MAKERGB565(r,g,b)   ((((uint16_t)((r) >> 3)) << 11) | (((uint16_t)((g) >> 2)) << 5) | ((uint16_t)((b) >> 3)))    //rrrrrggg,gggbbbbb 

#define COLOR_WHITE		MAKERGB565(255,255,255)
#define COLOR_BLACK		MAKERGB565(0,0,0)
#define COLOR_RED		MAKERGB565(255,0,0)
#define COLOR_GREEN		MAKERGB565(0,255,0)
#define COLOR_BLUE		MAKERGB565(0,0,255)
#define COLOR_YELLOW	MAKERGB565(255,255,0)
#define COLOR_GRAYHALF	MAKERGB565(128,128,128)

//lcd 抽象的操作接口
typedef void (*drvLcdSetWindow)(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1);
typedef void (*drvLcdWriteData)(HCOLOR *psrc,uint32_t len);
typedef void (*drvLcdWriteColor)(HCOLOR color,uint32_t len);
typedef void (*drvLcdSleepInOut)(uint16_t sleep);
typedef void (*drvLcdHardReset)(void);
typedef void (*drvLcdWriteCtrlReg)( uint8_t reg);	//寄存器操作
typedef void (*drvLcdWriteCtrlData)( uint8_t data); //数据操作
typedef void (*drvLcdWriteCtrlCommand)(uint8_t LCD_Reg, uint8_t *pRegValue,uint8_t len); //WriteReg+WriteData的简化版本
//读寄存器数据
//regIndex: 寄存器索引(寄存器编号)
//regNum: 要读的数据字节数(最多4个字节)
//返回：0x11223344(从高到低依次存放参数1-4)
typedef  uint32_t (*drvLcdReadReg)(uint8_t regIndex,uint8_t regNum);
//duty ratio:0-100: 0=off,100=on,other=pwm duty ratio
#define LCD_BACKLIGHT_ON	100
#define LCD_BACKLIGHT_OFF	0
typedef void (*drvLcdBacklightSet)(uint16_t duty);

typedef struct lcdDriver{
	drvLcdSetWindow		SetWindow;		//set window
	drvLcdWriteData		WriteData;		//write data
	drvLcdWriteColor	WriteColor;		//write color
	drvLcdSleepInOut	SleepInOut;		//sleep in/out
	drvLcdBacklightSet	BkSet;			//backlight set
	drvLcdHardReset		Reset;			//hardware reset
	drvLcdWriteCtrlReg		CtrlWriteReg;		//write controller register
	drvLcdWriteCtrlData		CtrlWriteData;		//write controller register’s parameter
	drvLcdWriteCtrlCommand	CtrlWriteCommand; 	//write controller register and parameter
	drvLcdReadReg			ReadReg;			//read register data
	uint16_t	x,y;							//screen resolution
}DRVLCD;

//专用于lcd底层的延时函数(阻塞类型的)
#define LCD_Delay(ms)			DelayMs(ms)

//注册lcd1的mcu 8bit驱动接口
//DRVLCD:驱动管理接口
//xOffset: 窗口偏移x
//yOffset: 窗口偏移y
//注册lcd1的mcu 8bit驱动接口
extern void Lcd1_RegInterface_Mcu8(DRVLCD *pLcd,uint16_t xOffset,uint16_t yOffset);

/////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/************************END OF FILE*************************************/

