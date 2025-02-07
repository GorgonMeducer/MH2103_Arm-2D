/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2024.06.30
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2024 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#ifndef		__DATETYPE__
#define		__DATETYPE__
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>
#include	<ctype.h>

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
//=====================================================================

#ifdef	VAR_GLOBAL
	#define VAR_EXTERN
#else
	#define VAR_EXTERN extern
#endif

#define SYS_EXTERN extern
#ifndef bool
typedef unsigned int		bool;
#endif
typedef unsigned char		UINT8;			//8 bit unsigned number
typedef unsigned short int  UINT16;			//16 bit unsigned number
typedef unsigned int		UINT32;			//32 bit unsigned number
//typedef signed char		SINT8;			//8 bit signed number(-128-127)
typedef char				SINT8;			//8 bit char(0-127)
typedef signed short int   	SINT16;			//16 bit signed number
typedef signed int			SINT32;			//32 bit signed number
typedef float				FLOAT;			//32 bit
typedef double				DOUBLE;			//64 bit
typedef unsigned char		BOOL;

typedef unsigned char		BYTE;			//8 bit unsigned number
typedef unsigned short int  WORD;			//16 bit unsigned number
typedef unsigned int		DWORD;			//32 bit unsigned number
typedef signed char			SBYTE;			//8 bit signed number
typedef signed short int   	SWORD;			//16 bit signed number
typedef signed int			SDWORD;			//32 bit signed number

typedef unsigned int	UINT;

typedef unsigned short int WCHAR;
//确认实际的数据类型有正确的值
//技巧的原理:在定义一个数组的时候,编译器一般会根据平台的差异有一个最大长度的限制.
//x86一般在2GB,下面的定义如果出现表达是为0的情况,就会导致一个-1的长度,编译器在编
//译的时候就会提示错误,以此来确认数据长度定义.
#define GUI_COMPILE_TIME_ASSERT(name, x)               \
       typedef int GUI_dummy_##name[(x) * 2 - 1]
GUI_COMPILE_TIME_ASSERT(uint8, sizeof(UINT8) == 1);
GUI_COMPILE_TIME_ASSERT(sint8, sizeof(SINT8) == 1);
GUI_COMPILE_TIME_ASSERT(uint16, sizeof(UINT16) == 2);
GUI_COMPILE_TIME_ASSERT(sint16, sizeof(SINT16) == 2);
GUI_COMPILE_TIME_ASSERT(uint32, sizeof(UINT32) == 4);
GUI_COMPILE_TIME_ASSERT(sint32, sizeof(SINT32) == 4);
#undef GUI_COMPILE_TIME_ASSERT

/* 常用常数定义*/
#undef TRUE
#undef FALSE
#ifndef TRUE
	#define TRUE			1
	#define FALSE      		0
#endif

#define true			1
#define false			0

#undef NULL
#ifndef	NULL
	#define NULL        	((void *)0)
#endif

#define	ON				TRUE
#define OFF        		FALSE
#define HIGH       		TRUE
#define LOW        		FALSE
#define ENABLED    		TRUE
#define DISABLED   		FALSE
	
#define STR_NULL_ID        0xFFFFFFFF      //无效字符串id

#define INVALID_RES_ID		0xFFFFFFFF		//无效的资源ID
	

/* 一般性常用的 MACRO */
#define	MASK_BIT0				((UINT8)0x01)
#define	MASK_BIT1				((UINT8)0x02)
#define	MASK_BIT2				((UINT8)0x04)
#define	MASK_BIT3				((UINT8)0x08)
#define	MASK_BIT4				((UINT8)0x10)
#define	MASK_BIT5				((UINT8)0x20)
#define	MASK_BIT6				((UINT8)0x40)
#define	MASK_BIT7				((UINT8)0x80)
#define	MASK_BIT8				0x0100
#define	MASK_BIT9				0x0200
#define	MASK_BIT10				0x0400
#define	MASK_BIT11				0x0800
#define	MASK_BIT12				0x1000
#define	MASK_BIT13				0x2000
#define	MASK_BIT14				0x4000
#define	MASK_BIT15				0x8000
#define	MASK_BIT16				0x00010000UL
#define	MASK_BIT17				0x00020000UL
#define	MASK_BIT18				0x00040000UL
#define	MASK_BIT19				0x00080000UL
#define	MASK_BIT20				0x00100000UL
#define	MASK_BIT21				0x00200000UL
#define	MASK_BIT22				0x00400000UL
#define	MASK_BIT23				0x00800000UL
#define	MASK_BIT24				0x01000000UL
#define	MASK_BIT25				0x02000000UL
#define	MASK_BIT26				0x04000000UL
#define	MASK_BIT27				0x08000000UL
#define	MASK_BIT28				0x10000000UL
#define	MASK_BIT29				0x20000000UL
#define	MASK_BIT30				0x40000000UL
#define	MASK_BIT31				0x80000000UL

/* 取最小值,最大值 */
#undef MIN
#undef MAX
#undef ABS
#define	MIN(a, b)			(((a) < (b)) ? (a) : (b))
#define	MAX(a, b)			(((a) > (b)) ? (a) : (b))
#define ABS(X)				((X) > 0 ? (X) : -(X))

/*=====  结构定义 ===========*/
//8 bit 结构
typedef struct Bit16{

    UINT16   d0			: 1;            /* bit 0 */
    UINT16   d1			: 1;            /* bit 1 */
    UINT16   d2			: 1;            /* bit 2 */
    UINT16   d3			: 1;            /* bit 3 */
    UINT16   d4			: 1;            /* bit 4 */
    UINT16   d5			: 1;            /* bit 5 */
    UINT16   d6			: 1;            /* bit 6 */
    UINT16   d7			: 1;            /* bit 7 */
    UINT16   d8			: 1;            /* bit 9 */
    UINT16   d9			: 1;            /* bit 10 */
    UINT16   d10		: 1;            /* bit 11 */
    UINT16   d11		: 1;            /* bit 12 */
    UINT16   d12		: 1;            /* bit 13 */
    UINT16   d13		: 1;            /* bit 14 */
    UINT16   d14		: 1;            /* bit 15 */
    UINT16   d15		: 1;            /* bit 16 */
} Bit16;

//Byte , bit 的联合体
typedef union TUnionWord{

    UINT16  word;                   /* word */
    Bit16   bit;                    /* 16 bit 的结构 */
    struct{
    	UINT16	low			: 8;
    	UINT16	high		: 8;
	} byte;
} UnionWord;

#define    	WPARAM        	UINT32
#define    	LPARAM        	UINT32
typedef UINT32	(* WNDPROC)(WPARAM wParam, LPARAM lParam);		//窗口处理函数指针

#define LOBYTE(w)           ((BYTE)((w) & 0x00FF))		//返回word的低byte
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))						//返回word的高byte
#define MAKEWORD(high,low) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))	//组合两个byte为word
#define LOWORD(l)           ((WORD)(DWORD)(l))						//返回long的低word
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))	//返回long的高word
#define LOSWORD(l)          ((SWORD)(DWORD)(l))						//Returns the low signed word of the double word
#define HISWORD(l)          ((SWORD)((((DWORD)(l)) >> 16) & 0xFFFF))		//Returns the high signed word of the double word
#define MAKELONG(high,low) ((DWORD)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))//组合两个word为Dword

//求一个数组的元素数量
#define SIZEOF(x) 			(sizeof(x)/sizeof(x[0]))
	
//算出结构中项目的 offset
#define	STD_OFFSETOF(Struct, member)		((TWord)(&(((Struct *)0)->member)))
// 算出结构中某一个成员的size
#define	STD_SIZEOF(Struct, member)			sizeof(((Struct *)0x00000000L)->member)

#define ENABLE_INTERRUPT()			__enable_irq()		//enable global interrupt
#define DISABLE_INTERRUPT()			__disable_irq()		//disable global interrupt

#define PRAGMA_PACK __packed

//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/************************END OF FILE*************************************/

