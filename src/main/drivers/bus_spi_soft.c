/*
 * bus_spi_soft.c
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#include <stdbool.h>
#include <stdint.h>

#include <platform.h>

#ifdef USE_SOFTSPI

#include "build/build_config.h"


#include "drivers/io.h"
#include "io_impl.h"
#include "drivers/bus_spi.h"
#include "bus_spi_soft.h"


void softSpiInit(const softSPIDevice_t *dev)
{
    // SCK as output
    IOInit(IOGetByTag(dev->sckTag),  OWNER_SPI_SCK,  RESOURCE_INDEX(SOFT_SPIDEV_1) + 10);
if defined(STM32F3)
    IOConfigGPIOAF(IOGetByTag(dev->sckTag), SPI_IO_AF_CFG, 0);
#endif

    // MOSI as output
    IOInit(IOGetByTag(dev->mosiTag),  OWNER_SPI_MOSI,  RESOURCE_INDEX(SOFT_SPIDEV_1) + 10);
if defined(STM32F3)
    IOConfigGPIOAF(IOGetByTag(dev->mosiTag), SPI_IO_AF_CFG, 0);
#endif

    // MISO as input
    IOInit(IOGetByTag(dev->misoTag),  OWNER_SPI_MISO,  RESOURCE_INDEX(SOFT_SPIDEV_1) + 10);
if defined(STM32F3)
    IOConfigGPIOAF(IOGetByTag(dev->misoTag), SPI_IO_AF_CFG, 0);
#endif

    // NSS as output
    if (dev->nssTag != IOTAG_NONE) {
        IOInit(IOGetByTag(dev->nssTag),  OWNER_SPI_CS,  RESOURCE_INDEX(SOFT_SPIDEV_1) + 10);
if defined(STM32F3)
        IOConfigGPIOAF(IOGetByTag(dev->nssTag), SPI_IO_AF_CFG, 0);
#endif
    }
}

uint8_t softSpiTransferByte(const softSPIDevice_t *dev, uint8_t byte)
{
    for (int ii = 0; ii < 8; ++ii) {
        if (byte & 0x80) {
            IOHi(IOGetByTag(dev->mosiTag));
        } else {
            IOLo(IOGetByTag(dev->mosiTag));
        }
        IOHi(IOGetByTag(dev->sckTag));
        byte <<= 1;
        if (IORead(IOGetByTag(dev->misoTag)) == 1) {
            byte |= 1;
        }
        IOLo(IOGetByTag(dev->sckTag));
    }
    return byte;
}
#endif
