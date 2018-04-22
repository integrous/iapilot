/*
 * scheduler.h
 *
 *  Created on: 2018. 4. 20.
 *      Author: kangju
 */

#ifndef SRC_MAIN_SCHELDULER_SCHEDULER_H_
#define SRC_MAIN_SCHELDULER_SCHEDULER_H_

#pragma once

#include "common/time.h"

#define TASK_PERIOD_HZ(hz) (1000000 / (hz))
#define TASK_PERIOD_MS(ms) ((ms) * 1000)
#define TASK_PERIOD_US(us) (us)


typedef enum {
    TASK_PRIORITY_IDLE = 0,     // Disables dynamic scheduling, task is executed only if no other task is active this cycle
    TASK_PRIORITY_LOW = 1,
    TASK_PRIORITY_MEDIUM = 3,
    TASK_PRIORITY_MEDIUM_HIGH = 4,
    TASK_PRIORITY_HIGH = 5,
    TASK_PRIORITY_REALTIME = 6,
    TASK_PRIORITY_MAX = 255
} cfTaskPriority_e;

typedef struct {
    timeUs_t     maxExecutionTime;
    timeUs_t     totalExecutionTime;
    timeUs_t     averageExecutionTime;
} cfCheckFuncInfo_t;

typedef struct {
    const char * taskName;
    const char * subTaskName;
    bool         isEnabled;
    uint8_t      staticPriority;
    timeDelta_t  desiredPeriod;
    timeDelta_t  latestDeltaTime;
    timeUs_t     maxExecutionTime;
    timeUs_t     totalExecutionTime;
    timeUs_t     averageExecutionTime;
} cfTaskInfo_t;

typedef enum {
    /* Actual tasks */
    TASK_SYSTEM = 0,
    TASK_GYROPID,
    TASK_ACCEL,
    TASK_ATTITUDE,
    TASK_RX,
    TASK_SERIAL,
    TASK_DISPATCH,
    TASK_BATTERY_VOLTAGE,
    TASK_BATTERY_CURRENT,
    TASK_BATTERY_ALERTS,
#ifdef BEEPER
    TASK_BEEPER,
#endif
#ifdef USE_GPS
    TASK_GPS,
#endif
#ifdef USE_MAG
    TASK_COMPASS,
#endif
#ifdef USE_BARO
    TASK_BARO,
#endif

#if defined(USE_BARO) || defined(USE_RANGEFINDER)
    TASK_ALTITUDE,
#endif
#ifdef USE_DASHBOARD
    TASK_DASHBOARD,
#endif
#ifdef USE_TELEMETRY
    TASK_TELEMETRY,
#endif
#ifdef USE_LED_STRIP
    TASK_LEDSTRIP,
#endif
#ifdef USE_TRANSPONDER
    TASK_TRANSPONDER,
#endif
#ifdef STACK_CHECK
    TASK_STACK_CHECK,
#endif
#endif
#ifdef USE_BST
    TASK_BST_MASTER_PROCESS,
#endif
#ifdef USE_ESC_SENSOR
    TASK_ESC_SENSOR,
#endif
#ifdef USE_CMS
    TASK_CMS,
#endif
#ifdef USE_VTX_CONTROL
    TASK_VTXCTRL,
#endif
#ifdef USE_CAMERA_CONTROL
    TASK_CAMCTRL,
#endif

#ifdef USE_RCDEVICE
    TASK_RCDEVICE,
#endif

#ifdef USE_ADC_INTERNAL
    TASK_ADC_INTERNAL,
#endif

#ifdef USE_PINIOBOX
    TASK_PINIOBOX,
#endif

    /* Count of real tasks */
    TASK_COUNT,

    /* Service task IDs */
    TASK_NONE = TASK_COUNT,
    TASK_SELF
} cfTaskId_e;

typedef struct {
    // Configuration
    const char * taskName;
    const char * subTaskName;
    bool (*checkFunc)(timeUs_t currentTimeUs, timeDelta_t currentDeltaTimeUs);
    void (*taskFunc)(timeUs_t currentTimeUs);
    timeDelta_t desiredPeriod;      // target period of execution
    const uint8_t staticPriority;   // dynamicPriority grows in steps of this size, shouldn't be zero

    // Scheduling
    uint16_t dynamicPriority;       // measurement of how old task was last executed, used to avoid task starvation
    uint16_t taskAgeCycles;
    timeDelta_t taskLatestDeltaTime;
    timeUs_t lastExecutedAt;        // last time of invocation
    timeUs_t lastSignaledAt;        // time of invocation event for event-driven tasks

#ifndef SKIP_TASK_STATISTICS
    // Statistics
    timeUs_t movingSumExecutionTime;  // moving sum over 32 samples
    timeUs_t maxExecutionTime;
    timeUs_t totalExecutionTime;    // total time consumed by task since boot
#endif
} cfTask_t;

extern cfTask_t cfTasks[TASK_COUNT];
extern uint16_t averageSystemLoadPercent;

void getCheckFuncInfo(cfCheckFuncInfo_t *checkFuncInfo);
void getTaskInfo(cfTaskId_e taskId, cfTaskInfo_t *taskInfo);
void rescheduleTask(cfTaskId_e taskId, uint32_t newPeriodMicros);
void setTaskEnabled(cfTaskId_e taskId, bool newEnabledState);
timeDelta_t getTaskDeltaTime(cfTaskId_e taskId);
void schedulerSetCalulateTaskStatistics(bool calculateTaskStatistics);
void schedulerResetTaskStatistics(cfTaskId_e taskId);

void schedulerInit(void);
void scheduler(void);
void taskSystem(timeUs_t currentTime);

#define LOAD_PERCENTAGE_ONE 100

#define isSystemOverloaded() (averageSystemLoadPercent >= LOAD_PERCENTAGE_ONE)



#endif /* SRC_MAIN_SCHELDULER_SCHEDULER_H_ */