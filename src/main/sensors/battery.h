/*
 * battery.h
 *
 *  Created on: 2018. 4. 19.
 *      Author: kangju
 */

#ifndef SRC_MAIN_SENSORS_BATTERY_H_
#define SRC_MAIN_SENSORS_BATTERY_H_

#pragma one

#include "pg/pg.h"

#include "common/filter.h"
#include "common/time.h"
#include "sensors/current.h"
#include "sensors/voltage.h"

typedef struct batteryConfig_s{
	uint8_t vbatmaxcellvoltage;
	uint8_t vbatmincellvoltage;
	uint8_t vbatwarningcellvoltage;
	uint8_t vbatnotpresentcellvoltage;
	uint8_t IvcPercentage;
	voltageMeterSource_e voltageMeterSource;
	currentMeterSource_e currentMeterSource;
	uint16_t batteryCapacity;
	bool useVBatAlerts;
	bool useConsumptionAlerts;
	uint8_t consumptionWarningPercentage;
	uint8_t vbathysteresis;
	uint8_t vbatfullcellvoltage;
}batteryConfig_t;

typedef struct lowVoltageCutoff_s{
	bool enabled;
	uint8_t percentage;
	timeUs_t startTime;
}lowVoltageCutoff_t;

PG_DECLARE(batteryConfig_t, batteryConfig);

typedef enum{
	BATTERY_OK = 0,
	BATTERY_WARNING,
	BATTERY_CRITICAL,
	BATTERY_NOT_PRESENT,
	BATTERY_INIT
}batteryState_e;

void batteryInit(void);
void batteryUpdateVoltage(timeUs_t currentTimeUs);
void batteryUpdatePresence(void);

batteryState_e getBatteryState(void);
const char * getBatteryStateString(void);

void batteryUpdateStates(timeUs_t currentTimeUs);
void batteryUpdateAlarms(void);

struct rxConfig_s;

float calculateVbatPidCompensation(void);
uint8_t calculateBatteryPercentageRemaining(void);
bool isBatteryVoltageConfigured(void);
uint16_t getBatteryVoltage(void);
uint16_t getBatteryVoltageLatest(void);
uint8_t getBatteryCellCount(void);
uint16_t getBatteryAverageCellVoltage(void);

bool isAmperageConfigured(void);
int32_t getAmperage(void);
int32_t getAmperageLatest(void);
int32_t getMAhDrawn(void);

void batteryUpdateCurrentMeter(timeUs_t currentTimeUs);

const lowVoltageCutoff_t *getLowVoltageCutoff(void);
};


#endif /* SRC_MAIN_SENSORS_BATTERY_H_ */
