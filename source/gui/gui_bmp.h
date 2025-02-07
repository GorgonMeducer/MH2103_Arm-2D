#ifndef __GUI_BMP_H__
#define __GUI_BMP_H__
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
 
#ifdef __cplusplus
extern "C" {
#endif
#include "inc.h"
////////////////////////////////////////////////////////////////////////////
//正常情况输出图片
extern void guiPutBmp(HDC dc,uint32_t noraddr,uint16_t x,uint16_t y);
// 交换高低字节方式显示位图
extern void PutBmpSwap16(HDC dc,uint32_t noraddr,uint16_t x,uint16_t y);
//输出ROM空间的图片
extern void guiPutBmpByRom(HDC dc,const uint8_t *pBmp,uint16_t x,uint16_t y);
//获取BMP图片的大小
//noraddr:bmp图片在FLASH中的地址
//pWidth:用于输出图片宽度
//pHeight:用于输出图片高度
extern void guiGetBmpSize(HDC dc,uint32_t noraddr,uint16_t *pWidth,uint16_t *pHeight);

////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif /* __GUI_QUEUE_H__ */




