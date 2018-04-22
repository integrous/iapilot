/*
 * platform_config.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_VCP_PLATFORM_CONFIG_H_
#define SRC_MAIN_VCP_PLATFORM_CONFIG_H_

#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#if defined (STM32F10X_MD) || defined (STM32F10X_HD)
#include "stm32f10x.h"
#elif defined (STM32F303xC)
#include "stm32f30x.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/*Unique Devices IDs register set*/

#if defined (STM32F37X) || defined(STM32F303xC)

#define         ID1          (0x1FFFF7AC)
#define         ID2          (0x1FFFF7B0)
#define         ID3          (0x1FFFF7B4)

#else /*STM32F1x*/

#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)

#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */



#endif /* SRC_MAIN_VCP_PLATFORM_CONFIG_H_ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
