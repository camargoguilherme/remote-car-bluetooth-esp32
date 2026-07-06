# pragma once

#include "bluetooth/bluetooth.h"
#include "common/failsafe.h"
#include "config/config.h"
#include "gamepad/gamepad.h"
#include "drivers/lights.h"
#include "drivers/motors.h"
#include "vehicle/vehicle.h"

void app_init(void);
void app_run(void);
