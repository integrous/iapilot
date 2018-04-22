/*
 * exti.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_DRIVERS_EXTI_H_
#define SRC_MAIN_DRIVERS_EXTI_H_

#pragma once

#include <stdbool.h>

#include "drivers/io_types.h"

typedef struct extiCallbackRec_s extiCallbackRec_t;
typedef void extiHandlerCallback(extiCallbackRec_t *self);

struct extiCallbackRec_s {
    extiHandlerCallback *fn;
};

void EXTIInit(void);

void EXTIHandlerInit(extiCallbackRec_t *cb, extiHandlerCallback *fn);

void EXTIConfig(IO_t io, extiCallbackRec_t *cb, int irqPriority, EXTITrigger_TypeDef trigger);
void EXTIRelease(IO_t io);
void EXTIEnable(IO_t io, bool enable);



#endif /* SRC_MAIN_DRIVERS_EXTI_H_ */
