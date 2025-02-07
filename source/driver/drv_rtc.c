/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2022.08.19
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2015 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#include "inc.h"


void RtcNVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RTC_WaitForSynchro();
    RTC_WaitForLastTask();
    RTC_ITConfig(RTC_IT_SEC, ENABLE);   //second int
    RTC_WaitForLastTask();
}
#define RTC_ENABLE_FALG1		0x4842 //HB
#define RTC_ENABLE_FALG2		0x5A4A //ZJ
//写入软件升级标志
//use: BKP_DR10-13
#define SOFTUPDATE_FALG1		0x4842 //HB
#define SOFTUPDATE_FALG2		0x5A4A //ZJ
#define SOFTUPDATE_FALG3		0xC9FD	//升
#define SOFTUPDATE_FALG4		0xBCB6	//级
//清除软件升级标志
void SetSoftwareUpdateFlag(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    
    BKP_WriteBackupRegister(BKP_DR10, SOFTUPDATE_FALG1);
    BKP_WriteBackupRegister(BKP_DR11, SOFTUPDATE_FALG2);
    BKP_WriteBackupRegister(BKP_DR12, SOFTUPDATE_FALG3);
    BKP_WriteBackupRegister(BKP_DR13, SOFTUPDATE_FALG4);
    
    PWR_BackupAccessCmd(DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, DISABLE);
}

#define LSE_STARTUP_TIMEOUT		0x300000
void RtcInit(void)
{
     UINT32 StartUpCounter = 0,LSEStatus;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    if ((BKP_ReadBackupRegister(BKP_DR1) != RTC_ENABLE_FALG1) && (BKP_ReadBackupRegister(BKP_DR2) != RTC_ENABLE_FALG2))
    {
        DebugPrintf("RTC not yet configured....\r\n");
        BKP_DeInit();
        RCC_LSEConfig(RCC_LSE_ON);
        StartUpCounter = 0;
        do
		{
			LSEStatus = RCC_GetFlagStatus(RCC_FLAG_LSERDY);
			StartUpCounter++;
		} while((LSEStatus == RESET) && (StartUpCounter < LSE_STARTUP_TIMEOUT));//Wait LSE is ready
		if(LSEStatus == RESET)
		{   //crystal fail
            DebugPrintf("crystal fail\r\n");
		}
		else
		{
            RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
            RCC_RTCCLKCmd(ENABLE);
            RTC_WaitForSynchro();   //wait apb1 clock sync
            RTC_WaitForLastTask();  //wait write finish

            RtcNVIC_Configuration();

            RTC_SetPrescaler(32767); //RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
            RTC_WaitForLastTask();
            {
                DATETIME dt;
               
                dt.year = 2024; 
                dt.month = 10;
                dt.day = 1;
                dt.week = 1;
                dt.hour = 12;
                dt.minute = 9;
                dt.second = 9;
                RTC_SetCounter(DataTimeToSecond(&dt));
                RTC_WaitForLastTask();            
            }
            
            BKP_WriteBackupRegister(BKP_DR1, RTC_ENABLE_FALG1);
            BKP_WriteBackupRegister(BKP_DR2, RTC_ENABLE_FALG2);
        }
    }
    else
    {
        DebugPrintf("RTC configured....\r\n");
        RtcNVIC_Configuration();
    } 
    PWR_BackupAccessCmd(DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, DISABLE);
}
void RTC_IRQHandler(void)
{
//	static  uint32_t rtc_counter = 0;
	
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_SEC);
    RTC_WaitForLastTask();
	guiSendMessageEx(MSG_SECOND,0);
//    DebugPrintf("RTC second = %d\r\n",rtc_counter++);
  }
  else if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
  {
  }
}
///////////////////////////////////////////////////////////////////////////
//基姆拉尔森计算公式: W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7
//  在公式中d表示日期中的日数，m表示月份数，y表示年数。注意：在公式中有个与其他公式不同的地方：
//  把一月和二月看成是上一年的十三月和十四月，例：如果是2004-1-10则换算成：2003-13-10来代入公式计算
//w=0-6(星期一-星期天)
//根据年月日求出来星期几
uint32_t GetWeekDay(DATETIME *pDt)
{
	uint32_t y,m,d;
	uint32_t week;

	y = pDt->year;
	m = pDt->month;
	d = pDt->day;
	if (m < 3)
	{
		m += 12;
		y -= 1;
	}
	week = (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7; 	//1=monday
	return week+1;//week=1-7,转换为自己的格式
}
void GetRtc(DATETIME *dt)
{
    uint32_t sec;

    RTC_WaitForSynchro();
    sec = RTC_GetCounter();
    SecondToDataTime(dt,sec);
    dt->week = GetWeekDay(dt);
}
extern void RCC_Reset_Backup(void);
void SetRtc(DATETIME *dt)
{
    RCC_Reset_Backup();     //先复位rtc备份域，防止备份域掉电后处于未知状态
    RtcInit();              //reinit
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    RTC_WaitForSynchro();
    RTC_SetCounter(DataTimeToSecond(dt));
    RTC_WaitForLastTask();            

    PWR_BackupAccessCmd(DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, DISABLE);
}
//将时间转换距2000.1.1 0:00:00的秒数
//返回转换结果的数值
uint8_t const DayOfMonthList[] = {31,28,31,30,31,30,31,31,30,31,30,31};
uint32_t DataTimeToSecond(DATETIME *pDt)
{
	uint32_t i;
	uint32_t TotalDay;
	uint32_t TotalSec;

	TotalDay = pDt->year - 2000;
	if(	TotalDay ) //不是2000年
		TotalDay = TotalDay * 365 + (TotalDay + 3) / 4;	//计算当前年距离起始年的天数和经过了多少个闰年
	for(i = 0; i < (uint32_t)pDt->month - 1;i++)		//再计算余下的月份里有多少天
	{
		TotalDay += DayOfMonthList[i];
	}
	if((((pDt->year) % 4) == 0) && (pDt->month > 2))	//闰年(2000-2100年,简化),并且当前月份大于2月
		TotalDay++;
	TotalDay += pDt->day - 1;		//计算出来已经过去的天数
	TotalSec = 	(uint32_t)TotalDay * 3600L * 24L; //过去的天数有多少秒
	TotalSec += (uint32_t)pDt->hour * 3600L +(uint32_t) pDt->minute * 60L +  (uint32_t)pDt->second;

	return TotalSec;
}
//将秒数转换为时间
uint16_t const MonthDayofYear[12] = {31,31+28,31+28+31,31+28+31+30,
                                   120+31,120+31+30,120+31+30+31,120+31+30+31+31,
                                    243+30,243+30+31,243+30+31+30,243+30+31+30+31,
                                    };
uint16_t const MonthDayofleapYear[12] = {31,31+29,31+29+31,31+29+31+30,
                                   121+31,121+31+30,121+31+30+31,121+31+30+31+31,
                                    244+30,244+30+31,244+30+31+30,244+30+31+30+31,
                                    };
void SecondToDataTime(DATETIME *pDt,uint32_t second)
{
    uint32_t year,day,remain;
    uint32_t leap,i;
    uint16_t const *pTable;
    

    day = second / (3600L*24L) + 1;  //多少天(是从1.1日起,不是0)
    year = day / 365;          
    remain = day % 365;
    
    leap = (year + 3) / 4;   //2000以来过了多少个闰年(不包含当前年)
    pDt->year = year + 2000;
    if(remain <= leap)   //剩余天数不够补闰年
    {
        pDt->year -= 1;
        if((pDt->year % 4) == 0) //闰年
            remain = remain + 366 - leap;
        else
            remain = remain + 365 - leap;
    }
    else
    {
       remain -= leap;  //已经过去的闰年,不包含本年
    }
    if((pDt->year % 4) == 0) //闰年
        pTable = MonthDayofleapYear;
    else
        pTable = MonthDayofYear;
    for(i = 0; i < 12;i++)
    {
        if(remain <= pTable[i])
            break;
    }
    pDt->month = i + 1;
    if(i)
        pDt->day = remain - pTable[i - 1];
    else
        pDt->day = remain;
    
    remain = second % (3600L*24); //剩余一天的秒数
    pDt->hour = (UINT8)(remain / 3600L);
    remain  = remain % 3600L;
    pDt->minute = remain / 60;
    pDt->second = remain % 60;
}

//////////////////////////////////////////////////////////



