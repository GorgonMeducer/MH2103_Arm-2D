/***********************************************************************
  [Version]		1.00
  [Author ]		huangbin
  [Date   ]		2022.5.23
  [E-mail ]		huangembed@163.com 
  [  Q Q  ]		405803243
************************************************************************/
#include "inc.h"
//异常应答码

MODBUS_RTU mbRtu;
MODBUS_CONFIG mbCfg;
#define mbUartSend UartDebugSend

//void mbCmd01(uint8_t *p,uint16_t len);//01功能码:读线圈状态(指的是输出继电器)
//void mbCmd02(uint8_t *p,uint16_t len);//02功能码:读离散量输入状态(指的是输入IO的状态)
//void mbCmd03(uint8_t *p,uint16_t len);//03功能码:读保持寄存器
//void mbCmd04(uint8_t *p,uint16_t len);//04功能码:读输入寄存器
//void mbCmd05(uint8_t *p,uint16_t len);//05功能码:写单个线圈
//void mbCmd06(uint8_t *p,uint16_t len);//06功能码：写单个寄存器
//void mbCmd15(uint8_t *p,uint16_t len);//(15)0x0F:写多个线圈
//void mbCmd16(uint8_t *p,uint16_t len);//(16)0x10:写多个寄存器
void mbCmd100(uint8_t *p,uint16_t len);//100:写flash
void mbCmd101(uint8_t *p,uint16_t len);//101:读flash
void mbCmd102(uint8_t *p,uint16_t len);//102:reboot
//异常应答
void mbExceptionRsp(EX_CODE ex_code);

typedef void	(* ModbusCmdProcess)(uint8_t *p,uint16_t len);		//命令处理函数
const ModbusCmdProcess mbFunction[] = {
//	NULL,mbCmd01,mbCmd02,mbCmd03,mbCmd04,mbCmd05,mbCmd06,NULL, 	//0x00-0x07
//	NULL,NULL,NULL,NULL,NULL,NULL,NULL,mbCmd15,				//0x08-0x0f
//	mbCmd16,NULL,NULL,NULL,NULL,NULL,NULL,NULL,			//0x10-0x17
	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,			//0x18-0x1f
};
//用户功能码
const ModbusCmdProcess mbFunction100_110[] = {
	mbCmd100,mbCmd101,mbCmd102,NULL,NULL,NULL,NULL,NULL 	//0x00-0x07
};
//这里被中断调用，会有临界问题？
extern void mbCommandNotify(void);
uint16_t mbCopyDataToRtu(uint8_t *pBuf,uint16_t len)
{
	if((pBuf[0] != mbCfg.DevAddr) && (pBuf[0] != 0))	//不是本机地址，也不是广播地址
		return FALSE;
	memcpy(mbRtu.RxBuf,pBuf,len);
	mbRtu.Total = len;
//	mbCommandNotify();
	return TRUE;
}
void mbConfigInit(void)
{
	mbCfg.DevAddr = 0x80;
}
//解码命令
void mbDecodeData(void)
{
	uint16_t crc;
	mbRtu.devAddr	= mbRtu.RxBuf[0];
	mbRtu.funCode	= mbRtu.RxBuf[1];
	if(mbRtu.Total < 4)	//发生错误
	{
		DebugPrintf("Modbus RX error: %d\r\n",mbRtu.Total);
		return;
	}
	mbRtu.payloadLen	= mbRtu.Total - 4; //addr+code+crc16
	mbRtu.crc	= MAKEWORD(mbRtu.RxBuf[mbRtu.Total - 1],mbRtu.RxBuf[mbRtu.Total - 2]);
	crc = mbCRC16(mbRtu.RxBuf,mbRtu.Total - 2);
	if(crc != mbRtu.crc)
	{
		mbExceptionRsp(EX_CODE_03);
		return;
	}
	if((mbRtu.funCode > 100-1) && (mbRtu.funCode <= 110))
	{
		uint16_t userCode = mbRtu.funCode - 100;
		if(mbFunction100_110[userCode] != NULL)
			mbFunction100_110[userCode](&mbRtu.RxBuf[2],mbRtu.payloadLen);
		else
			mbExceptionRsp(EX_CODE_01);
	}
	else if(mbRtu.funCode < SIZEOF(mbFunction)) //不能超过处理表
	{
		if(mbFunction[mbRtu.funCode] != NULL)
			mbFunction[mbRtu.funCode](&mbRtu.RxBuf[2],mbRtu.payloadLen);
		else
			mbExceptionRsp(EX_CODE_01);
	}
	else
		mbExceptionRsp(EX_CODE_01);
}
//异常应答
void mbExceptionRsp(EX_CODE ex_code)
{
	uint16_t crc;
	
	mbRtu.TxBuf[0] = mbRtu.devAddr;			//device address
	mbRtu.TxBuf[1] = mbRtu.funCode | 0x80; 	//error
	mbRtu.TxBuf[2] = ex_code; 				//exception code
	crc = mbCRC16(mbRtu.TxBuf,3);
	mbRtu.TxBuf[3] = (uint8_t)(crc & 0xff); //low byte
	mbRtu.TxBuf[4] = (uint8_t)(crc >> 8);	  //high byte
	DebugPrintf("mbExceptionRsp code: %d\r\n",ex_code);

	mbUartSend(mbRtu.TxBuf,3 + 2);
	
}
//正常接收应答
void mbCmdResponseWithLen(uint8_t *pdata,uint16_t len)
{
	uint16_t crc;
	
	mbRtu.TxBuf[0] = mbRtu.devAddr;	//device address
	mbRtu.TxBuf[1] = mbRtu.funCode;	//function code
	mbRtu.TxBuf[2] = len; //有长度为0的应答？
	if((pdata != NULL) && len)
	{
		memcpy(&mbRtu.TxBuf[3],pdata,len);
	}
	crc = mbCRC16(mbRtu.TxBuf,len + 3);
	mbRtu.TxBuf[len + 3] = (uint8_t)(crc & 0xff); //low byte
	mbRtu.TxBuf[len + 4] = (uint8_t)(crc >> 8);	  //high byte

//	DebugWriteHex(mbRtu.TxBuf,len + 3 + 2 + 1);
	mbUartSend(mbRtu.TxBuf,len + 3 + 2);
}
//正常接收应答,只返回数据，不带数据长度指示(数据域有长度字节)
void mbCmdResponse(uint8_t *pdata,uint16_t len)
{
	uint16_t crc;
	
	mbRtu.TxBuf[0] = mbRtu.devAddr;	//device address
	mbRtu.TxBuf[1] = mbRtu.funCode;	//function code
	if((pdata != NULL) && len)
	{
		memcpy(&mbRtu.TxBuf[2],pdata,len);
	}
	crc = mbCRC16(mbRtu.TxBuf,len + 2);
	mbRtu.TxBuf[len + 2] = (uint8_t)(crc & 0xff); //low byte
	mbRtu.TxBuf[len + 3] = (uint8_t)(crc >> 8);	  //high byte

	mbUartSend(mbRtu.TxBuf,len + 2 + 2);
}
//////////////////////////////////////功能码响应函数////////////////////////////////////////////
uint8_t FlashBuf[MDTX_MAX]; //flash操作的缓存，最小要保证256
//100:写flash数据
//in： 功能码+flash地址4bytes+数据长度2bytes+数据
//out：功能码+flash地址4bytes+数据长度2bytes
void mbCmd100(uint8_t *p,uint16_t len)
{
	uint32_t addr;
	uint16_t length;
	
	addr = MAKELONG(MAKEWORD(p[0],p[1]),MAKEWORD(p[2],p[3]));
	length = MAKEWORD(p[4],p[5]);
	if(length > sFLASH_SPI_SECTOR_SIZE)
	{
		mbExceptionRsp(EX_CODE_03);
		return;
	}
	if((addr % sFLASH_SPI_SECTOR_SIZE) == 0) //地址在一个扇区起始地址，先擦除扇区
		sFLASH_EraseSector(addr);
	if((addr /sFLASH_SPI_SECTOR_SIZE)  != ((addr+length) /sFLASH_SPI_SECTOR_SIZE) ) //跨扇区
		sFLASH_EraseSector(addr + length);
	sFLASH_WriteBuffer(p+6,addr,length);
	mbCmdResponse(p,6);		//返回表示向xx地址写了多少数据
}
//101:读flash数据)
//in： 功能码+flash地址4bytes+数据长度2bytes
//out：功能码+数据长度2bytes+数据
void mbCmd101(uint8_t *p,uint16_t len)
{
	uint32_t addr;
	uint16_t length;
	
	addr = MAKELONG(MAKEWORD(p[0],p[1]),MAKEWORD(p[2],p[3]));
	length = MAKEWORD(p[4],p[5]);
	if(length > MDTX_MAX - 16)	//预留16bytes
	{
		mbExceptionRsp(EX_CODE_03);
		return;
	}
	sFLASH_ReadBuffer(FlashBuf,addr,length);
	mbCmdResponseWithLen(FlashBuf,length);
}
//#define FLASH_PROGRAM_ADDR 0x400000 - (64*1024*4) //最后256K用来下载程序,0x3C0000
bool VerifyChecksum8(uint32_t start);
bool VerifyChecksum32(uint32_t start);
//reboot
extern void SetSoftwareUpdateFlag(void);
void mbCmd102(uint8_t *p,uint16_t len)
{
	volatile uint32_t addr;
	uint8_t flag1,flag2;
	
	addr = MAKELONG(MAKEWORD(p[0],p[1]),MAKEWORD(p[2],p[3]));
	flag1 = p[4];
	flag2 = p[5];
	if((flag1 != 'H') || (flag2 != 'B'))
	{
		return;
	}
//	if(VerifyChecksum(addr)) //最后256K用来下载程序
	if(VerifyChecksum32(addr)) //最后256K用来下载程序
	{
		SetSoftwareUpdateFlag();
		mbCmdResponse(p,6);
		DelayMs(500); //debug=5000,releas=500
		NVIC_SystemReset();
	}
	else
		mbExceptionRsp(EX_CODE_03);
}
bool VerifyChecksum8(uint32_t start)
{
	uint32_t addr = start;
	uint32_t len,checksum;
	uint32_t sum = 0;
	uint8_t *p = FlashBuf;
	
	sFLASH_ReadBuffer(p,addr,8);
	len = MAKELONG(MAKEWORD(p[0],p[1]),MAKEWORD(p[2],p[3]));
	checksum = MAKELONG(MAKEWORD(p[4],p[5]),MAKEWORD(p[6],p[7]));
	addr += 8;
	while(len)
	{
		uint32_t readbytes;
		if(len >= MDTX_MAX)
		{
			readbytes = MDTX_MAX;
			len -= MDTX_MAX;
		}
		else
		{
			readbytes = len;
			len = 0;
		}
		
		sFLASH_ReadBuffer(FlashBuf,addr,readbytes);
		addr += readbytes;
		for(uint32_t m = 0;m < readbytes;m++)
			sum += FlashBuf[m];
	}
	if(checksum != sum)
		return false;
	return true;
}
//上位机要把升级文件凑成256的倍数(不够的填0)，校验和用32bit的小端模式数据累加
bool VerifyChecksum32(uint32_t start)
{
	uint32_t addr = start;
	uint32_t len,checksum,page;
	uint32_t sum = 0;
	uint8_t *p = FlashBuf;

	
	sFLASH_ReadBuffer(p,addr,8);
	len = MAKELONG(MAKEWORD(p[0],p[1]),MAKEWORD(p[2],p[3]));
	checksum = MAKELONG(MAKEWORD(p[4],p[5]),MAKEWORD(p[6],p[7]));
	if(len > 1024 * 256) //flash实际没有写进去数据或者写入的是错误数据，非代码
	{
		return false;
	}
	addr += 256; //前面空了一页
	page = len / 256;
	while(page)
	{
		uint32_t readbytes = 256;
	
		sFLASH_ReadBuffer(FlashBuf,addr,readbytes);
		addr += readbytes;
		uint32_t *p32 = (uint32_t*)FlashBuf;
		for(uint32_t m = 0;m < readbytes / 4;m++)
			sum += *p32++;
		page--;
	}
	if(checksum != sum)
		return false;
	return true;
}




