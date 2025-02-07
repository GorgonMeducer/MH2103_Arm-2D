/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2024.10.16
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
/*******************************************************************************
HD24068C22-V4规格书.pdf
********************************************************************************/  
#include "inc.h"
#include "lcd_config.h"

static void lcdControlerInit(DRVLCD *pLcd);
void lcd_St7789P3_mcu_001_init(DRVLCD *pLcd)
{
    pLcd->x = 320;
    pLcd->y = 240;
    Lcd1_RegInterface_Mcu8(pLcd,0,0);
    pLcd->Reset();
    lcdControlerInit(pLcd);
}

#define WriteComm(reg)	pLcd->CtrlWriteReg(reg)
#define WriteData(dat)	pLcd->CtrlWriteData(dat)
#define Delay(x) LCD_Delay(x)
static void lcdControlerInit(DRVLCD *pLcd)
{
    WriteComm( 0x11);     
    LCD_Delay(50);
    WriteComm( 0x36);     
    WriteData( 0x60);   

    WriteComm( 0x3A);     
    WriteData( 0x05);  //0x05( 16bit 65K COLOR,06=18bit)

    WriteComm( 0xB2);     
    WriteData( 0x0C);   
    WriteData( 0x0C);   
    WriteData( 0x00);   
    WriteData( 0x33);   
    WriteData( 0x33);   

    WriteComm(  0xB7);     
    WriteData( 0x00);   

    WriteComm( 0xBB);     
    WriteData( 0x34);   //0x3e???

    WriteComm( 0xC0);     
    WriteData( 0x2c);  

    WriteComm( 0xC2);     
    WriteData( 0x01);   

    WriteComm( 0xC3);     
    WriteData( 0x09);   //GVDD=4.8V             //0x15

    //WriteComm( 0xC4);     
    //WriteData( 0x20);   //VDV, 0x20:0v

    WriteComm( 0xC6);     
    WriteData( 0x0F);   //0x0F:60Hz   

    WriteComm( 0xD0);     
    WriteData( 0xA4);   
    WriteData( 0xA1);

    WriteComm( 0xD6);     
    WriteData( 0xA1);  

    WriteComm( 0xE0);     
    WriteData( 0x70);   
    WriteData( 0x04);   
    WriteData( 0x08);   
    WriteData( 0x0a);   
    WriteData( 0x0a);   
    WriteData( 0x05);   
    WriteData( 0x25);   
    WriteData( 0x33);   
    WriteData( 0x3c);   
    WriteData( 0x24);   
    WriteData( 0x0e);   
    WriteData( 0x0f);   
    WriteData( 0x27);   
    WriteData( 0x2F);   

    WriteComm(  0xE1);     
    WriteData( 0xf0);   
    WriteData( 0x02);   
    WriteData( 0x06);   
    WriteData( 0x06);   
    WriteData( 0x04);   
    WriteData( 0x22);   
    WriteData( 0x25);   
    WriteData( 0x32);   
    WriteData( 0x3b);   
    WriteData( 0x3a);   
    WriteData( 0x15);   
    WriteData( 0x17);   
    WriteData( 0x2d);   
    WriteData( 0x37);   

    WriteComm(  0x21);     //ips屏需要翻转
    WriteComm(  0x29);     
}
/************************END OF FILE*************************************/





