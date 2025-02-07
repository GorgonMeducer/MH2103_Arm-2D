/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MH210x_IT_H
#define __MH210x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "mh210x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif /* __mh210x_IT_H */

/******************* (C) COPYRIGHT  MHSEMICONDUCTOR *****END OF FILE****/
