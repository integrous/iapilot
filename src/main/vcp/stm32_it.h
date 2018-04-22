/*
 * stm32_it.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_VCP_STM32_IT_H_
#define SRC_MAIN_VCP_STM32_IT_H_

#ifndef __STM32_IT_H
#define __STM32_IT_H

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USBWakeUp_IRQHandler(void);
void USB_FS_WKUP_IRQHandler(void);

#endif /* __STM32_IT_H */



#endif /* SRC_MAIN_VCP_STM32_IT_H_ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
