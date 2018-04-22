/*
 * usb_io.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_USB_IO_H_
#define SRC_MAIN_DRIVERS_USB_IO_H_

#pragma once

void usbGenerateDisconnectPulse(void);

void usbCableDetectDeinit(void);
void usbCableDetectInit(void);
bool usbCableIsInserted(void);



#endif /* SRC_MAIN_DRIVERS_USB_IO_H_ */
