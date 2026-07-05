#pragma once

#include "config/config.h"
#include "gamepad/gamepad.h"
#include "vehicle/vehicle.h"

typedef void (*BluetoothGamepadCallback)(const VehicleState* vehicle, void* context);

void bluetooth_init(const AppConfig* config, BluetoothGamepadCallback callback, void* context);
void bluetooth_run(void);
bool bluetooth_is_connected(void);
