#pragma once

#include "driver/gpio.h"

typedef enum
{
    DRIVE_ACKERMANN = 0,
    DRIVE_DIFFERENTIAL
} BoardDriveType;

#define BOARD_DRIVE_TYPE                                DRIVE_ACKERMANN

#define BOARD_LEFT_MOTOR                                MOTOR_1
#define BOARD_RIGHT_MOTOR                               MOTOR_2

#define BOARD_DRIVE_MOTOR                               MOTOR_1
#define BOARD_STEERING_MOTOR                            MOTOR_2

/* ============================================================
 * GPIO / CHANNELS
 * ============================================================ */
/* ============================================================
 * Motores
 * ============================================================ */
#define BOARD_CHANNEL_MOTOR1_A                          LEDC_CHANNEL_0
#define BOARD_CHANNEL_MOTOR1_B                          LEDC_CHANNEL_1

#define BOARD_PIN_MOTOR1_A                              GPIO_NUM_18
#define BOARD_PIN_MOTOR1_B                              GPIO_NUM_19

#define BOARD_CHANNEL_MOTOR2_A                          LEDC_CHANNEL_2
#define BOARD_CHANNEL_MOTOR2_B                          LEDC_CHANNEL_3

#define BOARD_PIN_MOTOR2_A                              GPIO_NUM_22
#define BOARD_PIN_MOTOR2_B                              GPIO_NUM_23

/* ============================================================
 * Faróis
 * ============================================================ */
#define BOARD_CHANNEL_HEADLIGHT                         LEDC_CHANNEL_4
#define BOARD_PIN_HEADLIGHT                             GPIO_NUM_25


/* ============================================================
 * Lanternas
 * GPIO_NUM_12 é pino de boot.
 * Mantê-lo desligado durante o reset.
 * ============================================================ */
#define BOARD_PIN_TAIL_LIGHT                            GPIO_NUM_14
#define BOARD_PIN_BRAKE_LIGHT                           GPIO_NUM_13
#define BOARD_PIN_REVERSE_LIGHT                         GPIO_NUM_12


/* ============================================================
 * Setas
 * ============================================================ */
#define BOARD_PIN_LEFT_INDICATOR                        GPIO_NUM_33
#define BOARD_PIN_RIGHT_INDICATOR                       GPIO_NUM_32

/* ============================================================
 * Buzina
 * ============================================================ */
#define BOARD_PIN_HORN                                  GPIO_NUM_5

void board_init(void);
