#pragma once

#include "controller/uni_gamepad.h"

#include "config/config.h"
#include "gamepad/gamepad_mapper.h"
#include "gamepad/gamepad_logger.h"
#include "vehicle/vehicle_command.h"

void gamepad_init(void);
void gamepad_parse_controller(const uni_gamepad_t* controller, const AppConfig* config, VehicleCommand* command);
