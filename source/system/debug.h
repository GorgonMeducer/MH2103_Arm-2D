/***********************************************************************
  [version]		1.00
  [author]		huangbin
  [date]		2024.06.30
  [email]		huangbinrd@163.com
 ----------------------------------------------------------------------
	COPYRIGHT 2024 SHENZHEN HB TECHNOLOGY DEVELOPMENT CO.,LTD. 
************************************************************************/
#ifndef __DEBUG_H__
#define	__DEBUG_H__

#ifdef DEBUG
	extern void DebugPrintf(SINT8* str,...);
	extern void DebugWriteHex(uint8_t *buf,uint32_t len);
#else
	#define DebugPrintf(...)
	#define DebugWriteHex(buf,len)
#endif
#define RedBold             "\033[31;1m" // 红色加粗
#define RedBoldBlink        "\033[31;1;5m" // 红色加粗、闪烁
#define GreenBold             "\033[32;1m" // 绿色加粗
#define GreenBoldBlink        "\033[32;1;5m" // 绿色加粗、闪烁
#define YellowBold             "\033[33;1m" // 黄色加粗
#define YellowBoldBlink        "\033[33;1;5m" // 黄色加粗、闪烁
#define BlueBold            "\033[34;1m" // 蓝色加粗
#define BlueBoldBlink       "\033[34;1;5m" // 蓝色加粗、闪烁
#define PurpleBold          "\033[35;1m"   // 紫色加粗
#define PurpleBoldBlink     "\033[35;1;5m" // 紫色加粗、闪烁
#define DGreenBold             "\033[36;1m" // 深绿色加粗
#define DGreenBoldBlink        "\033[36;1;5m" // 红深绿色加粗、闪烁
#define WhiteBold             "\033[37;1m" // 白色加粗
#define WhiteBoldBlink        "\033[37;1;5m" // 白色加粗、闪烁
#define ColorReset               "\033[0;25m"   // 颜色复位

#if USE_STD_PRINTF
#define LOGO_DEBUG(fmt,...) printf(fmt,##__VA_ARGS__)
#else
#define LOGO_DEBUG(fmt,...) _printf(fmt,##__VA_ARGS__)
#endif

#if CONFIG_CI_LOG_EN
/*need open SecureCRT ansi color*/
#define CI_LOG(debug, comlevel, message, args...)                                          \
    do {                                                                                   \
        if((debug)<=(comlevel)) {                                                          \
            if((debug) & (CI_LOG_ASSERT))LOGO_DEBUG("\033[35m"message"\033[0m", ## args);   \
            if((debug) & (CI_LOG_ERROR))LOGO_DEBUG("\033[31m"message"\033[0m", ## args);    \
            if((debug) & (CI_LOG_WARN))LOGO_DEBUG("\033[33m"message"\033[0m", ## args);     \
            if((debug) & (CI_LOG_INFO))LOGO_DEBUG(message, ## args);                        \
            if((debug) & (CI_LOG_DEBUG))LOGO_DEBUG("\033[36m"message"\033[0m", ## args);    \
            if((debug) & (CI_LOG_VERBOSE))LOGO_DEBUG("\033[32m"message"\033[0m", ## args);  \
        }                                                                                  \
    } while(0)
#else
#define CI_LOG(debug, message, args...)   do{}while(0)           
#endif
	
/* log打印等级 参考Andriod Logcat */
#define LOG_VERBOSE             (1 << 6) 	//详细
#define LOG_DEBUG               (1 << 5)	//调试
#define LOG_INFO                (1 << 4)	//信息
#define LOG_WARN                (1 << 3)	//警告
#define LOG_ERROR               (1 << 2)	//错误
#define LOG_ASSERT              (1 << 1)	//断言
#define LOG_NONE                (1 << 0)	//调试关闭
/** @brief 日志打印--详细 */
#define ci_logverbose(comlevel, message, args...) CI_LOG(LOG_VERBOSE, comlevel, message, ## args)
/** @brief 日志打印--调试 */
#define ci_logdebug(comlevel, message, args...)   CI_LOG(LOG_DEBUG, comlevel, message, ## args)
/** @brief 日志打印--信息 */
#define ci_loginfo(comlevel, message, args...)    CI_LOG(LOG_INFO, comlevel, message, ## args)
/** @brief 日志打印--警告 */
#define ci_logwarn(comlevel, message, args...)    CI_LOG(LOG_WARN, comlevel, message, ## args)
/** @brief 日志打印--错误 */
#define ci_logerr(comlevel, message, args...)     CI_LOG(LOG_ERROR, comlevel, message, ## args)
/** @brief 日志打印--断言 */
#define ci_logassert(comlevel, message, args...)  CI_LOG(LOG_ASSERT, comlevel, message, ## args)

#ifdef LOG_INFO_ENABLE
	#define log_info(format, ...) PRINTF("[Info]:" format "\r\n", ##__VA_ARGS__)
#else
	#define log_info(...)
#endif

#ifdef LOG_DEBUG_ENABLE
	#define log_debug(format, ...) PRINTF("[Debug]:" format "\r\n", ##__VA_ARGS__)
#else
	#define log_debug(...)
#endif

#ifdef LOG_ERROR_ENABLE
	#define log_error(format, ...) PRINTF("<Error>:" format "\r\n", ##__VA_ARGS__)
#else
	#define log_error(...)
#endif




#endif 

