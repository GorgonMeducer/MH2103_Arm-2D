#ifndef __LCD1_IF_MCU8_H__
#define __LCD1_IF_MCU8_H__
#include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
////////////////////////////define//////////////////////////////////////
#define MCU_8080_PORT	GPIOA

#define LCD1_MCU8_RESET_PORT			GPIOC				//lcd reset port
#define LCD1_MCU8_RESET_PIN				GPIO_Pin_0	//lcd reset pin


#define LCD1_MCU8_CS_PORT				GPIOC				//lcd cs port
#define LCD1_MCU8_CS_PIN				GPIO_Pin_1			//lcd cs pin

#define LCD1_MCU8_DC_PORT				GPIOC				//lcd DC port
#define LCD1_MCU8_DC_PIN				GPIO_Pin_2	//lcd DC pin


#define LCD1_MCU8_WR_PORT				GPIOC				//lcd write port
#define LCD1_MCU8_WR_PIN				GPIO_Pin_3	//lcd write pin

#define LCD1_MCU8_RD_PORT				GPIOC		//lcd read port
#define LCD1_MCU8_RD_PIN				GPIO_Pin_5		//lcd read pin

#define LCD1_MCU8_BK_PORT				GPIOB		//lcd backlight enable port
#define LCD1_MCU8_BK_PIN				GPIO_Pin_0	//lcd backlight enable pin

#define LCD1_MCU8_RESET_HIGH()			GPIO_SetBits(LCD1_MCU8_RESET_PORT, LCD1_MCU8_RESET_PIN)
#define LCD1_MCU8_RESET_LOW()			GPIO_ResetBits(LCD1_MCU8_RESET_PORT, LCD1_MCU8_RESET_PIN)

#define LCD1_MCU8_CS_HIGH()				GPIO_SetBits(LCD1_MCU8_CS_PORT, LCD1_MCU8_CS_PIN)
#define LCD1_MCU8_CS_LOW()				GPIO_ResetBits(LCD1_MCU8_CS_PORT, LCD1_MCU8_CS_PIN)

#define LCD1_MCU8_DC_HIGH()				GPIO_SetBits(LCD1_MCU8_DC_PORT, LCD1_MCU8_DC_PIN)
#define LCD1_MCU8_DC_LOW()				GPIO_ResetBits(LCD1_MCU8_DC_PORT, LCD1_MCU8_DC_PIN)

#if LCD1_BACKLIGHT_TYPE	== LCD1_BACKLIGHT_TYPE_3V3
	#define LCD1_MCU8_BK_OFF()				GPIO_SetBits(LCD1_MCU8_BK_PORT, LCD1_MCU8_BK_PIN)
	#define LCD1_MCU8_BK_ON()				GPIO_ResetBits(LCD1_MCU8_BK_PORT, LCD1_MCU8_BK_PIN)
#elif 	LCD1_BACKLIGHT_TYPE	== LCD1_BACKLIGHT_TYPE_18V
	#define LCD1_MCU8_BK_ON()				GPIO_SetBits(LCD1_MCU8_BK_PORT, LCD1_MCU8_BK_PIN)
	#define LCD1_MCU8_BK_OFF()				GPIO_ResetBits(LCD1_MCU8_BK_PORT, LCD1_MCU8_BK_PIN)
#endif

#define LCD1_MCU8_WR_HIGH()			GPIO_SetBits(LCD1_MCU8_WR_PORT, LCD1_MCU8_WR_PIN)
#define LCD1_MCU8_WR_LOW()			GPIO_ResetBits(LCD1_MCU8_WR_PORT, LCD1_MCU8_WR_PIN)

#define LCD1_MCU8_RD_HIGH()			GPIO_SetBits(LCD1_MCU8_RD_PORT, LCD1_MCU8_RD_PIN)
#define LCD1_MCU8_RD_LOW()			GPIO_ResetBits(LCD1_MCU8_RD_PORT, LCD1_MCU8_RD_PIN)
//复位
#define LCD1_RESET_HIGH()			LCD1_MCU8_RESET_HIGH()
#define LCD1_RESET_LOW()			LCD1_MCU8_RESET_LOW()
//片选
#define LCD1_CS_HIGH()				LCD1_MCU8_CS_HIGH()
#define LCD1_CS_LOW()				LCD1_MCU8_CS_LOW()
//寄存器和RAM选择切换
#define LCD1_DC_HIGH()				LCD1_MCU8_DC_HIGH()
#define LCD1_DC_LOW()				LCD1_MCU8_DC_LOW()
//背光控制
#define LCD1_BK_OFF()				LCD1_MCU8_BK_OFF()
#define LCD1_BK_ON()				LCD1_MCU8_BK_ON()
//写信号
#define LCD1_WR_HIGH()				LCD1_MCU8_WR_HIGH()
#define LCD1_WR_LOW()				LCD1_MCU8_WR_LOW()
//读信号
#define LCD1_RD_HIGH()				LCD1_MCU8_RD_HIGH()
#define LCD1_RD_LOW()				LCD1_MCU8_RD_LOW()
/////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/************************END OF FILE*************************************/

