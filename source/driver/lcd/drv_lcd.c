/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.11.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#include "inc.h"
#include "drv_lcd.h"


static DRVLCD drvLcd[2];

//LCD0支持的屏
extern void lcd_St7789P3_mcu_001_init(DRVLCD *pLcd);

//LCD0初始化
void MainlcdInit(void)
{
    lcd_St7789P3_mcu_001_init(&drvLcd[0]); //ok
}
static DRVLCD *pLcd = &drvLcd[0];
void Disp0_DrawBitmap (uint32_t x,uint32_t y,uint32_t width,uint32_t height,const uint8_t *bitmap)
{
    pLcd->SetWindow(x,y,x+width,y+height);
    pLcd->WriteData((uint16_t *)bitmap,width * height);
}
////////////////////////////////////////////////////////////
//LCD1支持的屏
//和宏定义一一对应
const drvExtLcdInit DrvExtLCD[] = {0,
};
//LCD1初始化
void ExtlcdInit(uint16_t num)
{
    if(num < SIZEOF(DrvExtLCD)) {
        DrvExtLCD[num](&drvLcd[1]);
    } else {
        DrvExtLCD[0](&drvLcd[1]);
    }
}
//获取lcd操作结构的指针
DRVLCD *lcdGetLCD(uint16_t lcd)
{
    return &drvLcd[lcd];
}



