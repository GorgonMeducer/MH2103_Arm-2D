#ifndef __GUI_BASIC_H__
#define __GUI_BASIC_H__
#include "inc.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
/////////////////////////////////////////////////////////////////////////
/************************************************************************
GUI的概念:	屏幕的输出都是基于DC结构的设置,包括颜色,模式,用的画笔等,
			同时受限于当前客户区的窗口范围(被窗口剪切).
关于坐标 :	1.基于屏幕的操作坐标都是从0开始,不包括最后一个点.
			比如要画一条10个点的线: 需要给的坐标是(0,0,0,10)而不是(0,0,0,9)
			如果坐标以x0,y0,x1,y1的形式给出,x0,y0表示左上角,x1,y1表示右下角
			2.gui函数的坐标是相对于当前客户区的左上角的偏移.
			  如客户区为(10,10,100,100),输出坐标(0,0),表示实际的绘制坐标在屏幕的绝对坐标(10,10)
************************************************************************/
#define HFONT	uint16_t
//屏幕输出模式定义
#define ROP_COPY         0        //copy to screen
#define ROP_AND         1        //and with screen pixel
#define ROP_OR          2        // or with screen pixel
#define ROP_XOR         3        // Xor with screen pixel

//文字输出模式(背景模式)
#define 	FT_OPAQUE               0x00		//不透明
#define 	FT_TRANSPARENT          0x01		//透明

//文字输出行模式
#define 	FT_MULTI_LINE			0x00		//多行显示,自动换行
#define 	FT_SINGLE_LINE          0x01		//单行显示,不换行

//等宽字体(以中文为模式，英文宽度减半)
#define		FONT8x12					0x01
#define		FONT8x16					0x02		//支持英文
#define		FONT16x16					0x03		//
#define		FONT24x24					0x04		//支持英文
#define		FONT32x32					0x05	

#define		FONT_MAX_W		24
#define		FONT_MAX_H		24
//语言种类,定义顺序不可随便修改，ui图片顺序需要和这个顺序保持一致
typedef enum
{
	LAN_CHINESE  = 0,		/* 中文 */
	LAN_ENGLISH  = 1,		/* 英文 */
	LAN_JAPANESE = 2,		/* 日语 */
	LAN_TRADITIONAL_CHINESE = 3, /* 中文繁体 */
	LAN_FRENCH  = 4,		/* 法语 */
	LAN_GERMAN  = 5,		/* 德语 */
	LAN_ITALIAN = 6,		/* 意大利语 */
	LAN_KOREAN  = 7,		/* 韩语 */
	LAN_PORTUGUESE = 8,		/* 葡萄牙语 */
	LAN_RUSSIAN = 9,		/* 俄语 */
	LAN_SPANISH = 0xA,		/* 西班牙语 */
	LAN_DUTCH  = 0xB,	    /* 荷兰语 */
	LAN_HEBREW = 0xC,       /* 希伯来语 */
	LAN_ARABIC = 0xD,       /*  阿拉伯语 */
	LAN_MALAYSIAN = 0xE,     /* 马来西亚语 */
	LAN_THAI = 0xF,         /*  泰语 */
	LAN_VIETNAMESE = 0x10,   /* 越南语 */
	LAN_INDONESIA = 0x11,   /*  印尼 */
	LAN_MAX,
}Language_Typedef;
#define DEFAULT_LANGUAGE   2  
#define LANGUAGE_MAX	8	//最大语言数量(根据实际使用的语言资源数量修改)

typedef struct _RECT
{
    int16_t    left;    //the x coordinate of the upper-left corner of the rectangle.
    int16_t     top;    //the y coordinate of the upper-left corner of the rectangle.
    int16_t     right;    //the x coordinate of the lower-right corner of the rectangle.
    int16_t     bottom;    //the y coordinate of the lower-right corner of the rectangle.
} RECT,*PRECT;

typedef struct _POINT
{
    int16_t    x;        //the x coordinate of the point.
    int16_t    y;        //the y coordinate of the point.
} POINT,*PPOINT;

//显示方向类型定义
typedef enum
{
	DIR_HORIZONTAL,
	DIR_VERTICAL,
}DIRECTION;

typedef struct tagDC
{
    HCOLOR			BkColor;		//背景颜色
    HCOLOR			PenColor;		//笔的颜色
	HCOLOR			BrushColor;		//画刷的颜色(填充图形函数使用)
    int16_t 		CharExtra;		//字符间距(象素)
    int16_t			LineExtra;		//行的上面间隔(象素)
    int16_t			BkMode;			//背景模式(文字输出的模式)
	HFONT			pLogFont;		//当前字体
	int16_t		  	language;		//语言
	RECT			rcWnd;			//窗口size
    DRVLCD			*pLcd;			//当前屏幕操作函数集
}DC,*PDC;

//extern	DC dc[];
#define HDC	PDC

//////////////////////////////// gui core API///////////////////////////////////
//功能：设置背景颜色
void guiSetBkColor(HDC hdc,HCOLOR color);

//功能：获取背景颜色
HCOLOR guiGetBkColor(HDC hdc);

//功能：设置画刷颜色
void guiSetBrushColor(HDC hdc,HCOLOR color);

//功能：获取画刷颜色
HCOLOR guiGetBrushColor(HDC hdc);

//功能：设置画笔颜色
void guiSetPenColor(HDC hdc,HCOLOR color);

//功能：获取画笔颜色
HCOLOR guiGetPenColor(HDC hdc);

//功能：设置背景模式
//参数：
//bkMode:	FT_OPAQUE 不透明
//            FT_TRANSPARENT 透明
void guiSetBkMode(HDC hdc,uint32_t bkMode);

/**
 * 函数：uint32_t guiGetBkMode(void);
 * 功能：获取背景模式
 * 返回：FT_OPAQUE 不透明, FT_TRANSPARENT 透明 
 */
uint32_t guiGetBkMode(HDC hdc);


//设置和获取窗口区域
extern	void guiSetDefaultWindow(HDC hdc);
extern	void guiSetClientWindow(HDC hdc,int16_t x0,int16_t y0, int16_t x1, int16_t y1);
extern	void guiGetClientWindow(HDC hdc,RECT *rc);

//设置默认的DC环境
extern	void guiSetDefaultGui(HDC hdc);
 
//功能：设置字体
extern	HFONT guiSetFont(HDC hdc,HFONT font);

//功能：获取当前字体
extern	HFONT guiGetFont(HDC hdc);

//获取字符间距
extern	int16_t guiGetFontExtra(HDC hdc);

//设置字符间距
extern	int16_t guiSetFontExtra(HDC hdc,int16_t extra);

//功能：获取当前语言
extern	void guiSetLanguage(HDC hdc,int16_t lan);

//功能：获取当前语言
extern	uint16_t guiGetLanguage(HDC hdc);

//功能：获取当前DC
#define	GUI_DC_MAIN		0
#define	GUI_DC_SUB		1
//src_id:屏幕id
extern	HDC guiGetDC(uint16_t src_id);


//获取屏幕最大x分辨率
extern	HCOLOR guiGetMaxX(HDC hdc);

//获取屏幕最大y分辨率
extern	HCOLOR guiGetMaxY(HDC hdc);

/**
 * 函数：bool InRect( const LCDRECT *rect, const LCDPOINT *point)
 * 功能：判断一个点是否在矩形区域中
 * 参数：
 *  @rect 矩形区域
 *  @ point 点
 * 返回：在矩形区域中true/不在矩形区域中false
 */
uint8_t guiInRect( const RECT *rect, const POINT *point);

//ui system init
extern void guiInit(void);

//初始化外部LCD
extern void guiInitExtLcd(uint16_t num);

//开关对应设备的背光
//#define LCD_BACKLIGHT_ON	100
//#define LCD_BACKLIGHT_OFF	0
extern void guiBacklightOnoff(HDC hdc, uint16_t duty);

//读设备的寄存器数据
//regIndex: 寄存器索引(寄存器编号)
//regNum: 要读的数据字节数(最多4个字节)
//返回：0x11223344(从高到低依次存放参数1-4)
extern uint32_t guiReadReg(HDC hdc,uint8_t regIndex,uint8_t regNum);
//for demo
extern void guidemo(void);


#define GUIX_LOCK()		//DISABLE_INTERRUPT()//互斥信号，或者关闭中断
#define GUIX_UNLOCK()	//ENABLE_INTERRUPT()//互斥信号，或者开中断

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/************************END OF FILE*************************************/
