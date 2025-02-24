#include "inc.h"
#include "mh.h"
#include "perf_counter.h"
#include "arm_2d_helper.h"
#include "arm_2d_scenes.h"
#include "arm_2d_disp_adapters.h"
#include "arm_2d_disp_adapter_0.h"
#include "arm_2d_scene_watch_face_01.h"
#include "arm_2d_example_controls.h"
#include "arm_2d_demos.h"


extern void RCC_ClkConfiguration(void);
void SysTest(void);

int main(void)
{
    RCC_ClocksTypeDef clocks;

    RCC_ClkConfiguration(); //reset mcu clock，for 216Mhz，default 72Mhz
    RCC_GetClocksFreq(&clocks);
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);	//1ms，定时器，延时函数都要用到systick，要最先初始化
    Uart_debug_int();
    DebugPrintf("SYSCLK: %3.1fMhz, \r\nHCLK: %3.1fMhz, \r\nPCLK1: %3.1fMhz, \r\nPCLK2: %3.1fMhz, \r\nADCCLK: %3.1fMhz\r\n",
    (float)clocks.SYSCLK_Frequency/1000000, (float)clocks.HCLK_Frequency/1000000,
    (float)clocks.PCLK1_Frequency/1000000, 	(float)clocks.PCLK2_Frequency / 1000000, 
    (float)clocks.ADCCLK_Frequency / 1000000);

    DebugPrintf("system clock = %dHz,%dMhz\r\n",SystemCoreClock,SystemCoreClock/1000/1000);
    sFLASH_Init();
    LoadSystemConfig(); //要放到最前面，其他初始化依赖配置信息
    guiInit(); //gui初始要放到osalinit前面。
    //	OsalInit();
    BuzzerInit();
    KeyInit();
    mcuUartInit();
    Timer2Init();	//基于该定时器的调度的应用要先初始化
    Timer3Init();	//超时定时器
    BeepOnce();
    WatchDogInit();
    guiBacklightOnoff(guiGetDC(GUI_DC_MAIN),LCD_BACKLIGHT_ON); //open backlight
    init_cycle_counter(true); //don't init systick

    arm_irq_safe {
        arm_2d_init();
    }
    disp_adapter0_init();
    /*******************************************************************************/	
    {//show demo
        //arm_2d_scene_filters_init(&DISP0_ADAPTER);
        arm_2d_scene_text_reader_init(&DISP0_ADAPTER);
    }
    /*******************************************************************************/	

    while (1) {
        disp_adapter0_task();
    }
}
//CCT6: LQFP48,flash = 256KB,ram=64K
//RCT6: LQFP64,flash = 256KB,ram=64K
//AVGT6: LQFP100,flash = 1024KB,ram=96K
void SysTest(void)
{
    volatile uint32_t t,id1,id2,id3;
    volatile uint32_t sram,flash;

    t = *((uint32_t*)0x1FFFF7E0);
    DebugPrintf("Flash size =0x%04X\r\n",t & 0xFFFF);
    //0x0080：FLASH 容量是 128K 字节。

    DebugPrintf("chip RAM=0x%02X,FLASH=0x%02X\r\n",sram>>4,flash);
    id1 = *((uint32_t*)0x1FFFF7E0+0x08); //chip id,96bit
    id2 = *((uint32_t*)0x1FFFF7E0+0x0C); //chip id,96bit
    id2 = *((uint32_t*)0x1FFFF7E0+0x10); //chip id,96bit
    DebugPrintf("chip tag 96bits ID1=0x%08X,ID2=0x%08X,ID3=0x%08X\r\n",id1,id2,id3);
}
/*
Boot_JumpToApp（msp,jumpaddr);
Boot_JumpToApp  PROC
                 EXPORT  Boot_JumpToApp             [WEAK]
                 CPSID   I             ; Mask interrupts
                 MSR     MSP, R0  ; 设置主栈指针 (MSP) 
                 BX        R1           ; 跳转到复位处理函数
                 ENDP
*/