/*
 * rx_spi.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_RX_RX_SPI_H_
#define SRC_MAIN_DRIVERS_RX_RX_SPI_H_

#pragma once

#include <stdint.h>

typedef enum {
    RX_SPI_SOFTSPI,
    RX_SPI_HARDSPI
} rx_spi_type_e;

#define RX_SPI_MAX_PAYLOAD_SIZE 32

void rxSpiDeviceInit(rx_spi_type_e spiType);
uint8_t rxSpiTransferByte(uint8_t data);
uint8_t rxSpiWriteByte(uint8_t data);
uint8_t rxSpiWriteCommand(uint8_t command, uint8_t data);
uint8_t rxSpiWriteCommandMulti(uint8_t command, const uint8_t *data, uint8_t length);
uint8_t rxSpiReadCommand(uint8_t command, uint8_t commandData);
uint8_t rxSpiReadCommandMulti(uint8_t command, uint8_t commandData, uint8_t *retData, uint8_t length);



#endif /* SRC_MAIN_DRIVERS_RX_RX_SPI_H_ */
