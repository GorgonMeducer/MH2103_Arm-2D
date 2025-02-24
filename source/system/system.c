/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2022.05.10
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2015 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#include "inc.h"
#include "mh.h"

static uint32_t systick_cnt = 0;
static uint32_t os_cnt = 0;
static uint32_t SleepCounter = 0;
static bool	SystemRunning = true;
void CheckSleepTime(void);
void SysTick_Handler(void)
{
    systick_cnt++;
    os_cnt++;
    CheckSleepTime();
    if(os_cnt >= 10) {
        os_cnt = 0;
    //  osalTimerIsr(OSAL_TIMER_MS*10);	
    }
}
const uint32_t SleepTime[] = {30*1000,60*1000,300*1000,0xFFFFFFFF};
void CheckSleepTime(void)
{
//	if(SystemRunning)
//	{
//		if(gSet.chSleepTime != SLEEP_TIME_xxxS)
//		{
//			SleepCounter++;
//			if(SleepCounter > SleepTime[gSet.chSleepTime])
//			{
//				SystemRunning = false;
//				SleepCounter = 0;
//				guiBacklightOnoff(guiGetDC(GUI_DC_MAIN),LCD_BACKLIGHT_OFF); //close backlight

//			}
//		}
//		else
//			SleepCounter = 0;
//	}
}
bool ClearSleepTimeByKey(void)
{
    if(SystemRunning == false) {
    SystemRunning = true;
        //  guiBacklightOnoff(guiGetDC(GUI_DC_MAIN),LCD_BACKLIGHT_ON); //open backlight
        return true;
    }
    SleepCounter = 0;
    return false;
}

bool GetSystemRunStatus(void)
{
    return SystemRunning;
}

void DelayMs(uint32_t ms)
{
    volatile uint32_t i,j;
    WatchDogFeed();
    for(i = 0;i < ms;i++) {
        for(j = 0;j < 4500;j++) __NOP();;
    }
}

void HardFault_Handler(void)
{
#define WORD_LED_PORT   GPIOC
#define WORD_LED_PIN    GPIO_Pin_13

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = WORD_LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(WORD_LED_PORT, &GPIO_InitStructure);

    while(1) {
        WatchDogFeed();
        GPIO_ResetBits(WORD_LED_PORT, WORD_LED_PIN);   // Toggle LED
        for ( int i = 0; i < 66666; ++i) __NOP();
        GPIO_SetBits(WORD_LED_PORT, WORD_LED_PIN);   // Toggle LED
        for ( int i = 0; i < 66666; ++i) __NOP();
    }
}
void WatchDogInit(void)
{
#ifdef WATCHDOG_ENABLE	
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_128); //clock input = 40K,period = 3.2ms
    IWDG_SetReload(1000); //(0 -4095)3.2s
    IWDG_ReloadCounter(); 
    IWDG_Enable(); 	
    DebugPrintf("watchdog running!\r\n");
#endif
}
void WatchDogFeed(void)
{
#ifdef WATCHDOG_ENABLE
    IWDG_ReloadCounter();
    //DebugPrintf("watchdog Feed!\r\n");
#endif
}
extern UINT32 wndMainWndProcess(WPARAM wParam, LPARAM lParam);
SYS_CONFIG gConfig;
void LoadSystemConfig(void)
{
    gConfig.MainLcdEn 	= ENABLE;
    gConfig.ExternLcdEn = DISABLE;
    gConfig.CTpEn    = DISABLE;
//  gVar.pWin = wndMainWndProcess;
//  LoadMenuConfig();
}

void RCC_ClkConfiguration(void)
{
    RCC_DeInit();

    RCC_HSEConfig(RCC_HSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    RCC_PLLCmd(DISABLE);
    MH_RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_27,1);

    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);

//  RCC_LSICmd(ENABLE);
//  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
//  RCC_HSICmd(ENABLE);
//  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
}


__attribute__((constructor))
void system_init(void)
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
}
