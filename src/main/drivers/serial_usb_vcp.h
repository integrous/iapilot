/*
 * serial_usb_vcp.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_SERIAL_USB_VCP_H_
#define SRC_MAIN_DRIVERS_SERIAL_USB_VCP_H_

#pragma once

typedef struct {
    serialPort_t port;

    // Buffer used during bulk writes.
    uint8_t txBuf[20];
    uint8_t txAt;
    // Set if the port is in bulk write mode and can buffer.
    bool buffering;
} vcpPort_t;

serialPort_t *usbVcpOpen(void);
struct serialPort_s;
uint32_t usbVcpGetBaudRate(struct serialPort_s *instance);



#endif /* SRC_MAIN_DRIVERS_SERIAL_USB_VCP_H_ */
