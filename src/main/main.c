/*
 * main.c
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#include <stdbool.h>
#include <stdint.h>

#include "platform.h"

#include "fc/fc_init.h"

#include "scheduler/scheduler.h"

int main(void)
{
    init();
    while (true) {
        scheduler();
        processLoopback();
#ifdef SIMULATOR_BUILD
        delayMicroseconds_real(50); // max rate 20kHz
#endif
    }
    return 0;
}
