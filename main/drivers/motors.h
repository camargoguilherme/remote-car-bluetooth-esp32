#pragma once

#include "common/util.h"
#include "config/config.h"
#include "drivers/motors.h"
#include "drivers/pwm.h"
#include "vehicle/vehicle.h"

void motors_init(const AppConfig* config);
void motors_stop(void);
void motors_apply(const VehicleState* state);
