/*
 * flash_m25p16.c
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

void m25p16_pageProgram(uint32_t address, const uint8_t *data, int length)
{
    m25p16_pageProgramBegin(address);

    m25p16_pageProgramContinue(data, length);

    m25p16_pageProgramFinish();
}

/**
 * Read `length` bytes into the provided `buffer` from the flash starting from the given `address` (which need not lie
 * on a page boundary).
 *
 * Waits up to DEFAULT_TIMEOUT_MILLIS milliseconds for the flash to become ready before reading.
 *
 * The number of bytes actually read is returned, which can be zero if an error or timeout occurred.
 */
int m25p16_readBytes(uint32_t address, uint8_t *buffer, int length)
{
    uint8_t command[5] = { M25P16_INSTRUCTION_READ_BYTES };

    m25p16_setCommandAddress(&command[1], address, isLargeFlash);

    if (!m25p16_waitForReady(DEFAULT_TIMEOUT_MILLIS)) {
        return 0;
    }

    m25p16_enable(bus);

    spiTransfer(bus->busdev_u.spi.instance, command, NULL, isLargeFlash ? 5 : 4);
    spiTransfer(bus->busdev_u.spi.instance, NULL, buffer, length);

    m25p16_disable(bus);

    return length;
}

/**
 * Fetch information about the detected flash chip layout.
 *
 * Can be called before calling m25p16_init() (the result would have totalSize = 0).
 */
const flashGeometry_t* m25p16_getGeometry(void)
{
    return &geometry;
}

#endif
