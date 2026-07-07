#pragma once

#include "driver/gpio.h"

/* ============================================================
 * Parametros no código
 * ============================================================ */
/* ============================================================
 * PWM
 * ============================================================ */

#define BOARD_PWM_TIMER                                LEDC_TIMER_0
#define BOARD_PWM_RESOLUTION                           LEDC_TIMER_10_BIT
#define BOARD_PWM_FREQUENCY_HZ                         20000

/* ============================================================
 * Tasks
 * ============================================================ */

#define BOARD_TASK_STACK_VEHICLE_OUTPUT                 4096
#define BOARD_TASK_PRIORITY_VEHICLE_OUTPUT              5

#define BOARD_TASK_STACK_FAILSAFE                       3072
#define BOARD_TASK_PRIORITY_FAILSAFE                    5

#define BOARD_OUTPUT_UPDATE_INTERVAL_MS                 20

/* ============================================================
 * Lights
 * ============================================================ */

#define BOARD_HEADLIGHT_LOW_PERCENT                     30
#define BOARD_HEADLIGHT_MEDIUM_PERCENT                  60
#define BOARD_HEADLIGHT_HIGH_PERCENT                    100
#define BOARD_BLINK_INTERVAL_MS                         500

/* ============================================================
 * Failsafe
 * ============================================================ */
#define BOARD_FAILSAFE_PERIOD_MS                        50

void board_init(void);
