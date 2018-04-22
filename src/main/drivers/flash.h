/*
 * flash.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_FLASH_H_
#define SRC_MAIN_DRIVERS_FLASH_H_

#pragma once

#include <stdint.h>

typedef struct flashGeometry_s {
    uint16_t sectors; // Count of the number of erasable blocks on the device
    const uint16_t pageSize; // In bytes
    uint32_t sectorSize; // This is just pagesPerSector * pageSize
    uint32_t totalSize;  // This is just sectorSize * sectors
    uint16_t pagesPerSector;
} flashGeometry_t;



#endif /* SRC_MAIN_DRIVERS_FLASH_H_ */
