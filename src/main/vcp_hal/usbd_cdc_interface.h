/*
 * usbd_cdc_interface.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_VCP_HAL_USBD_CDC_INTERFACE_H_
#define SRC_MAIN_VCP_HAL_USBD_CDC_INTERFACE_H_

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_IF_H
#define __USBD_CDC_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"
#include "stm32f7xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"

/* Definition for TIMx clock resources */
#define TIMusb                             TIM7
#define TIMx_IRQn                        TIM7_IRQn
#define TIMx_IRQHandler                  TIM7_IRQHandler
#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM7_CLK_ENABLE

/* Periodically, the state of the buffer "UserTxBuffer" is checked.
   The period depends on CDC_POLLING_INTERVAL */
#define CDC_POLLING_INTERVAL             10 /* in ms. The max is 65 and the min is 1 */

extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;

uint32_t CDC_Send_DATA(const uint8_t *ptrBuffer, uint32_t sendLength);
uint32_t CDC_Send_FreeBytes(void);
uint32_t CDC_Receive_DATA(uint8_t* recvBuf, uint32_t len);
uint32_t CDC_Receive_BytesAvailable(void);
uint8_t usbIsConfigured(void);
uint8_t usbIsConnected(void);
uint32_t CDC_BaudRate(void);

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif /* __USBD_CDC_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



#endif /* SRC_MAIN_VCP_HAL_USBD_CDC_INTERFACE_H_ */
