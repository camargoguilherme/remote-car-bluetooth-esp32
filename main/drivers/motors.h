#pragma once

#include "config/config.h"
#include "drivers/motors.h"
#include "vehicle/vehicle.h"
#include "pwm.h"
#include "util.h"

void motors_init(const AppConfig* config);
void motors_stop(void);
void motors_apply(const VehicleState* state);
