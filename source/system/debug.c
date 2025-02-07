/***********************************************************************
  [File]		debug.c
  [version]		1.00
  [author]		huangbin
  [date]		2022.05.05
 ----------------------------------------------------------------------
	COPYRIGHT 2014 SHENZHEN ZOOY TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#include "inc.h"

#ifdef DEBUG	
void DebugPrintf(SINT8* str,...)
{
	SINT8 txt[200];
	va_list args;

	va_start(args,str);
	vsprintf((char *)txt,(const char *)str,args);
	va_end(args);
	Uart_debug_send((UINT8*)txt,strlen(txt));
}
void DebugWriteHex(uint8_t *buf,uint32_t len)
{
	uint32_t i,block,remind;
	uint8_t buffer[16];
	block = len/16;
	remind = len%16;
	for(i = 0; i < block;i++)
	{
		memcpy(buffer,buf+i*16,16);
		DebugPrintf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \r\n",
					buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],
					buffer[8],buffer[9],buffer[10],buffer[11],buffer[12],buffer[13],buffer[14],buffer[15]);
	}
	if(remind)
	{
		memcpy(buffer,buf+block*16,remind);
		for(i = 0; i < remind;i++)
			DebugPrintf("%02x ",buffer[i]);
		DebugPrintf("\r\n");
	}
}
#endif
#if defined(__MICROLIB)
void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
{
    (void)chCond;
    (void)chLine;
    (void)wErrCode;
    
    while(1) {
    }
}
#else
//#pragma import(__use_no_semihosting)     //???????
#if (__ARMCC_VERSION >= 6010050)
__asm(".global __use_no_semihosting\n\t");
__asm(".global __ARM_use_no_argv\n\t");
#else
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

typedef int FILEHANDLE;
FILEHANDLE _sys_open(const char *name,int openmode)
{
 return 0;
}
int _sys_close(FILEHANDLE fh)
{
    return 0;
}
int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
{
    return 0;
}
int _sys_read(FILEHANDLE fh, unsigned char*buf, unsigned len, int mode)
{
    return 0;
}

int _sys_istty(FILEHANDLE fh)
{
    return 0;
}
int _sys_seek(FILEHANDLE fh, long pos)
{
    return 0;
}

int _sys_ensure(FILEHANDLE fh)
{
    return 0;
}

long _sys_flen(FILEHANDLE fh)
{
    return 0;
}
void _sys_exit(int status)
{
    //while(1);
}
int _sys_tmpnam(char *name, int fileno, unsigned maxlength)
{
    return 0;
}

void _ttywrch(int ch)
{
}
int remove(const char *filename)
{
    return 0;
}
char *_sys_command_string(char *cmd, int len)
{
 return NULL;
}

void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
{
    (void)chCond;
    (void)chLine;
    (void)wErrCode;
	DebugPrintf("arm-2d assert: %s,%s,%d\r\n",chCond,chLine,wErrCode);
    while(1) {
			
    }
}
int fputc(int ch,FILE *p)
{
	return 0;
}
//int64_t clock(void)
//{
//    return 0;
//}
#endif
