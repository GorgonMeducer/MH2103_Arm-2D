/**
  ******************************************************************************
  * @file    GPIO/IOToggle/mh210x_it.c 
  * @author  NONE
  * @version NONE
  * @date    NONE
  * @brief   NONE
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "mh210x_it.h" 


 
void NMI_Handler(void)
{
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
/******************************************************************************/
/*                 mh210x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_mh210x_xx.s).                                            */
/******************************************************************************/
