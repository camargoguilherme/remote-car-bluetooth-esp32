#pragma once

#include "driver/gpio.h"

/* ============================================================
 * Motores
 * ============================================================ */

#define BOARD_PIN_LEFT_MOTOR_PWM        GPIO_NUM_19
#define BOARD_PIN_LEFT_MOTOR_DIRECTION  GPIO_NUM_21

#define BOARD_PIN_RIGHT_MOTOR_PWM       GPIO_NUM_25
#define BOARD_PIN_RIGHT_MOTOR_DIRECTION GPIO_NUM_26

/* ============================================================
 * Faróis
 * ============================================================ */

#define BOARD_PIN_HEADLIGHT_LOW         GPIO_NUM_32
#define BOARD_PIN_HEADLIGHT_MEDIUM      GPIO_NUM_33
#define BOARD_PIN_HEADLIGHT_HIGH        GPIO_NUM_27

/* ============================================================
 * Lanternas
 * ============================================================ */

#define BOARD_PIN_TAIL_LIGHT            GPIO_NUM_14
#define BOARD_PIN_BRAKE_LIGHT           GPIO_NUM_13
#define BOARD_PIN_REVERSE_LIGHT         GPIO_NUM_12

/* ============================================================
 * Setas
 * ============================================================ */

#define BOARD_PIN_LEFT_INDICATOR        GPIO_NUM_4
#define BOARD_PIN_RIGHT_INDICATOR       GPIO_NUM_2

/* ============================================================
 * Buzina
 * ============================================================ */

#define BOARD_PIN_HORN                  GPIO_NUM_15

void board_init(void);
