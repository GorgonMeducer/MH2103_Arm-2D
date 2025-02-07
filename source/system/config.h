/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2024.06.30
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2024 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#ifndef	__CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
//////////////////////////////////////////////////////////////////////////

//#define DEBUG				//debug switch	 注意一点：由于复用了串口debug打开的时候就不能往flash下载数据。
//#define WATCHDOG_ENABLE	//if define,enable watchdog
/**************************************************************************
1. 1.0.0: release
**************************************************************************/
#define VERSION_MAJOR		1
#define VERSION_MINOR		0
#define VERSION_MICRO		0
#define VERSION_CODE(major, minor, micro)   ((((UINT16)major)<<8) | (((UINT16)minor)<<4) | ((UINT16)micro))
#define GET_CUR_VERSION()   VERSION_CODE(VERSION_MAJOR,VERSION_MINOR,VERSION_MICRO) 

/**************************************************************************
	设备硬件信息配置
**************************************************************************/
#define DEVICE_TYPE_XL_YIGUANG001		1		//yiguang project 001

#define DEVICE_TYPE		DEVICE_TYPE_XL_YIGUANG001


//设备类型选择
#if DEVICE_TYPE == DEVICE_TYPE_XL_YIGUANG001

#endif 


//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/************************END OF FILE*************************************/

