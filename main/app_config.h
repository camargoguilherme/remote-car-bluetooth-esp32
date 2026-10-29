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

#define BOARD_SERVO_TIMER                              LEDC_TIMER_1
#define BOARD_SERVO_RESOLUTION                         LEDC_TIMER_16_BIT
#define BOARD_SERVO_FREQUENCY_HZ                       50
#define BOARD_SERVO_MIN_PULSE_US                       1000
#define BOARD_SERVO_MAX_PULSE_US                       2000

/* ============================================================
 * Tasks
 * ============================================================ */

#define BOARD_TASK_STACK_VEHICLE_OUTPUT                 4096
#define BOARD_TASK_PRIORITY_VEHICLE_OUTPUT              7

#define BOARD_TASK_STACK_FAILSAFE                       3072
#define BOARD_TASK_PRIORITY_FAILSAFE                    8

#define BOARD_TASK_STACK_TELEMETRY                      4096
#define BOARD_TASK_PRIORITY_TELEMETRY                   3
#define BOARD_TELEMETRY_PERIOD_MS                       10000

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

/* ============================================================
 * ADC / Battery
 * ============================================================ */
#define BOARD_ADC_UNIT                                  ADC_UNIT_1
#define BOARD_ADC_CHANNEL                               ADC_CHANNEL_7 // GPIO35
#define BOARD_BATTERY_VOLTAGE_DIVIDER                   2.0f          // Ex: 10k/10k divider
#define BOARD_ADC_VREF_MV                               1100          // Internal VRef approx 1.1V

