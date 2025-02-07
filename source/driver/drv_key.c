/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.11.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#include "inc.h"

#define KEY_SCAN_SPACE_CYCLE			20	//按键扫描周期,以10ms为单位
typedef enum
{
	STA_NO_KEY,						//没有按键按下
	STA_SCAN_DELAY,					//延时检测状态，防抖
	STA_KEY_DOWN,					//有按键按下
}KEYSTATUS;

typedef	struct tagKeyBoard 
{
	KEYSTATUS		KeyStatus;				//键盘状态
	uint32_t			LastScanCode;			//上一次的键值
	uint32_t			KeyDownTime;			//按键按下的时间
	uint32_t			KeyScanCount;			//按键扫描计数
} KEYDATA;

KEYDATA Keydata;

void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	memset(&Keydata,0,sizeof(Keydata));
	Keydata.KeyStatus = STA_NO_KEY;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
uint32_t HardWareKeyScan(void)
{
	uint32_t ScanCode;
	uint32_t PortC;
	
    ScanCode = GPIOB->IDR;
    ScanCode = ~ScanCode;
    ScanCode &= 0xF000; //12-15
	ScanCode >>= 12;
	
	PortC = GPIOC->IDR;//pc6-7
	PortC = ~PortC;
	PortC &= 0x00C0;
	PortC >>= 2;
	
	ScanCode |= PortC;
	return ScanCode;
}
//统计有多少个位的值为1
uint32_t Getbit1Count(uint32_t value)
{	
	value = value - ((value >> 1) & 0x55555555);
	value = (value & 0x33333333) + ((value & 0xCCCCCCCC) >> 2);
	value = (value + (value >> 4)) & 0x0F0F0F0F;
	value = value + (value >> 8);
	value = value + (value >> 16);
	return value & 0x3F;
}
void SendKeyMessage(uint32_t MessageType,uint32_t ScanCode);
void KeyScan(void)
{
	uint32_t ScanCode = 0;
	
	if (Keydata.KeyStatus == STA_KEY_DOWN)
	{
		if (++Keydata.KeyScanCount > KEY_SCAN_SPACE_CYCLE)
		{
			Keydata.KeyScanCount = 0;
            if(Keydata.KeyDownTime < 0xFFFF - KEY_SCAN_SPACE_CYCLE)
                Keydata.KeyDownTime += KEY_SCAN_SPACE_CYCLE;
			ScanCode = HardWareKeyScan();
			if (ScanCode)
			{
				if (Getbit1Count(ScanCode) == 1)
				{					
					if (ScanCode & Keydata.LastScanCode)
					{
						//发送key down 消息
						SendKeyMessage(MSG_KEYDOWN,ScanCode);
//						DebugPrintf("key down 2 = 0x%x\r\n",ScanCode);
					}
					else 
					{
						//有两个键同时按下，先按的键已弹起
						SendKeyMessage(MSG_KEYUP,Keydata.LastScanCode);
//						DebugPrintf("key up 2 = 0x%x\r\n",ScanCode);
						Keydata.KeyStatus = STA_SCAN_DELAY;
						Keydata.LastScanCode = ScanCode;
					}
				}
			}
			else
			{				
				//发送key up 消息
				SendKeyMessage(MSG_KEYUP,Keydata.LastScanCode);
//				DebugPrintf("key up = 0x%x\r\n",ScanCode);
				Keydata.KeyStatus = STA_NO_KEY;
				Keydata.LastScanCode = 0;
				Keydata.KeyDownTime = 0;
			}
		}
	}
	else
	{
		if (++Keydata.KeyScanCount > 3)
		{
			ScanCode = HardWareKeyScan();
			if (Keydata.KeyStatus == STA_SCAN_DELAY)
			{
				Keydata.KeyScanCount = 0;
				if (ScanCode == Keydata.LastScanCode)
				{
					//发送key down 消息
					SendKeyMessage(MSG_KEYDOWN,ScanCode);
//					DebugPrintf("key down 1 = 0x%x\r\n",ScanCode);
					Keydata.KeyStatus = STA_KEY_DOWN;
					Keydata.KeyScanCount = 0;
				}
				else
				{
					Keydata.KeyStatus = STA_NO_KEY;
				}
			}
			else if (Keydata.KeyStatus == STA_NO_KEY)
			{
				if (ScanCode)
				{
					if (Getbit1Count(ScanCode) == 1)		//只有一个键按下才有效
					{
						Keydata.KeyStatus = STA_SCAN_DELAY;
						Keydata.LastScanCode = ScanCode;
					}
				}
			}
		}
	}
}
//按键物理键值与逻辑键值对应表
const uint16_t ScanCodeToKeyCode[] = 
{
	//逻辑键值
	VK_ESC,
	VK_ENTER,
	VK_RIGHT,
	VK_LEFT,
	VK_DOWN,
	VK_UP,
	VK_NULL,
	VK_NULL,
};
/*
void SendKeyMessage(uint32_t MessageType,uint32_t ScanCode)
{
	MSGTYPE msg;
	
	msg.type = MessageType;
	msg.value = ScanCodeToKeyCode[Getbit1Count(ScanCode-1)];
//	DebugPrintf("key value=%d\r\n",msg.value);
	msg.value |= (Keydata.KeyDownTime << 16);
	if (Keydata.KeyDownTime)
	{
		msg.value &= ~KS_KEYSTATUS;
	}
	else
	{
		msg.value |= KS_KEYSTATUS;
	}
    if(msg.type == MSG_KEYDOWN)
    {
        BeepOnce();
    }

//	DebugPrintf("Key: pre-status:%s,time:%0dms,keytype:%s, keyvalue=%0x\r\n",
//    PREKEY_ISUP(msg.value)?"up":"down",GETKEY_TIME(msg.value),msg.type == 1?"msg_keydown":"msg_keyup",GETKEY_VALUE(msg.value));
	guiSendMessage(&msg);
}
*/
void SendKeyMessage(uint32_t MessageType,uint32_t ScanCode)
{
	if(MessageType == MSG_KEYDOWN)
	{
//		Beep(5,5,1);
//		DebugPrintf("===Key===\r\n");
//		rg_send_key_evt(nygGetGuiInstance(),NGY_MSG_KEY_EVT_DOWN,ScanCodeToKeyCode[Getbit1Count(ScanCode-1)]);
	}
	else
	{
//		rg_send_key_evt(nygGetGuiInstance(),NGY_MSG_KEY_EVT_UP,ScanCodeToKeyCode[Getbit1Count(ScanCode-1)]);
	}
}

