# pragma once

#include "esp_err.h"

#include "bluetooth/bluetooth.h"
#include "common/failsafe.h"
#include "config/config.h"
#include "gamepad/gamepad.h"
#include "drivers/lights.h"
#include "drivers/motors.h"
#include "vehicle/vehicle.h"

esp_err_t app_init(void);
void app_run(void);
