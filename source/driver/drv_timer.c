/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2022.05.10
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2015 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#include "inc.h"
#include "drv_timer.h"

//////////////////////////time2////////////////////////////////
//10ms的定时器
void Timer2Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;	//0.1ms,//max:0xffff
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 1000 / 10 - 1; //自适应系统频率,100us
	TIM_TimeBaseStructure.TIM_Period = 100-1;	//10ms,max:0xfffff
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//	TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);
//	TIM_SetAutoreload(TIM2,TIM_TimeBaseStructure.TIM_Period);
	TIM_ARRPreloadConfig(TIM2, ENABLE);	//enable auto reload

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
extern void mbInput(void);
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		BeepRunCheck();
		KeyScan();
		guiCheckTimer(10);
  }
}
////////////////////////////timer3//////////////////////////////
//1ms定时器，用于通讯超时控制
void Timer3Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;	//0.1ms
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 1000 / 10 - 1;
	TIM_TimeBaseStructure.TIM_Period = 10-1;	//1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//这里ENABLE后，会导致复位后第一次定时中断时间不正确 20220808，
	//原因:重载值会默认为第一次的1ms,也就是说，写重载计数器ARR的同时会把计数器的值设为这个值
	//解决：把初值设大，或者关闭重载
//	TIM_ARRPreloadConfig(TIM3, ENABLE);	//enable auto reload
	TimeOutInit();
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM3, ENABLE);
}
//typedef void (* fpTimeout)(void);		//超时回调函数
static fpTimeout cbTimer = NULL;
void TimeoutTimerEnable(uint32_t ms, fpTimeout ftimeout)
{
	cbTimer = ftimeout;
	//	TIM3->CNT = ms * 10 - 1;	//设置这个寄存器不正确
	TIM3->ARR = ms * 10 - 1;
	TIM3->CR1 |= TIM_CR1_CEN; //	TIM_Cmd(TIM3, ENABLE);
}
void TimeOutTimerReset(uint32_t ms)
{
	TIM3->ARR = ms * 10 - 1;
//	TIM_UpdateRequestConfig(TIM4,1);
	TIM3->CR1 |= TIM_CR1_URS;//放在UG前
	TIM3->EGR |= TIM_EGR_UG; //产生一个更新事件
	TIM3->CR1 |= TIM_CR1_CEN;//需重开定时器
//	TIM_Cmd(TIM4, ENABLE);
}
void TIM3_IRQHandler(void)
{

  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TimeOutCheck(1);
//	TIM_Cmd(TIM3, DISABLE);
//	if(cbTimer)
//		cbTimer();
//	DebugPrintf("timer2 interrupt!\r\n");
  }
}
static TIMEOUT_TIMER tTimer;
//使能超时定时器
//nTimer：哪一个超时定时器
//ms： 超时时间，单位ms
//pfTimeout：超时回调函数
void TimeOutEnable(TIMEOUT_LIST nTimer, uint16_t ms,fpTimeout pfTimeout)
{
	if(nTimer < TIMEOUT_MAX)
	{
		DISABLE_INTERRUPT();
		tTimer.uTime[nTimer] = ms;
		tTimer.uTimeRemaind[nTimer] = ms;
		tTimer.pfCallback[nTimer] = pfTimeout;
		tTimer.uTimerEn |= 1 << nTimer;
		ENABLE_INTERRUPT();
	}
}
//超时定时器初始化
void TimeOutInit(void)
{
	tTimer.uTimerEn = 0;
	for(uint16_t i = 0; i < TIMEOUT_MAX;i++)
		tTimer.pfCallback[i] = NULL;
}
//复位超时定时器
void TimeOutReset(TIMEOUT_LIST nTimer)
{
	DISABLE_INTERRUPT();
	tTimer.uTimeRemaind[nTimer] = tTimer.uTime[nTimer];
	ENABLE_INTERRUPT();
}
//关闭超时定时器
void TimeOutDisable(TIMEOUT_LIST nTimer)
{
	tTimer.uTimerEn &= ~(1 << nTimer);
}
//超时定时器检查
void TimeOutCheck( uint32_t ms)
{
	if(tTimer.uTimerEn)
	{
		for(uint16_t i = 0; i < TIMEOUT_MAX;i++)
		{
			if(tTimer.uTimerEn & (1 << i))
			{
				if(tTimer.uTimeRemaind[i] > ms)
					tTimer.uTimeRemaind[i] -= ms;
				else if(tTimer.pfCallback[i])
				{
					tTimer.uTimerEn &= ~(1 << i);
					tTimer.pfCallback[i]();
				}
			}
		}
	}
}

