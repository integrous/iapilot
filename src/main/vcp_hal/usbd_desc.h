/*
 * usbd_desc.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_VCP_HAL_USBD_DESC_H_
#define SRC_MAIN_VCP_HAL_USBD_DESC_H_

#ifndef __USBD_DESC_H
#define __USBD_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define  USB_SIZ_STRING_SERIAL       0x1A
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern USBD_DescriptorsTypeDef VCP_Desc;

#endif /* __USBD_DESC_H */



#endif /* SRC_MAIN_VCP_HAL_USBD_DESC_H_ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
