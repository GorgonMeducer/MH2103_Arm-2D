/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.11.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#include "inc.h"
static BEEPSET beepset;

void BuzzerInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB, GPIO_Pin_9);   //buzzer off
	beepset.run = FALSE;
}

static void BuzzerSet(uint32_t on)
{
	if(on)
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
	else
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}
/////////////////////////////////////////////////////////////////////////
//供异常提示使用
void BuzzerAlwaysOn(void)
{
	BuzzerInit();
	for(int i = 0;i<0x0FFFFFFF;i++)
	{
		WatchDogFeed();
		BuzzerSet(ON);
		DelayMs(200);
		BuzzerSet(OFF);
		DelayMs(200);
	}
}
#define BEEP_TIME_10MS	1
void Beep(uint16_t set_10ms, uint16_t stop_10ms,uint16_t repeat)
{
	DISABLE_INTERRUPT();
	if(beepset.run)
	{
		beepset.run = FALSE;
        BuzzerSet(OFF);
	}
	beepset.Set = BEEP_TIME_10MS*set_10ms;
	beepset.Stop = BEEP_TIME_10MS*stop_10ms;
	beepset.Setshadow = beepset.Set;
	beepset.Stopshadow = beepset.Stop;
	beepset.Repeat = repeat;
//	beepset.mode = mode;
//	switch (mode & BEEP_MODE_MASK)
//	{
//		case BEEP_MODE_NORMAL:
//			beepset.Set = BEEP_TIME_10MS*20;
//			beepset.Stop = BEEP_TIME_10MS*20;
//			beepset.Setshadow = beepset.Set;
//			beepset.Stopshadow = beepset.Stop;
//			beepset.Repeat = 1;
//		break;
//		case BEEP_MODE_ALARM:
//			beepset.Set = BEEP_TIME_10MS*10;
//			beepset.Stop = BEEP_TIME_10MS*10;
//			beepset.Setshadow = beepset.Set;
//			beepset.Stopshadow = beepset.Stop;
//			beepset.Repeat = 5;
//		break;
//		default :
//		return;
//	}
	beepset.run = TRUE;
	BuzzerSet(ON);
	ENABLE_INTERRUPT();
}
void BeepRunCheck(void)
{
	if(beepset.run == FALSE)
		return;
	if(beepset.Setshadow) //鸣叫周期
	{
		beepset.Setshadow--;
		if(beepset.Setshadow == 0)	//鸣叫周期结束
		{
			BuzzerSet(OFF);
		}
	}
	else //停止周期
	{
		if(beepset.Stopshadow)
		{
			beepset.Stopshadow--;
			if(beepset.Stopshadow == 0)	//停止周期结束	
			{
				beepset.Repeat--; //一次循环结束
				if(beepset.Repeat)
				{
					beepset.Stopshadow = beepset.Stop; //reload
					beepset.Setshadow = beepset.Set;
					BuzzerSet(ON);
				}
				else//结束重复周期
				{
					beepset.run = FALSE;
					BuzzerSet(OFF);
				}
			}
		}
	}
}

