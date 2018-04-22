/*
 * usb_endp.c
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define VCOMPORT_IN_FRAME_INTERVAL             5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t packetSent;         // HJI
extern __IO uint8_t receiveBuffer[64];  // HJI
__IO uint32_t receiveLength;             // HJI
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * Function Name  : EP1_IN_Callback
 * Description    :
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/

void EP1_IN_Callback(void)
{
    packetSent = 0;  // HJI
}

/*******************************************************************************
 * Function Name  : EP3_OUT_Callback
 * Description    :
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void EP3_OUT_Callback(void)
{
    receiveLength = GetEPRxCount(ENDP3);                                              // HJI
    PMAToUserBufferCopy((unsigned char*)receiveBuffer, ENDP3_RXADDR, receiveLength);  // HJI
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
