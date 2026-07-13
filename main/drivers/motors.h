#pragma once

#include "config/config.h"

typedef enum
{
	MOTOR_1 = 0,
	MOTOR_2,
	MOTOR_COUNT
} MotorId;

void motors_init(const AppConfig* config);
void motors_set_speed(MotorId motor, int speed_percent);
void motors_stop(void);
