/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.11.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
/*
lcd1的mcu接口抽象文件，所有上层不同驱动ic共享此接口文件。
接口类型：8bit mcu 8080
*/
#include "inc.h"
#include	"lcd1_if_mcu8.h"



static void Lcd1_Mcu8_IoInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7; //D0-D7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD1_MCU8_RESET_PIN;
    GPIO_Init(LCD1_MCU8_RESET_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD1_MCU8_CS_PIN;
    GPIO_Init(LCD1_MCU8_CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD1_MCU8_DC_PIN;
    GPIO_Init(LCD1_MCU8_DC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD1_MCU8_WR_PIN;
    GPIO_Init(LCD1_MCU8_WR_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = LCD1_MCU8_RD_PIN;
    GPIO_Init(LCD1_MCU8_RD_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD1_MCU8_BK_PIN;
    GPIO_Init(LCD1_MCU8_BK_PORT, &GPIO_InitStructure);

    LCD1_MCU8_RESET_HIGH();
    LCD1_MCU8_WR_HIGH();
    LCD1_RD_HIGH();     //read引脚不使用的时候，一定要接高，不然读写状态不明确。
    LCD1_MCU8_CS_HIGH();
    LCD1_MCU8_BK_OFF();
    DebugPrintf("LCD1 init mcu8080 io!\r\n");
}
//通过mcu接口写一个字节数据
static void Lcd1_Mcu8_WriteByte(uint8_t d)
{
    uint16_t out;

    LCD1_MCU8_WR_LOW();	
    out = (uint16_t)MCU_8080_PORT->ODR;
    out &= 0xFF00;	//only low byte
    out |= d;
    MCU_8080_PORT->ODR = out;
    LCD1_MCU8_WR_HIGH();
}
//硬件复位
static void Lcd1_Mcu8_HardReset(void)
{
    LCD1_MCU8_RESET_HIGH();
    LCD_Delay(10);
    LCD1_MCU8_RESET_LOW();
    LCD_Delay(50);
    LCD1_MCU8_RESET_HIGH();
    LCD_Delay(10);
}
///////////////////////////////lcd命令接口///////////////////////////////////////////////
//写一个lcd寄存器索引值
static void Lcd1_Mcu8_WriteReg( uint8_t reg)
{
    LCD1_DC_LOW();
    LCD1_CS_LOW();
    Lcd1_Mcu8_WriteByte(reg);
    LCD1_CS_HIGH();
}
//写寄存器的数据
static void Lcd1_Mcu8_WriteRegData( uint8_t data)
{
    LCD1_DC_HIGH();
    LCD1_CS_LOW();
    Lcd1_Mcu8_WriteByte(data);
    LCD1_CS_HIGH();
    //	LCD1_DC_LOW();
}

//写寄存器及数据(一条完整的命令)
static void Lcd1_Mcu8_WriteCommand(uint8_t LCD_Reg, uint8_t *pRegValue,uint8_t len)
{
    uint32_t i;

    Lcd1_Mcu8_WriteReg(LCD_Reg);

    LCD1_DC_HIGH();
    LCD1_CS_LOW();
    for(i = 0; i < len; i++)
    {
        uint8_t dat = *pRegValue;
        Lcd1_Mcu8_WriteByte(dat);
        pRegValue++;
    }
    LCD1_CS_HIGH();
    //	LCD_DC_LOW();
}
//============================lcd api接口======================================
//#define MCU18BIT
//写显示数据
static void Lcd1_Mcu8_WriteData(HCOLOR *psrc,uint32_t len)
{
    uint32_t u8Len = len;

    LCD1_DC_HIGH();
    LCD1_CS_LOW();
    while(u8Len--)
    {
        uint8_t h,l;
        uint16_t d;
        d = *psrc++;
    #ifdef MCU18BIT	
        uint8_t g,r,b;	
        r = d >> 11;
        g = (d >> 5) & 0x3f;
        b = d & 0x1F;
        r <<= 3;
        g <<= 2;
        b <<= 3;
        Lcd1_Mcu8_WriteByte(r);     //先写高8bit
        Lcd1_Mcu8_WriteByte(g);     //先写高8bit
        Lcd1_Mcu8_WriteByte(b);     //先写高8bit	
    #else
        h = (uint8_t)((d >> 8) & 0xFF);	
        Lcd1_Mcu8_WriteByte(h);     //先写高8bit
        l = (uint8_t)(d  & 0xFF);
        Lcd1_Mcu8_WriteByte(l);     //再写低8bit	
    #endif
    }
    LCD1_CS_HIGH();	
}
//写同一个颜色的显示数据
static void Lcd1_Mcu8_WriteColor(HCOLOR color,uint32_t len)
{
    uint8_t h,l;
    HCOLOR d;


    d = color;
    h = (uint8_t)((d >> 8) & 0xFF);	
    l = (uint8_t)(d  & 0xFF);
    #ifdef MCU18BIT	
    uint8_t g,r,b;	
    r = d >> 11;
    g = (d >> 5) & 0x3f;
    b = d & 0x1F;
    r <<= 3;
    g <<= 2;
    b <<= 3;
    #endif
    LCD1_DC_HIGH();
    LCD1_CS_LOW();
    while(len--)
    {
    #ifdef MCU18BIT
        Lcd1_Mcu8_WriteByte(r);     //先写高8bit
        Lcd1_Mcu8_WriteByte(g);     //先写高8bit
        Lcd1_Mcu8_WriteByte(b);     //先写高8bit
    #else
        Lcd1_Mcu8_WriteByte(h);     //先写高8bit
        Lcd1_Mcu8_WriteByte(l);     //再写低8bit
    #endif
    }
    LCD1_CS_HIGH();
}
//背光控制
static void Lcd1_Mcu8_BackLightSet(uint16_t duty)
{
    if(LCD_BACKLIGHT_ON == duty) {
        LCD1_BK_ON();
    } else if (LCD_BACKLIGHT_OFF == duty) {
        LCD1_BK_OFF();
    }
    //else //可以支持pwm调背光
}
//休眠进入和唤醒
void Lcd1_Mcu8_SleepInOut(uint16_t status)
{
//	if(sleep) //sleep in
//	{
//	}
//	else //sleep out
//	{
//	}
}
static uint16_t LcdXOffset,LcdYOffset;
static void Lcd1_Mcu8_SetWindow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1)
{
    uint8_t buffer[4];

    x0 += LcdXOffset;
    x1 += LcdXOffset;
    y0 += LcdYOffset;
    y1 += LcdYOffset;

    buffer[0] = (uint8_t)(x0 >> 8);
    buffer[1] = (uint8_t)(x0);
    buffer[2] = (uint8_t)((x1 - 1) >> 8);
    buffer[3] = (uint8_t)(x1 - 1);
    Lcd1_Mcu8_WriteCommand(0x2a,buffer,4);

    buffer[0] = (uint8_t)(y0 >> 8);
    buffer[1] = (uint8_t)(y0);
    buffer[2] = (uint8_t)((y1 - 1) >> 8);
    buffer[3] = (uint8_t)(y1 - 1);
    Lcd1_Mcu8_WriteCommand(0x2b,buffer,4);

    Lcd1_Mcu8_WriteReg(0x2c);

}
void Lcd1_RegInterface_Mcu8(DRVLCD *pLcd,uint16_t xOffset,uint16_t yOffset)
{
    Lcd1_Mcu8_IoInit();
    pLcd->SetWindow		= Lcd1_Mcu8_SetWindow;
    pLcd->SleepInOut 	= Lcd1_Mcu8_SleepInOut;
    pLcd->WriteColor	= Lcd1_Mcu8_WriteColor;
    pLcd->WriteData		= Lcd1_Mcu8_WriteData;
    pLcd->Reset			= Lcd1_Mcu8_HardReset;
    pLcd->BkSet			= Lcd1_Mcu8_BackLightSet;
    pLcd->CtrlWriteReg	= Lcd1_Mcu8_WriteReg;
    pLcd->CtrlWriteData	= Lcd1_Mcu8_WriteRegData;
    pLcd->CtrlWriteCommand = Lcd1_Mcu8_WriteCommand;
    LcdXOffset 			= xOffset;
    LcdYOffset 			= yOffset;
}

/************************END OF FILE*************************************/


