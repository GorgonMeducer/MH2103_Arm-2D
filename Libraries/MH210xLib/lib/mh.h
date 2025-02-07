/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MH_H
#define __MH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mh210x.h"

uint32_t MH_RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul, uint8_t Latency);
uint32_t MH_LSE_DriveConfig(uint8_t gear);
uint32_t MH_PLL_SelLockConfig(uint8_t mode);
uint32_t MH_PLL_CpConfig(uint8_t mode);

#endif

