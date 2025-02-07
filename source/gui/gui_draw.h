#ifndef __GUI_DRAW_H__
#define __GUI_DRAW_H__
#include "inc.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
/////////////////////////////////////////////////////////////////////////	
/**
 * 原型：void guiPoint(int16_t x0,int16_t y0)
 * 功能：用指定的颜色绘制点
 * 参数：
 *  @hdc 设备句柄  
 *  @x0 起始位置x  
 *  @y0 起始位置y
 * 返回：无
 */
void guiPoint(HDC hdc,int16_t x0,int16_t y0);

/**
 * 原型：void guiLine(int16_t x0,int16_t y0, int16_t x1, int16_t y1)
 * 功能：绘制一条线段
 * 参数：
 *  @hdc 设备句柄  
 *  @x0 起始位置x  
 *  @y0 起始位置y
 *  @x1 终止位置x
 *  @y1 终止位置y
 * 返回：无
 */
void guiLine(HDC hdc,int16_t x0,int16_t y0, int16_t x1, int16_t y1);
 
/**
 * 原型：void guiRectangle(int16_t x0,int16_t y0, int16_t x1, int16_t y1)
 * 功能：用指定的颜色绘制矩形
 * 参数：
 *  @hdc 设备句柄  
 *  @x0 起始位置x
 *  @y0 起始位置y
 *  @x1 结束位置x
 *  @y1 结束位置y
 * 返回：无
 */
void guiRectangle(HDC hdc,int16_t x0,int16_t y0, int16_t x1, int16_t y1);

/**
 * 原型：void guiFillRect(int16_t x0,int16_t y0, int16_t x1, int16_t y1)
 * 功能：用指定的颜色填充矩形区域
 * 参数：
 *  @hdc 设备句柄  
 *  @x0 起始位置x
 *  @y0 起始位置y
 *  @x1 结束位置x
 *  @y1 结束位置y
 * 返回：无
 */
void guiFillRect(HDC hdc,int16_t x0,int16_t y0, int16_t x1, int16_t y1);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/************************END OF FILE*************************************/

