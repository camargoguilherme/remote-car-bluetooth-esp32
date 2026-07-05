#pragma once

#include "controller/uni_gamepad.h"

#include "gamepad/gamepad_button_state.h"
#include "config/config.h"
#include "vehicle/vehicle_command.h"

void gamepad_map_controller(const uni_gamepad_t* controller, const AppConfig* config, VehicleCommand* command);
