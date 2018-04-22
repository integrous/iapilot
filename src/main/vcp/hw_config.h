/*
 * hw_config.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_VCP_HW_CONFIG_H_
#define SRC_MAIN_VCP_HW_CONFIG_H_

#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
//#include "platform_config.h"
#include "usb_type.h"
#ifdef STM32F303
#include "stm32f30x.h"
#endif

#ifdef STM32F10X
#include "stm32f10x.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define MASS_MEMORY_START     0x04002000
#define BULK_MAX_PACKET_SIZE  0x00000040

/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
#ifdef STM32F10X
void USB_Interrupts_Disable(void);
#endif
void USB_Cable_Config(FunctionalState NewState);
void Get_SerialNum(void);
uint32_t CDC_Send_DATA(const uint8_t *ptrBuffer, uint32_t sendLength);  // HJI
uint32_t CDC_Send_FreeBytes(void);
uint32_t CDC_Receive_DATA(uint8_t* recvBuf, uint32_t len);       // HJI
uint32_t CDC_Receive_BytesAvailable(void);

uint8_t usbIsConfigured(void);  // HJI
uint8_t usbIsConnected(void);   // HJI
uint32_t CDC_BaudRate(void);

/* External variables --------------------------------------------------------*/
extern __IO uint32_t packetSent;     // HJI

#endif  /*__HW_CONFIG_H*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



#endif /* SRC_MAIN_VCP_HW_CONFIG_H_ */

