#pragma once

#include "driver/gpio.h"

/* ============================================================
 * Motores
 * ============================================================ */
#define BOARD_CHANNEL_DRIVE_MOTOR_A         LEDC_CHANNEL_0
#define BOARD_CHANNEL_DRIVE_MOTOR_B         LEDC_CHANNEL_1

#define BOARD_PIN_DRIVE_MOTOR_A             GPIO_NUM_18
#define BOARD_PIN_DRIVE_MOTOR_B             GPIO_NUM_19

#define BOARD_CHANNEL_STEERING_MOTOR_A      LEDC_CHANNEL_2
#define BOARD_CHANNEL_STEERING_MOTOR_B      LEDC_CHANNEL_3

#define BOARD_PIN_STEERING_MOTOR_A          GPIO_NUM_22
#define BOARD_PIN_STEERING_MOTOR_B          GPIO_NUM_23

/* ============================================================
 * Faróis
 * ============================================================ */
#define BOARD_CHANNEL_HEADLIGHT             LEDC_CHANNEL_4
#define BOARD_PIN_HEADLIGHT                 GPIO_NUM_25


/* ============================================================
 * Lanternas
 * GPIO_NUM_12 é pino de boot.
 * Mantê-lo desligado durante o reset.
 * ============================================================ */

#define BOARD_PIN_TAIL_LIGHT                GPIO_NUM_14
#define BOARD_PIN_BRAKE_LIGHT               GPIO_NUM_13
#define BOARD_PIN_REVERSE_LIGHT             GPIO_NUM_12
#define BOARD_BLINK_INTERVAL_MS             500           // Intervalo de piscada em milissegundos

/* ============================================================
 * Setas
 * ============================================================ */

#define BOARD_PIN_LEFT_INDICATOR            GPIO_NUM_33
#define BOARD_PIN_RIGHT_INDICATOR           GPIO_NUM_32

/* ============================================================
 * Buzina
 * ============================================================ */

#define BOARD_PIN_HORN                      GPIO_NUM_15

void board_init(void);
